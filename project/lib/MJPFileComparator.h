#ifndef MJPFileComparator_h
#define MJPFileComparator_h

#include "MJPFile.hpp"

class MJPFileComparator {
public:
	MJPFileComparator(MJPEntry::Type type, uint32_t year, const boost::filesystem::path& foxBeleidFile, const std::vector<boost::filesystem::path>& customFiles);

	void printEntriesMissingInFoxBeleid() const;
	void printEntriesMissingInCustomFiles() const;
	void printMismatchingAmounts() const;

private:
	std::vector<MJPEntry> getEntriesMissingInFoxBeleid(const MJPFile& customFile) const;
	std::vector<MJPEntry> getEntriesMissingInCustomFiles() const;

	MJPFile foxBeleidFile;
	std::vector<MJPFile> customFiles;
};


#endif
