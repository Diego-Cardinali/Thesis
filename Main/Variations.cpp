#include "Variations.h"

std::array<std::vector<double>, 3> Variations (const std::array<std::vector<double>, 3>& Data) {
	std::array<std::vector<double>, 3> ProcessedData;
	ProcessedData[0].reserve(Data[0].size()-1);
	ProcessedData[1].reserve(Data[1].size()-1);
	ProcessedData[2].reserve(Data[2].size()-1);
	for (unsigned long int I = 0ul; I != Data[0].size()-1; ++I){
		ProcessedData[0].push_back((Data[0][I+1]-Data[0][I]));
		ProcessedData[1].push_back((Data[1][I+1]-Data[1][I]));
		ProcessedData[2].push_back((Data[2][I+1]-Data[2][I]));
	}
	return ProcessedData;
}

std::array<std::vector<double>, 3> VariationsOnDeltaT (const std::array<std::vector<double>, 3>& Data, const double DeltaT) {
	std::array<std::vector<double>, 3> ProcessedData;
	ProcessedData[0].reserve(Data[0].size()-1);
	ProcessedData[1].reserve(Data[1].size()-1);
	ProcessedData[2].reserve(Data[2].size()-1);
	for (unsigned long int I = 0ul; I != Data[0].size()-1; ++I){
		ProcessedData[0].push_back((Data[0][I+1]-Data[0][I])/DeltaT);
		ProcessedData[1].push_back((Data[1][I+1]-Data[1][I])/DeltaT);
		ProcessedData[2].push_back((Data[2][I+1]-Data[2][I])/DeltaT);
	}
	return ProcessedData;
}

std::array<std::vector<double>, 3> VariationsStepN (const std::array<std::vector<double>, 3>& Data, const double DeltaT, const size_t Step) {
	std::array<std::vector<double>, 3> ProcessedData;
	ProcessedData[0].reserve(Data[0].size()-1);
	ProcessedData[1].reserve(Data[1].size()-1);
	ProcessedData[2].reserve(Data[2].size()-1);
	for (unsigned long int I = 0ul; I < Data[0].size()-Step; I+=Step){
		ProcessedData[0].push_back((Data[0][I+Step]-Data[0][I])/DeltaT);
		ProcessedData[1].push_back((Data[1][I+Step]-Data[1][I])/DeltaT);
		ProcessedData[2].push_back((Data[2][I+Step]-Data[2][I])/DeltaT);
	}
	return ProcessedData;
}
