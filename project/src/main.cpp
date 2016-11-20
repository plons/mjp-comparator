#include "MJPFileComparator.h"

using boost::filesystem::path;
using std::vector;

inline void compareBudget2015Try1()
{
	path foxBeleidMJPFile{path(DATA_DIR)/ "2015/20151114_1056_MJP_foxbeleid.csv"};
	vector<path> customMJPFiles({
		{path(DATA_DIR)/ "2015/20151114_1056_MJP_exploitatie-ontvangsten_v6.csv"},
		{path(DATA_DIR)/ "2015/20151114_1056_MJP_exploitatie-uitgaven_v7.csv"},
		{path(DATA_DIR)/ "2015/20151114_1056_MJP_investeringen-ontvangsten_v5.csv"},
		{path(DATA_DIR)/ "2015/20151114_1056_MJP_investeringen-uitgaven_v7.csv"},
		{path(DATA_DIR)/ "2015/20151114_1056_MJP_liquiditeiten-uitgaven+ontvangsten_v4.csv"},
	});

	MJPFileComparator comparator(MJPEntry::MJP, 2015, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
	comparator.printMismatchingAmounts();
}

inline void compareBudget2015()
{
	path foxBeleidMJPFile{path(DATA_DIR)/ "2015/20151117_2046_MJP_foxbeleid.csv"};
	vector<path> customMJPFiles({
		{path(DATA_DIR)/ "2015/20151117_2046_MJP_exploitatie-ontvangsten_v6.csv"},
		{path(DATA_DIR)/ "2015/20151117_2046_MJP_exploitatie-uitgaven_v7.csv"},
		{path(DATA_DIR)/ "2015/20151117_2046_MJP_investeringen-ontvangsten_v5.csv"},
		{path(DATA_DIR)/ "2015/20151117_2046_MJP_investeringen-uitgaven_v7.csv"},
		{path(DATA_DIR)/ "2015/20151117_2046_MJP_liquiditeiten-uitgaven+ontvangsten_v5.csv"},
	});

	MJPFileComparator comparator(MJPEntry::MJP, 2015, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
	comparator.printMismatchingAmounts();
}

inline void compareBudgetChange2016()
{
	path foxBeleidMJPFile{path(DATA_DIR)/ "2016/20160817_0740_BW1-2016_foxbeleid.csv"};
	vector<path> customMJPFiles({
		{path(DATA_DIR)/ "2016/20160817_Budgetwijziging_Ontvangsten.csv"},
		{path(DATA_DIR)/ "2016/20160817_Budgetwijziging_Uitgaven.csv"},
	});

	MJPFileComparator comparator(MJPEntry::BUDGET_CHANGE, 2016, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printMismatchingAmounts();
}

int main(int argc, const char** argv)
{
	compareBudgetChange2016();
	std::cout << "Thank you, come again!" << std::endl;
}
