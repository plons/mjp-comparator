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
		content << "Code;Actie;Beleidsitem code;Algemene rekening;Investeringsenveloppe;Eerste omschrijving;2014;2015;2016;2017;2018;2019" << "\n";
		content << "BP2014_2019-2/1.1.1.2/0701-00/6143007/BESTUUR/CBS/IE-GEEN/U;1.1.1.2;0701-00;6143007;IE-GEEN;Publiciteit, advertenties & info;€ 643,54;€ 800,00;€ 800,00;€ 800,00;€ 800,00;€ 800,00" << "\n";
		content << "BP2014_2019-2/1.1.1.3/0119-00/6143007/BESTUUR/CBS/IE-GEEN/U;1.1.1.3;0119-00;6143007;IE-GEEN;Publiciteit, advertenties & info;€ 27.283,98;€ 25.000,00;€ 20.000,00;€ 20.000,00;€ 20.000,00;€ 20.000,00" << "\n";
		content << "BP2014_2019-2/1.1.1.4/0119-00/6131002/BESTUUR/CBS/IE-GEEN/U;1.1.1.4;0119-00;6131002;IE-GEEN;Erelonen en vergoedingen consultancy;;€ 10.000,00;;;;" << "\n";
		content << "BP2014_2019-2/1.1.1.5/0703-00/6159999/BESTUUR/CBS/IE-GEEN/U;1.1.1.5;0703-00;6159999;IE-GEEN;Overige exploitatiekosten;€ 1.210,00;€ 1.500,00;€ 1.500,00;€ 1.500,00;€ 1.500,00;€ 1.500,00" << "\n";
		content << "BP2014_2019-2/1.1.1.5/0703-00/7010003/BESTUUR/CBS/IE-GEEN/O;1.1.1.5;0703-00;7010003;IE-GEEN;Toegangsgelden;€ 575,00;€ 500,00;€ 500,00;€ 500,00;€ 500,00;€ 500,00" << "\n";
		content << "BP2014_2019-2/1.1.1.6/0703-00/6001000/BESTUUR/CBS/IE-GEEN/U;1.1.1.6;0703-00;6001000;IE-GEEN;Aankoop technisch materiaal;€ 490,00;€ 1.500,00;€ 1.500,00;€ 1.500,00;€ 1.500,00;€ 1.500,00" << "\n";
		content << "BP2014_2019-2/1.1.1.6/0703-00/7020999/BESTUUR/CBS/IE-GEEN/O;1.1.1.6;0703-00;7020999;IE-GEEN;Opbrengsten uit andere prestaties;€ 100,00;€ 500,00;€ 100,00;€ 100,00;€ 100,00;€ 100,00" << "\n";
		content << "BP2014_2019-2/1.1.2.1/0160-00/6493000/BESTUUR/CBS/IE-GEEN/U;1.1.2.1;0160-00;6493000;IE-GEEN;Toegestane werkingssubsidies aan verenigingen;€ 500,00;€ 500,00;€ 500,00;€ 500,00;€ 500,00;€ 500,00" << "\n";
		content << "BP2014_2019-2/1.1.2.1/0390-00/6493000/BESTUUR/CBS/IE-GEEN/U;1.1.2.1;0390-00;6493000;IE-GEEN;Toegestane werkingssubsidies aan verenigingen;€ 250,00;€ 250,00;€ 250,00;€ 250,00;€ 250,00;€ 250,00" << "\n";
		content << "BP2014_2019-2/1.1.2.1/0500-00/6493000/BESTUUR/CBS/IE-GEEN/U;1.1.2.1;0500-00;6493000;IE-GEEN;Subsidie middenstandsraad;;€ 250,00;€ 250,00;€ 250,00;€ 250,00;€ 250,00" << "\n";
		foxBeleidMJPFile.reset(new MJPFile(content, MJPEntry::fromFoxBeleidFile));
	}

	std::stringstream content;
	uint expectedNrEntries{10};
	std::unique_ptr<MJPFile> foxBeleidMJPFile;
};

TEST_F(FoxBeleidMJPExample, parsesExpectedNumberOfEntries)
{
	ASSERT_THAT(foxBeleidMJPFile->getNrEntries(), Eq(expectedNrEntries));
}

TEST_F(FoxBeleidMJPExample, canFindEntryByKey)
{
	ASSERT_TRUE(foxBeleidMJPFile->containsKey(MJPEntryKey("1.1.1.4", "0119-00", "6131002", "IE-GEEN", "U")));
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
		customMJPFile.reset(new MJPFile(content, MJPEntry::fromCustomFile));
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

