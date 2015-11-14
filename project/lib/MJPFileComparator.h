#ifndef MJPFileComparator_h
#define MJPFileComparator_h

#include "MJPFile.hpp"

class MJPFileComparator {
public:
	MJPFileComparator(const boost::filesystem::path& foxBeleidFile, const std::vector<boost::filesystem::path>& customFiles);

	void printEntriesMissingInFoxBeleid() const;
	void printEntriesMissingInCustomFiles() const;

	std::vector<MJPEntry> getEntriesMissingInCustomFiles() const;

	void printMismatchingAmounts() const;

private:
	std::vector<MJPEntry> getEntriesMissingInFoxBeleid(const MJPFile& customFile) const;

	MJPFile foxBeleidFile;
	std::vector<MJPFile> customFiles;
};


#endif
