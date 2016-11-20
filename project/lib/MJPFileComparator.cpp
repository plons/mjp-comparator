#include "MJPFileComparator.h"

#include <boost/optional.hpp>
#include <iomanip>

using boost::optional;
using boost::filesystem::path;
using std::vector;

namespace std {
inline std::ostream& operator<<(std::ostream& ws, const std::vector<double>& amounts)
{
	ws << std::fixed << std::setprecision(2);
	std::copy(amounts.begin(), amounts.end(), std::ostream_iterator<double>(ws, " "));
	return ws;
}
}  // namespace std

MJPFileComparator::MJPFileComparator(MJPEntry::Type type, uint32_t year, const path& newFoxBeleidFile, const vector<path>& customFilesPaths) :
		foxBeleidFile(newFoxBeleidFile, MJPEntry::factoryFunction(MJPEntry::FOXBELEID, type, year))
{
	for (auto& customFilePath : customFilesPaths)
	{
		customFiles.emplace_back(customFilePath, MJPEntry::factoryFunction(MJPEntry::CUSTOM_FILE, type, year));
	}
	std::cout << "Comparing following files:\n";
	std::cout << "\tFoxbeleid file: " << foxBeleidFile.getPath().filename() << " containing " << foxBeleidFile.getAllEntries().size() << " entries." << std::endl;
	for (auto& customFile : customFiles)
	{
		std::cout << "\tCustom file: " << customFile.getPath().filename() << " containing " << customFile.getAllEntries().size() << " entries." << std::endl;
	}
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
		std::cout << "\tFound " << possibleMatches.size() << " possible matches with same amounts " << criteria.getAmounts() << ":\n";
		for (auto& possibleMatch : possibleMatches)
		{
			std::cout << "\tPossible match: " << possibleMatch.getKey() << "\n";
		}
	}
	else
	{
		std::cout << "\tMissing amounts: " << criteria.getAmounts() << std::endl;
	}
}

void MJPFileComparator::printEntriesMissingInFoxBeleid() const
{
	for (auto& customFile : customFiles)
	{
		auto missingEntries = getEntriesMissingInFoxBeleid(customFile);
		if (!missingEntries.empty())
		{
			std::cout << "========================================================" << std::endl;
			std::cout << "Verifying whether all entries from file " << customFile.getPath().filename() << " are present in the file from foxbeleid." << std::endl;
			std::cout << "========================================================" << std::endl;
			for (auto& entry : missingEntries)
			{
				std::cout << "Could not find following key in foxBeleid mjp file: " << entry.getKey() << "\n";
				auto possibleMatches = findPossibleMatches(foxBeleidFile, entry);
				printPossibleMatches(possibleMatches, entry);
			}
		}
	}
}

void MJPFileComparator::printEntriesMissingInCustomFiles() const
{
	auto missingEntries = getEntriesMissingInCustomFiles();
	if (!missingEntries.empty())
	{
		std::cout << "========================================================" << std::endl;
		std::cout << "Verifying whether all entries from file " << foxBeleidFile.getPath().filename() << " are present in the custom files." << std::endl;
		std::cout << "========================================================" << std::endl;
		for (auto& entry : missingEntries)
		{
			std::cout << "Could not find following key in custom mjp file: " << entry.getKey() << "\n";
			auto possibleMatches = findPossibleMatches(foxBeleidFile, entry);
			printPossibleMatches(possibleMatches, entry);
		}
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
	for (uint i = 1; i < amounts.size(); ++i)
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

std::ostream& operator<<(std::ostream& ws, const MismatchingAmounts& obj)
{
	ws << "Amounts did not match for key " << obj.customEntry.getKey() << "\n";
	ws << "\tCustom file: " << obj.customEntry.getAmounts() << "\n";
	ws << "\tFox beleid:  " << obj.foxbeleidEntry.getAmounts() << "\n";
	return ws;
}

static vector<MismatchingAmounts> findMismatchingAmounts(const MJPFile& customFile, const MJPFile& foxBeleidFile)
{
	vector<MismatchingAmounts> mismatchingAmounts;
	for (auto& entry : customFile.getAllEntries())
	{
		if (foxBeleidFile.containsKey(entry.getKey()))
		{
			auto& customEntry = entry;
			auto& foxbeleidEntry = foxBeleidFile.getEntry(entry.getKey());

#ifdef COMPARE_2016
			if (customEntry.getAmounts().size() == 1 && customEntry.getAmounts().at(0) == 0)
			{
				continue;
			}
#endif
			if (customEntry.getAmounts() != foxbeleidEntry.getAmounts())
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
		auto mismatchingAmountsVector = findMismatchingAmounts(customFile, foxBeleidFile);
		if (!mismatchingAmountsVector.empty())
		{
			std::cout << "========================================================" << std::endl;
			std::cout << "Printing mismatching amounts from file " << customFile.getPath().filename() << std::endl;
			std::cout << "========================================================" << std::endl;
			for (auto& entry : mismatchingAmountsVector)
			{
				std::cout << entry;
			}
		}
	}
}
