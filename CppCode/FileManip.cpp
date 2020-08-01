#include "FileManip.h"

//Takes a file name and gives a stringstream with the content of the file as an output.
//Returns empty stringstream if it fails
std::wstringstream FileManip::DataInput (const std::filesystem::path & Path) {
 	std::wstringstream Data;
	std::wifstream File (Path);
	if (!File.is_open()) {
		std::wcerr <<L"Could not open file "<<Path <<std::endl;
		return std::wstringstream();
	}
	Data<<File.rdbuf();
	return Data;
}

//Takes a vector of strings, searches in the specified directory for all files with names on the list and
//concatenates the content of the files in order of appearance. OutName is defaulted to "ConcatenatedFile.txt"
//and will overwrite.
//If ReportList is true (default) it will create another TEXT file named OutName+_List.txt where all the names of the
//(attempted) concatenated files are listed. Files which failed will be listed too.
//If a file name is invalid that entry will be automatically skipped (See DataInput's return value).
void FileManip::JoinFiles (const std::vector<std::filesystem::path> & ListOfNames, const std::filesystem::path & PathToDirectory, std::filesystem::path OutPath, const bool ReportList) {
	std::wstringstream Content;
	for (auto & Name : ListOfNames) {
		Content << (DataInput(PathToDirectory/Name)).str();
	}
	std::wofstream Output (OutPath);
	Output << Content.str();
	if (ReportList) {
		Output.close();
		//Necessary if the specified out is a path and not just a file name.
		//This inserts "_List" in the file name, and then modifies the extension to .txt.
		OutPath.replace_filename(OutPath.stem().wstring()+L"_List.txt");
		Output.open(OutPath);
		std::wstring FullList;
		for (auto & Name : ListOfNames) {
		FullList += Name.wstring()+L'\n';
		}
		Output << FullList;
   }
   return;
}