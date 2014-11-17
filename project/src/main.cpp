#include "MJPFile.hpp"

#include <boost/filesystem.hpp>

using boost::filesystem::path;

namespace std {

std::ostream& operator<<(std::ostream& ws, const std::vector<double>& amounts)
{
	std::copy(amounts.begin(), amounts.end(), std::ostream_iterator<double>(ws, " "));
	return ws;
}

}  // namespace std

int main(int argc, const char** argv)
{
	MJPFile foxBeleidMJPFile{path(DATA_DIR)/ "20141117_2230_MJP_foxbeleid.csv", convertFoxBeleidMJPLine};
	MJPFile customMJPFile   {path(DATA_DIR)/ "20141117_2230_MJP_custom.csv",    convertCustomMJPLine};

	for (auto& entry : customMJPFile.getAllEntries())
	{
		if (!foxBeleidMJPFile.containsKey(entry.getKey()))
		{
			std::cerr << "Could not find following key in foxBeleid mjp file: " << entry.getKey() << "\n";
		}
	}

	for (auto& entry : foxBeleidMJPFile.getAllEntries())
	{
		if (!customMJPFile.containsKey(entry.getKey()))
		{
			std::cerr << "Could not find following key in custom mjp file: " << entry.getKey() << "\n";
		}
	}

	for (auto& entry : customMJPFile.getAllEntries())
	{
		if (foxBeleidMJPFile.containsKey(entry.getKey()))
		{
			auto& customEntry = entry;
			auto& foxbeleidEntry = foxBeleidMJPFile.getEntry(entry.getKey());

			if (customEntry.getAmounts() != foxbeleidEntry.getAmounts())
			{
				std::cerr << "Amounts did not match for key " << entry.getKey() << "\n";
				std::cerr << "\tCustom file: " << customEntry.getAmounts() << "\n";
				std::cerr << "\tFox beleid:  " << foxbeleidEntry.getAmounts() << "\n";
			}
		}
	}
}
