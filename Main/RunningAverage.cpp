#include "RunningAverage.h"

//Computes the running average of each element in the vector, taking Radius element
//BEFORE and Radius element after.
std::vector<double> RunningAverage (const std::vector<double> & Data, const size_t Radius) {
	//Minimum working condition
	if (2.*Radius+1>Data.size()) {
		return std::vector<double> ();
	}
	std::vector <double> Averages;
	Averages.reserve(Data.size()-2*Radius);
	//Starting from the first point where it is possible
	for (size_t I = Radius; I != Data.size()-Radius; ++I) {
		std::vector<double>::const_iterator First = Data.begin()+I-Radius;
		std::vector<double>::const_iterator Last = Data.begin()+I+Radius;
		Averages.push_back(Utility::Mean(std::vector<double>(First, Last)));
	}
	return Averages;
}