#ifndef EXPLORE_DIRECTORY_H
#define EXPLORE_DIRECTORY_H
#include <string>
#include <vector>
#include <filesystem>
#include <boost/filesystem.hpp> //BOOST IS NECESSARY TO USE
								//-lboost_system -lboost_filesystem to link on Ubuntu

namespace Explore {
    //Finds all the files with the specified fragment of name in the specified directory and puts their names
    //into a vector of paths. Default argument is current directory.
    std::vector<std::filesystem::path> ExploreDirectoryByName (const std::wstring & NameFragment,
                                                      const std::filesystem::path Path = std::filesystem::current_path());

    //Finds all the files with the specified extension in the specified directory and puts their names
    //into a vector of paths. Default argument is current directory.
    std::vector<std::filesystem::path> ExploreDirectoryByExtension (std::wstring Extension,
										   			  const std::filesystem::path Path = std::filesystem::current_path());

    //Finds all the files in specified directory and puts their names into a vector of paths.
    //Default argument is current directory.
    std::vector<std::filesystem::path> ExploreDirectory (const std::filesystem::path Path = std::filesystem::current_path());

    //Finds all the files in specified directory and puts their paths relative to argument into a vector of paths.
    //Default argument is current directory.
    //Each path is referred to directory given as argument.
    //If a directory is found, it repeats with the found directory.
    //Extern is true only for the first evel of recursion.
    std::vector<std::filesystem::path> ExploreDirectoryFull (const std::filesystem::path Path = std::filesystem::current_path(), const bool Extern = true);
}
#endif