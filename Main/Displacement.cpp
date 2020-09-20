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