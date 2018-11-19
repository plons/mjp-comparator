#include "MJPFileComparator.h"

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>

//#define YEAR_2016_HACK_TO_REMOVE_PERSONEL_ENTRIES_FROM_EXPENSES_FILE

using boost::optional;
using boost::filesystem::path;
using boost::format;
using std::all_of;
using std::cout;
using std::copy;
using std::endl;
using std::map;
using std::ostream;
using std::ostream_iterator;
using std::fixed;
using std::round;
using std::runtime_error;
using std::setprecision;
using std::string;
using std::vector;

namespace std {
inline ostream& operator<<(ostream& ws, const vector<double>& amounts)
{
	ws << fixed << setprecision(2);
	copy(amounts.begin(), amounts.end(), ostream_iterator<double>(ws, " "));
	return ws;
}
}  // namespace std

inline void printTitle(const string& title)
{
	cout << "======================================================================" << endl;
	cout << title << endl;
	cout << "======================================================================" << endl;
}

MJPFileComparator::MJPFileComparator(MJPEntry::Type type, uint32_t year, const path& newFoxBeleidFile, const vector<path>& customFilesPaths) :
		foxBeleidFile(newFoxBeleidFile, MJPEntry::factoryFunction(MJPEntry::FOXBELEID, type, year))
{
	for (auto& customFilePath : customFilesPaths)
	{
		customFiles.emplace_back(customFilePath, MJPEntry::factoryFunction(MJPEntry::CUSTOM_FILE, type, year));
	}
	cout << "Comparing following files:\n";
	cout << "\tFoxbeleid file: " << foxBeleidFile.getPath().filename() << " containing " << foxBeleidFile.getAllEntries().size() << " entries." << endl;
	for (auto& customFile : customFiles)
	{
		cout << "\tCustom file: " << customFile.getPath().filename() << " containing " << customFile.getAllEntries().size() << " entries." << endl;
	}

#ifdef YEAR_2016_HACK_TO_REMOVE_PERSONEL_ENTRIES_FROM_EXPENSES_FILE
	MJPFile* personelFilePtr = nullptr;
	MJPFile* expensesFilePtr = nullptr;
	for (auto& customFile : customFiles)
	{
		if (customFile.getPath().filename() == "exploitatie-uitgaven_lonen_v2-1.csv") personelFilePtr = &customFile;
		if (customFile.getPath().filename() == "exploitatie-uitgaven_v8.csv") expensesFilePtr = &customFile;
	}

	if (personelFilePtr != nullptr && expensesFilePtr != nullptr)
	{
		cout << "Detected personnel file: " << personelFilePtr->getPath() << endl;
		cout << "Detected expense file: " << expensesFilePtr->getPath() << endl;
		auto allPersonelEntries = personelFilePtr->getAllEntries();
		for (auto iter = allPersonelEntries.begin(); iter != allPersonelEntries.end(); ++iter)
		{
			if (expensesFilePtr->containsKey(iter->getKey()))
			{
				// Yet another workaround
				if (iter->getKey().algemeneRekening == "6231003" && round(100*iter->getAmounts().back()) == 0)
				{
					personelFilePtr->removeEntry(iter->getKey());
				}
				else
				{
					expensesFilePtr->removeEntry(iter->getKey());
				}
			}
		}
	}
	else
	{
		throw runtime_error("Could not detect personel file and expense file (CUSTOM PROCESSING 2016)");
	}
#endif
}

vector<MJPEntry> findPossibleMatches(const MJPFile& file, const MJPEntry& criteria)
{
	vector<MJPEntry> possibleMatches;
	for (auto& entry : file.getAllEntries())
	{
		if (entry.getAmounts() == criteria.getAmounts())
		{
			int matchingCount = 0;
			matchingCount += (entry.getKey().actie == criteria.getKey().actie);
			matchingCount += (entry.getKey().beleidsItem == criteria.getKey().beleidsItem);
			matchingCount += (entry.getKey().algemeneRekening == criteria.getKey().algemeneRekening);
			matchingCount += (entry.getKey().investeringsEnveloppe == criteria.getKey().investeringsEnveloppe);

			if (matchingCount == 3)
			{
				possibleMatches.push_back(entry);
			}
		}
	}
	return possibleMatches;
}

vector<MJPEntry> findPossibleMatches(const vector<MJPFile>& files, const MJPEntry& criteria)
{
	vector<MJPEntry> possibleMatches;
	for (const auto& currentFile : files)
	{
		const auto& currentMatches = findPossibleMatches(currentFile, criteria);
		possibleMatches.insert(possibleMatches.end(), currentMatches.begin(), currentMatches.end());
	}
	return possibleMatches;
}

void printPossibleMatches(const vector<MJPEntry>& possibleMatches, const MJPEntry& criteria)
{
	if (!possibleMatches.empty())
	{
		cout << "\tFound " << possibleMatches.size() << " possible matches with same amounts " << criteria.getAmounts() << ":\n";
		for (auto& possibleMatch : possibleMatches)
		{
			cout << "\tPossible match: " << possibleMatch.getKey() << "\n";
		}
	}
	else
	{
		cout << "\tMissing amounts: " << criteria.getAmounts() << endl;
	}
}

void MJPFileComparator::printEntriesMissingInFoxBeleid() const
{
	bool allEntriesPresent = true;
	for (auto& customFile : customFiles)
	{
		auto missingEntries = getEntriesMissingInFoxBeleid(customFile);
		if (!missingEntries.empty())
		{
			printTitle((format("Verifying whether all entries from file %s are present in the file from foxbeleid.")%customFile.getPath().filename()).str());
			for (auto& entry : missingEntries)
			{
				allEntriesPresent = false;
				cout << "Could not find following key in foxBeleid file: " << entry.getKey() << "\n";
				auto possibleMatches = findPossibleMatches(foxBeleidFile, entry);
				printPossibleMatches(possibleMatches, entry);
			}
		}
	}
	if (allEntriesPresent)
	{
		printTitle("All entries from the custom files are present in the foxbeleid file!");
	}
}

void MJPFileComparator::printEntriesMissingInCustomFiles() const
{
	auto missingEntries = getEntriesMissingInCustomFiles();
	if (!missingEntries.empty())
	{
		printTitle((format("Verifying whether all entries from file %s are present in the custom files.")%foxBeleidFile.getPath().filename()).str());
		for (auto& entry : missingEntries)
		{
			cout << "Could not find following key in custom mjp file: " << entry.getKey() << "\n";
			auto possibleMatches = findPossibleMatches(foxBeleidFile, entry);
			printPossibleMatches(possibleMatches, entry);
		}
	}
	else
	{
		printTitle("All entries from the foxbeleid file are present in the custom files!");
	}
}


vector<MJPEntry> MJPFileComparator::getEntriesMissingInFoxBeleid(const MJPFile& customFile) const
{
	vector<MJPEntry> missingEntries;
	for (auto& entry : customFile.getAllEntries())
	{
		if (!foxBeleidFile.containsKey(entry.getKey()) && entry.totalAmount() != 0)
		{
			missingEntries.push_back(entry);
		}
	}
	return missingEntries;
}

static bool onlyFirstAmountIsNotZero(const vector<double>& amounts)
{
	assert(amounts.size() > 0);
	for (auto i = 1U; i < amounts.size(); ++i)
	{
		if (amounts[i] > 0) return false;
	}
	return true;
}

bool containsKey(const vector<MJPFile>& customFiles, const MJPEntryKey& key)
{
	for (auto& customFile : customFiles)
	{
		if (customFile.containsKey(key)) return true;
	}
	return false;
}

vector<MJPEntry> MJPFileComparator::getEntriesMissingInCustomFiles() const
{
	vector<MJPEntry> missingEntries;
	for (auto& entry : foxBeleidFile.getAllEntries())
	{
		if (!containsKey(customFiles, entry.getKey()))
		{

			if (!onlyFirstAmountIsNotZero(entry.getAmounts()))
			{
				missingEntries.push_back(entry);
			}
		}
	}
	return missingEntries;
}

struct MismatchingAmounts
{
	MismatchingAmounts(const MJPEntry& custom, const MJPEntry& foxbeleid) :
			customEntry(custom),
			foxbeleidEntry(foxbeleid)
	{
	}

	MJPEntry customEntry;
	MJPEntry foxbeleidEntry;
};

ostream& operator<<(ostream& ws, const MismatchingAmounts& obj)
{
	ws << "Amounts did not match for key " << obj.customEntry.getKey() << "\n";
	ws << "\tCustom file: " << obj.customEntry.getAmounts() << "\n";
	ws << "\tFox beleid:  " << obj.foxbeleidEntry.getAmounts() << "\n";
	return ws;
}

static bool amountsAreEqual(const vector<double>& list1, const vector<double>& list2, double allowedDifference)
{
	if (list1.size() != list2.size()) return false;

	for (auto i = 0U; i < list1.size(); ++i)
	{
		double amount1 = list1.at(i);
		double amount2 = list2.at(i);

		if (std::abs(amount1 - amount2) > allowedDifference)
		{
			return false;
		}
	}
	return true;
}

static vector<MismatchingAmounts> findMismatchingAmounts(const MJPFile& customFile, const MJPFile& foxBeleidFile, double allowedDifference)
{
	vector<MismatchingAmounts> mismatchingAmounts;
	for (auto& entry : customFile.getAllEntries())
	{
		if (foxBeleidFile.containsKey(entry.getKey()))
		{
			auto& customEntry = entry;
			auto& foxbeleidEntry = foxBeleidFile.getEntry(entry.getKey());

			if (!amountsAreEqual(customEntry.getAmounts(), foxbeleidEntry.getAmounts(), allowedDifference))
			{
				mismatchingAmounts.push_back(MismatchingAmounts(customEntry, foxbeleidEntry));
			}
		}
	}
	return mismatchingAmounts;
}

void MJPFileComparator::printMismatchingAmounts() const
{
	for (auto& customFile : customFiles)
	{
		// For 2017: The salary amounts are rounded in the bookkeeping but not in the custom files
		double allowedDifference = 0.01;
		if (customFile.getPath().filename() == "exploitatie-uitgaven_lonen_v2-1.csv")
		{
			allowedDifference = 1.5;
		}

		auto mismatchingAmountsVector = findMismatchingAmounts(customFile, foxBeleidFile, allowedDifference);
		if (!mismatchingAmountsVector.empty())
		{
			printTitle("Printing mismatching amounts from file " + customFile.getPath().filename().string());
			for (auto& entry : mismatchingAmountsVector)
			{
				cout << entry;
			}
		}
	}
}
