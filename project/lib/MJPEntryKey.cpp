#include "MJPEntryKey.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>

#include <stdexcept>

using boost::algorithm::starts_with;
using boost::algorithm::trim_copy;
using boost::algorithm::to_upper_copy;
using boost::format;
using boost::regex_match;
using boost::regex;

MJPEntryKey::MJPEntryKey(const std::string& actie, const std::string& beleidsItem, const std::string& algemeneRekening, const std::string& investeringsEnveloppe, const std::string& kasStroom) :
		actie(to_upper_copy(trim_copy(actie))),
		beleidsItem(trim_copy(beleidsItem)),
		algemeneRekening(trim_copy(algemeneRekening)),
		investeringsEnveloppe(trim_copy(investeringsEnveloppe)),
		kasStroom(trim_copy(kasStroom))
{
	checkValues();
}

bool MJPEntryKey::operator==(const MJPEntryKey& other) const
{
	return actie == other.actie
		&& beleidsItem == other.beleidsItem
		&& algemeneRekening == other.algemeneRekening
		&& investeringsEnveloppe == other.investeringsEnveloppe
		&& kasStroom == other.kasStroom;
}

bool MJPEntryKey::operator<(const MJPEntryKey& other) const
{
	if (actie != other.actie) return actie < other.actie;
	if (beleidsItem != other.beleidsItem) return beleidsItem < other.beleidsItem;
	if (algemeneRekening != other.algemeneRekening) return algemeneRekening < other.algemeneRekening;
	if (investeringsEnveloppe != other.investeringsEnveloppe) return investeringsEnveloppe < other.investeringsEnveloppe;
	if (kasStroom != other.kasStroom) return kasStroom < other.kasStroom;
	return false;
}

void MJPEntryKey::checkValues() const
{
	if (!regex_match(actie, regex("\\d+\\.\\d+\\.\\d+\\.\\d+"))
			&& actie != "GBB-CBS"
			&& actie != "GBR-VBJ")
	{
		throw std::invalid_argument((format("Het opgegeven veld '%s' komt niet overeen met de verwachte waarde voor een actie.")%actie).str());
	}
	if (!regex_match(beleidsItem, regex("\\d{4}-\\d{2}")))
	{
		throw std::invalid_argument((format("Het opgegeven veld '%s' komt niet overeen met de verwachte waarde voor een beleidsitem.")%beleidsItem).str());
	}
	if (!regex_match(algemeneRekening, regex("\\d+")))
	{
		throw std::invalid_argument((format("Het opgegeven veld '%s' komt niet overeen met de verwachte waarde voor een algemene rekening.")%algemeneRekening).str());
	}
	if (!starts_with(investeringsEnveloppe, "IE"))
	{
		throw std::invalid_argument((format("Het opgegeven veld '%s' komt niet overeen met de verwachte waarde voor een investerings enveloppe.")%investeringsEnveloppe).str());
	}
	if (kasStroom != "U" && kasStroom != "O")
	{
		throw std::invalid_argument((format("Het opgegeven veld '%s' komt niet overeen met de verwachte waarde voor een kasstroom.")%kasStroom).str());
	}
//	std::string actie;                 // (vb 0.3.0.0 of GBB-CBS),
//	std::string beleidsItem;           // (vb 0030-00)
//	std::string algemeneRekening;      // (vb 6479999)
//	std::string investeringsEnveloppe; // (vb IE-GEEN of IE1, IE2, ...)
}

std::ostream& operator<<(std::ostream& ws, const MJPEntryKey& key)
{
	ws << "actie=[" << key.actie << "]"
		<< " beleidsItem=[" << key.beleidsItem << "]"
		<< " algemeneRekening=[" << key.algemeneRekening << "]"
		<< " investeringsEnveloppe=[" << key.investeringsEnveloppe << "]"
		<< " kasStroom=[" << key.kasStroom << "]";

	return ws;
}
