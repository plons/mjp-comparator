#include "MJPFileComparator.h"

#include <boost/optional.hpp>

using boost::optional;
using boost::filesystem::path;
using std::vector;


MJPFileComparator::MJPFileComparator(const path& newFoxBeleidFile, const vector<path>& customFilesPaths) :
		foxBeleidFile(newFoxBeleidFile, MJPEntry::fromFoxBeleidFile)
{
	for (auto& customFilePath : customFilesPaths)
	{
		customFiles.emplace_back(customFilePath, MJPEntry::fromCustomFile);
	}
	std::cout << "Comparing following files:\n";
	std::cout << "\tFoxbeleid file: " << foxBeleidFile.getPath().filename() << " containing " << foxBeleidFile.getAllEntries().size() << " entries." << std::endl;
	for (auto& customFile : customFiles)
	{
		std::cout << "\tCustom file: " << customFile.getPath().filename() << " containing " << customFile.getAllEntries().size() << " entries." << std::endl;
	}
}


void MJPFileComparator::printEntriesMissingInFoxBeleid() const
{
	for (auto& customFile : customFiles)
	{
		std::cout << "========================================================" << std::endl;
		std::cout << "Verifying whether all entries from file " << customFile.getPath().filename() << " are present in the file from foxbeleid." << std::endl;
		std::cout << "========================================================" << std::endl;
		for (auto& entry : getEntriesMissingInFoxBeleid(customFile))
		{
			std::cout << "Could not find following key in foxBeleid mjp file: " << entry.getKey() << "\n";
		}
	}
}

void MJPFileComparator::printEntriesMissingInCustomFiles() const
{
	std::cout << "========================================================" << std::endl;
	std::cout << "Verifying whether all entries from file " << foxBeleidFile.getPath().filename() << " are present in the custom files." << std::endl;
	std::cout << "========================================================" << std::endl;
	for (auto& entry : getEntriesMissingInCustomFiles())
	{
		std::cout << "Could not find following key in custom mjp file: " << entry.getKey() << "\n";
	}
}


vector<MJPEntry> MJPFileComparator::getEntriesMissingInFoxBeleid(const MJPFile& customFile) const
{
	vector<MJPEntry> missingEntries;
	for (auto& entry : customFile.getAllEntries())
	{
		if (!foxBeleidFile.containsKey(entry.getKey()))
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


namespace std {

static std::ostream& operator<<(std::ostream& ws, const std::vector<double>& amounts)
{
	std::copy(amounts.begin(), amounts.end(), std::ostream_iterator<double>(ws, " "));
	return ws;
}

}  // namespace std


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
