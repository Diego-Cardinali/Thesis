#include "ExploreDirectory.h"

//Finds all the files with the specified fragment of name in the specified directory and puts their names
//into a vector of paths. Default argument is current directory.
std::vector<std::filesystem::path> Explore::ExploreDirectoryByName (const std::wstring & NameFragment, const std::filesystem::path Path) {
	std::vector<std::filesystem::path> NamesList;
    for (const auto & Entry : std::filesystem::directory_iterator(Path)) {
    	if (Entry.path().filename().wstring().find(NameFragment) != std::string::npos) {//Looks for name match, no matter where.
    		NamesList.push_back(Entry.path().filename());
    	}
    }
	return NamesList;
}

//Finds all the files with the specified extension in the specified directory and puts their names
//into a vector of paths. Default argument is current directory.
std::vector<std::filesystem::path> Explore::ExploreDirectoryByExtension (std::wstring Extension, const std::filesystem::path Path) {
	std::vector<std::filesystem::path> NamesList;
	if (Extension.find('.') == std::string::npos) { //Can accept both ".extension" and "extension".
		Extension = L"."+Extension;
	}
    for (const auto & Entry : std::filesystem::directory_iterator(Path)) {
    	if (!Entry.path().extension().wstring().compare(Extension)) {
    		NamesList.push_back(Entry.path().filename());
    	}
    }
	return NamesList;
}

//Finds all the files in specified directory and puts their names into a vector of paths.
//Default argument is current directory.
std::vector<std::filesystem::path> Explore::ExploreDirectory (const std::filesystem::path Path) {
	std::vector<std::filesystem::path> NamesList;
    for (const auto & Entry : std::filesystem::directory_iterator(Path)) {
    	NamesList.push_back(Entry.path().filename());
    }
	return NamesList;
}

//Finds all the files in specified directory and puts their paths relative to argument into a vector of paths.
//Default argument is current directory.
//Each path is referred to directory given as argument.
//If a directory is found, it repeats with the found directory.
//Extern is true only for the first evel of recursion.
std::vector<std::filesystem::path> Explore::ExploreDirectoryFull (const std::filesystem::path Path, const bool Extern) {
    std::vector<std::filesystem::path> NamesList;
    for (const auto & Entry : std::filesystem::directory_iterator(Path)) {
        if (Entry.path().has_extension()) {
            NamesList.push_back(Entry.path());
        }
        else {
            //Recursion if a directory is found
            std::vector<std::filesystem::path> TempList;
            TempList = ExploreDirectoryFull(Entry.path(), false);
            NamesList.insert(NamesList.end(), TempList.begin(), TempList.end());
        }
    }
    if (Extern) {
        //If Extern is true, i.e. first level of recursion, the paths are made relative to Path
        //Boost path and std path need to be constructed from wstrings
        for (auto & Name : NamesList) {
            Name = (boost::filesystem::relative(boost::filesystem::path(Name.wstring()), boost::filesystem::path(Path.wstring()))).wstring();
        }
    }
    return NamesList;
}
