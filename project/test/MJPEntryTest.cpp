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
			";€ 799.416,15;€ 917.523,45;€ 927.129,72;€ 953.644,22;€ 985.377,10;€ 1.007.216,94"};

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
			";U"
			";6479999"
			";Omschrijving van de algmeene rekening"
			";IE-GEEN"
			";Uitgebreidere omschrijving"
			";€ 43.118,42;€ 42.675,14;€ 43.730,91;€ 45.480,15;€ 47.299,35;€ 49.191,33;;naar boven;;;"};

	MJPEntryKey expectedKey{"GBB-CBS", "0020-00", "6479999", "IE-GEEN", "U"};
	std::vector<double> expectedAmounts{42675.14, 43730.91, 45480.15, 47299.35, 49191.33};
};

TEST_F(LineFromCustomMJPFile, canParseKey)
{
	auto entry = MJPEntry::fromCustomFile(line);

	ASSERT_THAT(entry.getKey(), Eq(expectedKey));
	ASSERT_THAT(entry.getAmounts(), Eq(expectedAmounts));
}

class LineFromFoxBeleidMJPFile2016 : public Test
{
public:
	std::string line{
		"1.1.1.2/0701-00/6143007/BESTUUR/CBS/0/IE-GEEN/U"
		";1.1.1.2"
		";0701-00"
		";6143007"
		";IE-GEEN"
		";Publiciteit, advertenties & info"
		";€ 250,00"
	};

	MJPEntryKey expectedKey{"1.1.1.2", "0701-00", "6143007", "IE-GEEN", "U"};
	std::vector<double> expectedAmounts{250};
};

TEST_F(LineFromFoxBeleidMJPFile2016, canBeParsed)
{
	auto entry = MJPEntry::fromFoxBeleidFile2016(line);

	ASSERT_THAT(entry.getKey(), Eq(expectedKey));
	ASSERT_THAT(entry.getAmounts(), Eq(expectedAmounts));
}

class LineFromCustomMJPFile2016 : public Test
{
public:
	std::string line{"1.1.1.3"                               // 0  : actie
		";Optimaliseren van het gemeentelijk infoblad"   // 1
		";0119-00"                                       // 2  : beleidsitem
		";Overige algemene diensten"                     // 3
		";U"                                             // 4  : kasStroom
		";6143007"                                       // 5  : algemene rekening
		";Publiciteit, advertenties & info"              // 6
		";IE-GEEN"                                       // 7  : investerings enveloppe
		";20000"                                         // 8
		";16001,77"                                      // 9
		";9467,52"                                       // 10
		";10532,48"                                      // 11
		";-6534,25"                                      // 12
		";-40,83 %"                                      // 13
		";"                                              // 14
		";-1500"                                         // 15
		";18500"		                                 // 16 : Amount
		";vaststelling nav rekening 2015"                // 17
		";bekijken BW2"                                  // 18
	};

	MJPEntryKey expectedKey{"1.1.1.3", "0119-00", "6143007", "IE-GEEN", "U"};
	std::vector<double> expectedAmounts{18500};
};

TEST_F(LineFromCustomMJPFile2016, canParseKey)
{
	auto entry = MJPEntry::fromCustomFile2016(line);

	ASSERT_THAT(entry.getKey(), Eq(expectedKey));
	ASSERT_THAT(entry.getAmounts(), Eq(expectedAmounts));
}
