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
		foxBeleidMJPFile.reset(new MJPFile(content, MJPEntry::fromFoxBeleidMJP2015));
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
		content << "Code;Actie;Actie oms.;Beleidsitem code;Algemene rekening;Algemene rekening oms.;Investeringsenveloppe;Eerste omschrijving;2014;2015;2016;2017;2018;2019;;nog aan te passen;;na te kijken;;;;;;" << "\n";
		content << "BP2014_2019-2T/1.1.1.5/0703-00/7010003/BESTUUR/CBS/IE-GEEN/O;1.1.1.5;Organiseren van jaarlijkse PC-lessen;0703-00;O;7010003;Toegangsgelden;IE-GEEN;Ontvangsten pc-lessen;€ 575,00;€ 500,00;€ 500,00;€ 500,00;€ 500,00;€ 500,00;;ok;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/1.1.1.6/0703-00/7020999/BESTUUR/CBS/IE-GEEN/O;1.1.1.6;Opvolgen van digitale ontwikkelingen en uitbouw van de digitale uitleen voor de bibliotheek;0703-00;O;7020999;Opbrengsten uit andere prestaties;IE-GEEN;Ontvangsten digitale uitleen;€ 500,00;€ 500,00;€ 100,00;€ 100,00;€ 100,00;€ 100,00;;150 euro is realistischer;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/2.2.2.7/0750-00/7406999/BESTUUR/CBS/IE-GEEN/O;2.2.2.7;Organiseren van kampen tijdens de vakantieperiodes;0750-00;O;7406999;Overige specifieke projectsubsidies;IE-GEEN;subsidie week van de opvoeding;€ 1 000,00;€ 1 000,00;€ 1 000,00;€ 1 000,00;€ 1 000,00;€ 1 000,00;ok;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/3.1.3.1/0629-00/7406999/BESTUUR/CBS/IE-GEEN/O;3.1.3.1;Onderzoeken van de mogelijkheden inzake nieuwe samenwoonvormen;0629-00;O;7406999;Overige specifieke projectsubsidies;IE-GEEN;Overige specifieke projectsubsidies;€ 20 000,00;;;;;;;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/3.4.2.5/0160-00/7404999/BESTUUR/CBS/IE-GEEN/O;3.4.2.5;Ontvangen van een sectorale subsidie inzake gemeentelijke ontwikkelingssamenwerking;0160-00;O;7404999;Overige algemene werkingssubsidies;IE-GEEN;Subsidie Vlaanderen Internat.gemeentelijke samenwerking;€ 48 000,00;€ 33 000,00;€ 0,00;€ 0,00;€ 0,00;€ 0,00;;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/3.5.1.4/0500-00/7020999/BESTUUR/CBS/IE-GEEN/O;3.5.1.4;Ondersteunen van netwerk- en handelskernversterkende initiatieven voor de Bierbeekse ondernemers;0500-00;O;7020999;Opbrengsten uit andere prestaties;IE-GEEN;Opbrengsten uit andere prestaties;;€ 1 400,00;€ 2 025,00;€ 2 400,00;€ 2 650,00;€ 2 900,00;???;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/3.5.1.4/0500-00/7406999/BESTUUR/CBS/IE-GEEN/O;3.5.1.4;Ondersteunen van netwerk- en handelskernversterkende initiatieven voor de Bierbeekse ondernemers;0500-00;O;7406999;Overige specifieke projectsubsidies;IE-GEEN;Overige specifieke projectsubsidies;€ 6 250,00;€ 3 034,46;€ 9 284,46;;;;;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/4.3.2.1/0729-00/7010003/BESTUUR/CBS/IE-GEEN/O;4.3.2.1;Stimuleren en ondersteunen van erfgoedparticipatie;0729-00;O;7010003;Toegangsgelden;IE-GEEN;Toegangsgelden;€ 1 425,00;;;;;;;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/GBB-CBS/0010-00/7400000/BESTUUR/CBS/IE-GEEN/O;GBB-CBS;Gelijkblijvend beleid CBS;0010-00;O;7400000;Gemeentefonds;IE-GEEN;Gemeentefonds;€ 1 235 762,40;€ 1 304 696,80;€ 1 642 444,53;€ 1 689 110,13;€ 1 737 408,53;€ 1 787 397,33;;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/GBB-CBS/0010-00/7404999/BESTUUR/CBS/IE-GEEN/O;GBB-CBS;Gelijkblijvend beleid CBS;0010-00;O;7404999;Overige algemene werkingssubsidies;IE-GEEN;Overige algemene werkingssubsidies;€ 117 207,67;€ 117 207,67;€ 117 207,67;€ 117 207,67;€ 117 207,67;€ 117 207,67;ok;;;;;;;;;" << "\n";
		content << "BP2014_2019-2T/GBR-VBJ/0010-00/7404999/BESTUUR/CBS/IE-GEEN/O;GBR-VBJ;Gelijkblijvend beleid CBS;0010-00;O;7404999;Overige algemene werkingssubsidies;IE-GEEN;Overige algemene werkingssubsidies;€ 117 207,67;€ 117 207,67;€ 117 207,67;€ 117 207,67;€ 117 207,67;€ 117 207,67;ok;;;;;;;;;" << "\n";
		customMJPFile.reset(new MJPFile(content, MJPEntry::fromCustomFileMJP2015));
	}

	std::stringstream content;
	uint expectedNrEntries{10};
	std::unique_ptr<MJPFile> customMJPFile;
};

TEST_F(CustomMJPExample, ignoresCumulatedResultOfPreviousBookingYear)
{
	// action GBR-VBJ => cumulated result of previous booking year => should be ignoreld!
	ASSERT_FALSE(customMJPFile->containsKey(MJPEntryKey("GBR-VBJ", "0010-00", "7404999", "IE-GEEN", "O")));
}

TEST_F(CustomMJPExample, parsesExpectedNumberOfEntries)
{
	ASSERT_THAT(customMJPFile->getNrEntries(), Eq(expectedNrEntries));
}

TEST_F(CustomMJPExample, canFindEntryByKey)
{
	ASSERT_TRUE(customMJPFile->containsKey(MJPEntryKey("GBB-CBS", "0010-00", "7400000", "IE-GEEN", "O")));
}
