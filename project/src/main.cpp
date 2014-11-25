#include "MJPFileComparator.h"

using boost::filesystem::path;

int main(int argc, const char** argv)
{
	path foxBeleidMJPFile{path(DATA_DIR)/ "20141120_0330_MJP_foxbeleid.csv"};
	path customMJPFile   {path(DATA_DIR)/ "20141120_0330_MJP_custom.csv"};
	MJPFileComparator comparator(foxBeleidMJPFile, customMJPFile);

	for (auto& entry : comparator.getEntriesMissingInFoxBeleid())
	{
		std::cerr << "Could not find following key in foxBeleid mjp file: " << entry.getKey() << "\n";
	}

	for (auto& entry : comparator.getEntriesMissingInCustomFile())
	{
		std::cerr << "Could not find following key in custom mjp file: " << entry.getKey() << "\n";
	}

	comparator.printMismatchingAmounts();
	std::cout << "Thank you, come again!" << std::endl;
}
