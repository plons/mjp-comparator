#include <gmock/gmock.h>

#include "MJPFile.hpp"
#include "MJPEntry.hpp"

#include <fstream>

using namespace testing;
using std::ifstream;



class FoxBeleidMJPExample : public Test
{
public:
	FoxBeleidMJPExample()
	{
		content << "Code;Actie;Beleidsitem code;Algemene rekening;Economische sectorcode;Investeringsenveloppe;Eerste omschrijving;2014;2015;2016;2017;2018;2019" << "\n";
		content << "BP2014_2019-1/0.1.0.0/0030-00/0902000/BESTUUR/CBS/600/IE-GEEN/U;0.1.0.0;0030-00;902000;600;IE-GEEN;Bestemde gelden voor investeringsuitgaven;€ 0.00;;;;;" << "\n";
		content << "BP2014_2019-1/0.3.0.0/0030-00/0901000/BESTUUR/CBS/600/IE-GEEN/U;0.3.0.0;0030-00;901000;600;IE-GEEN;Bestemde gelden voor exploitatie-uitgaven;€ 0.00;;;;;" << "\n";
		content << "BP2014_2019-1/1.1.1.2/0701-00/6143007/BESTUUR/CBS/600/IE-GEEN/U;1.1.1.2;0701-00;6143007;600;IE-GEEN;Publiciteit, advertenties & info;€ 800.00;€ 800.00;€ 800.00;€ 800.00;€ 800.00;€ 800.00" << "\n";
		content << "BP2014_2019-1/1.1.1.3/0119-00/6143007/BESTUUR/CBS/600/IE-GEEN/U;1.1.1.3;0119-00;6143007;600;IE-GEEN;Publiciteit, advertenties & info;€ 26,000.00;€ 33,000.00;€ 33,000.00;€ 33,000.00;€ 33,000.00;€ 33,000.00" << "\n";
		foxBeleidMJPFile.reset(new MJPFile(content, convertFoxBeleidMJPLine));
	}

	std::stringstream content;
	uint expectedNrEntries{4};
	std::unique_ptr<MJPFile> foxBeleidMJPFile;
};

TEST_F(FoxBeleidMJPExample, parsesExpectedNumberOfEntries)
{
	ASSERT_THAT(foxBeleidMJPFile->getNrEntries(), Eq(expectedNrEntries));
}

TEST_F(FoxBeleidMJPExample, canFindEntryByKey)
{
	ASSERT_TRUE(foxBeleidMJPFile->containsKey(MJPEntryKey("0.1.0.0", "0030-00", "902000", "IE-GEEN", "U")));
}

class CustomMJPExample : public Test
{
public:
	CustomMJPExample()
	{
		content << "Code;Actie;Actie oms.;BI;Beleidsitem oms.;AR;IE;Eerste omschrijving;U/O;2014;2015;2016;2017;2018;2019;;geen aanpassing;;;" << "\n";
		content << "BP2014_2019-0/GBB-CBS/0020-00/6479999/BESTUUR/CBS/100/IE-GEEN/U;GBB-CBS;Gelijkblijvend beleid CBS;0020-00;Fiscale aangelegenheden;6479999;IE-GEEN;Overige diverse operationele kosten;U;€ 43,118.42;€ 42,675.14;€ 43,730.91;€ 45,480.15;€ 47,299.35;€ 49,191.33;;naar boven;;;" << "\n";
		content << "BP2014_2019-0/GBB-CBS/0030-00/6400001/BESTUUR/CBS/100/IE-GEEN/U;GBB-CBS;Gelijkblijvend beleid CBS;0030-00;Financiële aangelegenheden;6400001;IE-GEEN;Roerende voorheffing op bankinteresten;U;€ 4,125.00;€ 4,125.00;€ 4,125.00;€ 4,125.00;€ 4,125.00;€ 4,125.00;;naar beneden;;;" << "\n";
		content << "BP2014_2019-0/GBB-CBS/0030-00/6570001/BESTUUR/CBS/700/IE-GEEN/U;GBB-CBS;Gelijkblijvend beleid CBS;0030-00;Financiële aangelegenheden;6570001;IE-GEEN;Debetintresten bankrekeningen;U;€ 100.00;€ 100.00;€ 100.00;€ 100.00;€ 100.00;€ 100.00;;te bespreken;;;" << "\n";
		content << "BP2014_2019-0/GBB-CBS/0040-00/6500000/BESTUUR/CBS/700/IE-GEEN/U;GBB-CBS;Gelijkblijvend beleid CBS;0040-00;Transacties in verband met de openbare schuld;6500000;IE-GEEN;Intresten op leningen;U;€ 344,119.60;€ 440,466.49;€ 491,708.03;€ 507,578.12;€ 532,510.77;€ 509,588.53;;;;;" << "\n";
		content << "BP2014_2019-0/GBB-CBS/0040-00/6500100/BESTUUR/CBS/700/IE-GEEN/U;GBB-CBS;Gelijkblijvend beleid CBS;0040-00;Transacties in verband met de openbare schuld;6500100;IE-GEEN;FinanciÃ«le kosten leasingschulden;U;€ 0.00;€ 0.00;€ 0.00;€ 0.00;€ 0.00;€ 0.00;;;;;" << "\n";
		customMJPFile.reset(new MJPFile(content, convertCustomMJPLine));
	}

	std::stringstream content;
	uint expectedNrEntries{5};
	std::unique_ptr<MJPFile> customMJPFile;
};

TEST_F(CustomMJPExample, parsesExpectedNumberOfEntries)
{
	ASSERT_THAT(customMJPFile->getNrEntries(), Eq(expectedNrEntries));
}

TEST_F(CustomMJPExample, canFindEntryByKey)
{
	ASSERT_TRUE(customMJPFile->containsKey(MJPEntryKey("GBB-CBS", "0040-00", "6500000", "IE-GEEN", "U")));
}

