#include "DataToJson.h"

jsoncons::wojson DataToJson (std::filesystem::path FileName) {
	jsoncons::wojson ProcessedData;
	ProcessedData.insert_or_assign(L"List",  jsoncons::json_array_arg);
	std::wstringstream DataStream = FileManip::DataInput(FileName);
	std::wstring Line;
	while (std::getline(DataStream, Line)) {
     	std::wistringstream Temp(Line);
		std::vector<double> Numbers {std::istream_iterator<double, wchar_t>{Temp}, std::istream_iterator<double, wchar_t>()}; 
		jsoncons::wojson Coordinates;
		Coordinates.insert_or_assign(L"Coordinates", jsoncons::json_array_arg);
		Coordinates[L"Coordinates"].push_back(Numbers[1]);
		Coordinates[L"Coordinates"].push_back(Numbers[2]);
		Coordinates[L"Coordinates"].push_back(Numbers[3]);
		ProcessedData[L"List"].push_back(Coordinates);
	}
	return ProcessedData;
}