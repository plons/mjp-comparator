#include "MJPFileComparator.h"

using boost::filesystem::path;
using std::vector;

int main(int argc, const char** argv)
{
	path foxBeleidMJPFile{path(DATA_DIR)/ "20151114_1056_MJP_foxbeleid.csv"};
	vector<path> customMJPFiles({
		{path(DATA_DIR)/ "20151114_1056_MJP_exploitatie-ontvangsten_v6.csv"},
		{path(DATA_DIR)/ "20151114_1056_MJP_exploitatie-uitgaven_v7.csv"},
		{path(DATA_DIR)/ "20151114_1056_MJP_investeringen-ontvangsten_v5.csv"},
		{path(DATA_DIR)/ "20151114_1056_MJP_investeringen-uitgaven_v7.csv"},
		{path(DATA_DIR)/ "20151114_1056_MJP_liquiditeiten-uitgaven+ontvangsten_v4.csv"},
	});

	MJPFileComparator comparator(foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
	comparator.printMismatchingAmounts();
	std::cout << "Thank you, come again!" << std::endl;
}
