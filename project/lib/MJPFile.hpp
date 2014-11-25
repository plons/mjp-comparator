#ifndef MJPFile_h
#define MJPFile_h

#include "MJPEntry.hpp"

#include <boost/filesystem/path.hpp>
#include <map>
#include <memory>

auto convertFoxBeleidMJPLine = [](const std::string& line){return std::make_shared<FoxBeleidMJPEntry>(line);};
auto convertCustomMJPLine    = [](const std::string& line){return std::make_shared<CustomMJPEntry>(line);};

/*
 * KEY:
 *  Actie (vb 0.3.0.0 of GBB-CBS),
 *  BeleidsItem (vb 0030-00)
 *  Algemene rekening (vb 6479999)
 *  Investerings Enveloppe (vb IE-GEEN of IE1, IE2, ...)
 *
 *
 * Economische sectorcode kan verschillen => optellen!
 *
 * CustomMJPExample => Eerste colomn niet gebruiken!!! (is niet altijd geupdated!!!)
 */
class MJPFile
{
public:
	using ConvertLineFunction = std::function<std::shared_ptr<MJPEntry> (const std::string&)>;

	MJPFile(const boost::filesystem::path& file, const ConvertLineFunction& convert);

	const boost::filesystem::path& getPath() const {return file;}
	uint getNrEntries() const;
	std::vector<MJPEntry> getAllEntries() const;
	bool containsKey(const MJPEntryKey& key) const;
	const MJPEntry& getEntry(const MJPEntryKey& key) const;

	void printOn(std::ostream& ws) const;

//testing:
	MJPFile(std::istream& input, const ConvertLineFunction& convert);

private:
	void init(std::istream& input, const ConvertLineFunction& convert);

	boost::filesystem::path file;
	std::map<MJPEntryKey, std::shared_ptr<MJPEntry>> entries;
};

std::ostream& operator<<(std::ostream& ws, const MJPFile& file);

#endif
