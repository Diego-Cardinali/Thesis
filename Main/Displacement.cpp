#include "Displacement.h"

void ComputeDisplacement (jsoncons::wojson & JData) {
    for (size_t K = 0; K != JData[L"Slow"].size(); ++K) {
        auto Start = JData[L"Slow"][K][L"Start"].as<std::vector<double>>();
        auto Stop = JData[L"Slow"][K][L"Stop"].as<std::vector<double>>();
        JData[L"Slow"][K].insert_or_assign(L"Displacement", Utility::Distance(Start, Stop));
    }
    for (size_t K = 0; K != JData[L"Fast"].size(); ++K) {
        auto Start = JData[L"Fast"][K][L"Start"].as<std::vector<double>>();
        auto Stop = JData[L"Fast"][K][L"Stop"].as<std::vector<double>>();
        JData[L"Fast"][K].insert_or_assign(L"Displacement", Utility::Distance(Start, Stop));
    }
    return;
}

jsoncons::wojson DisplacementOverTime (const std::array<std::vector<double>, 3> & Coordinates, const std::vector<size_t> & Lengths) {
    jsoncons::wojson Displacement;
    Displacement.insert_or_assign(L"Slow", jsoncons::json_array_arg);
    Displacement.insert_or_assign(L"Fast", jsoncons::json_array_arg);
    size_t Cumul = 0;
    for (size_t I = !static_cast<bool>(Lengths[0]); I != Lengths.size(); ++I) {
        const std::vector<double> Start = {Coordinates[0][Cumul], Coordinates[1][Cumul], Coordinates[2][Cumul]};        
        jsoncons::wojson Track;
        Track.insert_or_assign(L"Displacement", jsoncons::json_array_arg);
        Track.insert_or_assign(L"DistanceTravelled", jsoncons::json_array_arg);
        for (size_t J = 0; J != Lengths[I] ; ++J) {
            const std::vector<double> Point = {Coordinates[0][J+Cumul], Coordinates[1][J+Cumul], Coordinates[2][J+Cumul]};
            const std::vector<double> PrecPoint = {Coordinates[0][J+Cumul-1], Coordinates[1][J+Cumul-1], Coordinates[2][J+Cumul-1]};
            Track[L"Displacement"].push_back(Utility::Distance(Start, Point));
            Track[L"DistanceTravelled"].push_back(Utility::Distance(Point, PrecPoint));
        }
        Cumul += Lengths[I];
        Displacement[I%2?L"Slow":L"Fast"].push_back(Track);
    }
    return Displacement;
}

void MeanDisplacement (jsoncons::wojson & Data, const std::vector<std::wstring> & ExtKeys, const std::vector<std::wstring> & IntKeys) {
	std::vector<std::vector<size_t>> Sizes;
	Sizes.reserve(ExtKeys.size());
	//Prepare vector
	for (size_t I = 0; I != ExtKeys.size(); ++I) {
		Sizes.push_back(std::vector<size_t>());
		Sizes[I].reserve(IntKeys.size());
	}
	//Find max sizes
	for (size_t I = 0; I != ExtKeys.size(); ++I) {
		for (size_t J = 0; J != IntKeys.size(); ++J) {
			size_t MaxSize = 0;
			for (size_t K = 0; K != Data[ExtKeys[I]].size(); ++K){
				if (MaxSize < Data[ExtKeys[I]][K][IntKeys[J]].size()) {
					MaxSize = Data[ExtKeys[I]][K][IntKeys[J]].size();
				}
			}
			Sizes[I].push_back(MaxSize);
		}
	}
	//Copy data one at a time then compute means
	jsoncons::wojson Means;
	for (size_t I = 0; I != ExtKeys.size(); ++I) {
		for (size_t J = 0; J != IntKeys.size(); ++J) {
			std::wstring Name = L"Mean"+ExtKeys[I]+IntKeys[J];
			Means.insert_or_assign(Name, jsoncons::json_object_arg);
			Means[Name].insert_or_assign(L"Means", jsoncons::json_array_arg);
			Means[Name].insert_or_assign(L"NSamples", jsoncons::json_array_arg);
			Means[Name][L"Means"].reserve(Sizes[I][J]);
			Means[Name][L"NSamples"].reserve(Sizes[I][J]);
			std::vector<std::vector<double>> WorkingData;
			WorkingData.reserve(Data[ExtKeys[I]].size());
			for (size_t K = 0; K != Data[ExtKeys[I]].size(); ++K) {
				WorkingData.push_back(Data[ExtKeys[I]][K][IntKeys[J]].as<std::vector<double>>());
			}
			for (size_t K = 0; K != Sizes[I][J]; ++K) {
				double Tot = 0;
				size_t Counter = 0;
				for (size_t L = 0; L != WorkingData.size(); ) {
					if (K < WorkingData[L].size()) {
						Tot += WorkingData[L][K];
						++Counter;
						++L;
					}
					else {
						WorkingData.erase(WorkingData.begin()+L);
					}
				}
				Means[Name][L"Means"].push_back(Tot/static_cast<double>(Counter));
				Means[Name][L"NSamples"].push_back(Counter);
			}
		}
	}
	Data.insert_or_assign(L"Means", Means);
	return;
}