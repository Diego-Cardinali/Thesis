#ifndef FILE_MANIP_H
#define FILE_MANIP_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

namespace FileManip {
	//Takes a file path and gives a stringstream with the content of the file as an output.
	//Returns empty stringstream if it fails
	extern std::wstringstream DataInput (const std::filesystem::path & Path);

	//Takes a vector of paths, searches in the specified directory for all files with names on the list and
	//concatenates the content of the files in order of appearance. OutPath is defaulted to "ConcatenatedFile.txt"
	//and will overwrite.
	//If ReportList is true (default) it will create another TEXT file named OutPath+_List.txt where all the names of the
	//(attempted) concatenated files are listed. Files which failed will be listed too.
	//If a file name is invalid that entry will be automatically skipped (See DataInput's return value).
	extern void JoinFiles (const std::vector<std::filesystem::path> & ListOfNames, const std::filesystem::path & PathToDirectory,
		std::filesystem::path OutPath = L"ConcatenatedFile.txt", const bool ReportList = true);
}
#endif