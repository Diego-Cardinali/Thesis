#include "Utility.h"

//Returns true if 'Y' or 'y', false if 'N' or 'n', otherwise the loop repeats.
bool Utility::CheckYN (const bool Verbose) {
	std::wstring S;
	while (true) {
		std::wcin >> S;
		if (!S.compare(L"Y") or !S.compare(L"y")) {return true;}
		if (!S.compare(L"N") or !S.compare(L"n")) {return false;}
		else {
            if (Verbose) {
                std::wcout << L"Invalid input, please retry "<<std::flush;
            }
			std::wcin.clear();
			std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), L'\n');
		}
	}
}
//Computes the module of a set of n coordinates, treated as a vector.
double Utility::Module (const std::vector <double> & Vector) {
	double Module = 0.;
	for (auto & U : Vector) {
		Module += U*U;
	}
	return std::sqrt(Module);
}
//Computes the n-dimensional Euclidean distance of 2 sets of coordinates.
//If the vectors are not compatible (equal dimension) the value is -1.
double Utility::Distance (const std::vector <double> & Vector1, const std::vector <double> & Vector2) {
	if (Vector1.size() != Vector1.size()) {
		return -1.;
	}
	double Distance = 0.;
	for (long unsigned int I = 0ul; I != Vector1.size(); ++I) {
		Distance += (Vector1[I]-Vector2[I])*(Vector1[I]-Vector2[I]); 
	}
	return std::sqrt(Distance);
}

//Computes the unweighted mean of the elements of a vector.
double Utility::NormalMean (const std::vector<double> & Vector) {
	double Sum = 0.;
	for (auto & U : Vector) {
		Sum += U;
	}
	return (Sum/(double)Vector.size());

}