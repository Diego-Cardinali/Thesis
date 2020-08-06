#ifndef DATATOJSON_H
#define DATATOJSON_H
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include <iterator>

#include <jsoncons/json.hpp>

#include "./../Libraries/FileManip.h"

/**************************************************************
|		This code converts specifically formatted .txt        |
|		documents to .json. It was hardcoded for The          |
|		type of files I need it to convert.					  |
**************************************************************/

extern jsoncons::wojson DataToJson (std::filesystem::path);

#endif