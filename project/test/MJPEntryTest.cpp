#include <gmock/gmock.h>

#include "MJPEntry.hpp"

using namespace testing;

class LineFromFoxBeleidMJPFile : public Test
{
public:
	std::string line{
			"BP2014_2019-1/2.1.1.1/0909-00/6494000/BESTUUR/CBS/IE-GEEN/U"
			";2.1.1.1"
			";0909-00"
			";6494000"
			";IE-GEEN"
			";Toegestane werkingssubsidies aan andere overheidsinstellingen"
			";€ 799,416.15;€ 917,523.45;€ 927,129.72;€ 953,644.22;€ 985,377.10;€ 1,007,216.94"};

	MJPEntryKey expectedKey{"2.1.1.1", "0909-00", "6494000", "IE-GEEN", "U"};
	std::vector<double> expectedAmounts{917523.45, 927129.72, 953644.22, 985377.10, 1007216.94};
};

TEST_F(LineFromFoxBeleidMJPFile, canBeParsed)
{
	auto entry = MJPEntry::fromFoxBeleidFile(line);

	ASSERT_THAT(entry.getKey(), Eq(expectedKey));
	ASSERT_THAT(entry.getAmounts(), Eq(expectedAmounts));
}

class LineFromCustomMJPFile : public Test
{
public:
	std::string line{"BP2014_2019-0/GBB-CBS/0020-00/6479999/BESTUUR/CBS/IE-GEEN/U"
			";GBB-CBS"
			";Gelijkblijvend beleid CBS"
			";0020-00"
			";6479999"
			";Omschrijving van de algmeene rekening"
			";IE-GEEN"
			";Uitgebreidere omschrijving"
			";€ 43,118.42;€ 42,675.14;€ 43,730.91;€ 45,480.15;€ 47,299.35;€ 49,191.33;;naar boven;;;"};

	MJPEntryKey expectedKey{"GBB-CBS", "0020-00", "6479999", "IE-GEEN", "U"};
	std::vector<double> expectedAmounts{42675.14, 43730.91, 45480.15, 47299.35, 49191.33};
};

TEST_F(LineFromCustomMJPFile, canParseKey)
{
	auto entry = MJPEntry::fromCustomFile(line);

	ASSERT_THAT(entry.getKey(), Eq(expectedKey));
	ASSERT_THAT(entry.getAmounts(), Eq(expectedAmounts));
}
