#include "MJPFileComparator.h"

#include <boost/filesystem.hpp>

#include <iostream>

using boost::filesystem::path;
using std::vector;


inline path dataDir(uint32_t year)
{
	return path(DATA_DIR) / std::to_string(year);
}

inline vector<path> csvFiles(const path& directory)
{
	using namespace boost::filesystem;
	assert(is_directory(directory));

	vector<path> result;
	for (directory_iterator iter{directory}, end{}; iter != end; ++iter)
	{
		if (iter->path().extension() != ".csv") continue;
		result.push_back(iter->path());
	}
	std::sort(result.begin(), result.end());
	return result;
}

inline void compareMJP2015Try1()
{
	path foxBeleidMJPFile{dataDir(2015) / "20151114_1056_MJP_foxbeleid.csv"};
	vector<path> customMJPFiles({
		{dataDir(2015) / "20151114_1056_MJP_exploitatie-ontvangsten_v6.csv"},
		{dataDir(2015) / "20151114_1056_MJP_exploitatie-uitgaven_v7.csv"},
		{dataDir(2015) / "20151114_1056_MJP_investeringen-ontvangsten_v5.csv"},
		{dataDir(2015) / "20151114_1056_MJP_investeringen-uitgaven_v7.csv"},
		{dataDir(2015) / "20151114_1056_MJP_liquiditeiten-uitgaven+ontvangsten_v4.csv"},
	});

	MJPFileComparator comparator(MJPEntry::MJP, 2015, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
	comparator.printMismatchingAmounts();
}

inline void compareMJP2015()
{
	path foxBeleidMJPFile{dataDir(2015) / "20151117_2046_MJP_foxbeleid.csv"};
	vector<path> customMJPFiles({
		{dataDir(2015) / "20151117_2046_MJP_exploitatie-ontvangsten_v6.csv"},
		{dataDir(2015) / "20151117_2046_MJP_exploitatie-uitgaven_v7.csv"},
		{dataDir(2015) / "20151117_2046_MJP_investeringen-ontvangsten_v5.csv"},
		{dataDir(2015) / "20151117_2046_MJP_investeringen-uitgaven_v7.csv"},
		{dataDir(2015) / "20151117_2046_MJP_liquiditeiten-uitgaven+ontvangsten_v5.csv"},
	});

	MJPFileComparator comparator(MJPEntry::MJP, 2015, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
	comparator.printMismatchingAmounts();
}

inline void compareBudgetChange2016()
{
	path foxBeleidMJPFile{dataDir(2016) / "budget-change/20160817_0740_BW1-2016_foxbeleid.csv"};
	vector<path> customMJPFiles({
		{dataDir(2016) / "budget-change/20160817_Budgetwijziging_Ontvangsten.csv"},
		{dataDir(2016) / "budget-change/20160817_Budgetwijziging_Uitgaven.csv"},
	});

	MJPFileComparator comparator(MJPEntry::BUDGET_CHANGE, 2016, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printMismatchingAmounts();
}

inline void compareMJP2016()
{
	path foxBeleidMJPFile{dataDir(2016) / "mjp/foxbeleid_23-11-2016_21u51.csv"};
	vector<path> customMJPFiles({
		{dataDir(2016) / "mjp/exploitatie-ontvangsten_v2.csv"},
		{dataDir(2016) / "mjp/exploitatie-uitgaven_62_(personeel)_v2.csv"},
		{dataDir(2016) / "mjp/exploitatie-uitgaven_v3.csv"},
		{dataDir(2016) / "mjp/investerings-ontvangsten_v2.csv"},
		{dataDir(2016) / "mjp/investerings-uitgaven_v3.csv"},
		{dataDir(2016) / "mjp/liquiditeiten.csv"},
	});

	MJPFileComparator comparator(MJPEntry::MJP, 2016, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
	comparator.printMismatchingAmounts();
}

inline void compareMJP2017()
{
	//std::cout << MJPFile(dataDir(2017) / "foxbeleid_19-11-2017_21u24.csv", MJPEntry::factoryFunction(MJPEntry::FOXBELEID, MJPEntry::MJP, 2017)) << std::endl;
	//std::cout << MJPFile(dataDir(2017) / "investerings-ontvangsten_v6.csv", MJPEntry::factoryFunction(MJPEntry::CUSTOM_FILE, MJPEntry::MJP, 2017)) << std::endl;
	path foxBeleidMJPFile{dataDir(2017) / "foxbeleid_21-11-2017_8u04.csv"};
	vector<path> customMJPFiles({
		{dataDir(2017) / "exploitatie-ontvangsten_v7-1.csv"},
		{dataDir(2017) / "exploitatie-uitgaven_lonen_v2-1.csv"},
		{dataDir(2017) / "exploitatie-uitgaven_v8.csv"},
		{dataDir(2017) / "investerings-ontvangsten_v7-1.csv"},
		{dataDir(2017) / "investerings-uitgaven_v7-2.csv"},
		{dataDir(2017) / "liquiditeiten_v2.csv"},
	});

	MJPFileComparator comparator(MJPEntry::MJP, 2017, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
//	comparator.printMismatchingAmounts();
}

inline void compareMJP2018()
{
	auto foxBeleidMJPFile = dataDir(2018) / "mjp/foxbeleid_21-11-2018_21u59.csv";
	auto customMJPFiles = csvFiles(dataDir(2018) / "budget-change");

	MJPFileComparator comparator(MJPEntry::MJP, 2018, foxBeleidMJPFile, customMJPFiles);
	comparator.printEntriesMissingInFoxBeleid();
	comparator.printEntriesMissingInCustomFiles();
	comparator.printMismatchingAmounts();
}


int main(int argc, const char** argv)
{
//	std::cout << MJPFile(dataDir(2018) / "mjp/foxbeleid_19-11-2018_19u24.csv", MJPEntry::fromFoxBeleidMJP2016) << std::endl;
//	std::cout << MJPFile(dataDir(2018) / "budget-change/liquiditeiten_IB2019_v1.csv", MJPEntry::fromCustomFileMJP2018) << std::endl;
	compareMJP2018();
	std::cout << "Thank you, come again!" << std::endl;
}
