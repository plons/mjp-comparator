#include "MJPFileComparator.h"

using boost::filesystem::path;
using std::vector;


MJPFileComparator::MJPFileComparator(const path& newFoxBeleidFile, const path& newCustomFile) :
		foxBeleidFile(newFoxBeleidFile, MJPEntry::fromFoxBeleidFile),
		customFile(newCustomFile, MJPEntry::fromCustomFile)
{
	std::cout << "Comparing following files:\n";
	std::cout << "\tFoxbeleid file: " << foxBeleidFile.getPath() << " containing " << foxBeleidFile.getAllEntries().size() << " entries." << std::endl;
	std::cout << "\tCustom file: " << customFile.getPath() << " containing " << customFile.getAllEntries().size() << " entries." << std::endl;
	std::cout << "========================================================" << std::endl;
}

vector<MJPEntry> MJPFileComparator::getEntriesMissingInFoxBeleid() const
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

vector<MJPEntry> MJPFileComparator::getEntriesMissingInCustomFile() const
{
	vector<MJPEntry> missingEntries;
	for (auto& entry : foxBeleidFile.getAllEntries())
	{
		if (!customFile.containsKey(entry.getKey()))
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


void MJPFileComparator::printMismatchingAmounts() const
{

	for (auto& entry : customFile.getAllEntries())
	{
		if (foxBeleidFile.containsKey(entry.getKey()))
		{
			auto& customEntry = entry;
			auto& foxbeleidEntry = foxBeleidFile.getEntry(entry.getKey());

			if (customEntry.getAmounts() != foxbeleidEntry.getAmounts())
			{
				std::cerr << "Amounts did not match for key " << entry.getKey() << "\n";
				std::cerr << "\tCustom file: " << customEntry.getAmounts() << "\n";
				std::cerr << "\tFox beleid:  " << foxbeleidEntry.getAmounts() << "\n";
			}
		}
	}
}
