#ifndef TRACKLENGTH_H
#define TRACKLENGTH_H

#include <iterator>

//Container referred to by Out is filled with size_t representing the number of consecutive
//elements of the container referred to by Begin-End which are respectively higher and
//lower of Threshold. All even positions in Out are segments for which the data is greater
//or equal to Threshold, if the data starts with a portion that is lower the first element
//will be a 0. The return value is the size of Out.
//The state (high or low) is changed only if a discrepancy is found long at least Discontinuity
template<typename InIt, typename OutIt, typename T> 
size_t TrackLength( InIt Begin, InIt End, OutIt Out, T Threshold, size_t Discontinuity = 0) {
    size_t CntIns {};
    size_t N {};
    size_t D {};
    if (Begin != End) {
        if (*Begin < Threshold) {
            //If starting data is Low
            *Out++ = {};
            ++CntIns;
        }
        else {
            ++Begin;
            ++N;
        }
        while (Begin != End) {
            if (CntIns % 2) {
                // Low
                if (*Begin < Threshold) {
                    ++N;
                    D = 0;
                }
                else {
                    if (D < Discontinuity) {
                        ++D;
                        ++N;
                    }
                    else {
                        *Out++ = N-D;
                        ++CntIns;
                        N = D+1;
                        D = 0;
                    }
                }
            }
            else {
                // High
                if (*Begin >= Threshold) {
                    ++N;
                    D = 0;
                }
                else {
                    if (D < Discontinuity) {
                        ++D;
                        ++N;
                    }
                    else {
                        *Out++ = N-D;
                        ++CntIns;
                        N = D+1;
                        D = 0;
                    }
                }
            }
            ++Begin;
        }
        *Out++ = N;
    }
    return CntIns;
}

#endif