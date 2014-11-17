#ifndef MJPEntryKey_h
#define MJPEntryKey_h

#include <boost/operators.hpp>

class MJPEntryKey :
		public boost::equality_comparable<MJPEntryKey>
{
public:
	MJPEntryKey(const std::string& actie, const std::string& beleidsItem, const std::string& algemeneRekening, const std::string& investeringsEnveloppe, const std::string& kasStroom);

	bool operator==(const MJPEntryKey& other) const;

	std::string actie;                 // (vb 0.3.0.0 of GBB-CBS),
	std::string beleidsItem;           // (vb 0030-00)
	std::string algemeneRekening;      // (vb 6479999)
	std::string investeringsEnveloppe; // (vb IE-GEEN of IE1, IE2, ...)
	std::string kasStroom;             // uitgave / ontvangst

private:
	void checkValues() const;
};

std::ostream& operator<<(std::ostream& ws, const MJPEntryKey& key);

#endif
