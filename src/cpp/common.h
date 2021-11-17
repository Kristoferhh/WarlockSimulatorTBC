#ifndef WARLOCK_SIMULATOR_TBC_COMMON
#define WARLOCK_SIMULATOR_TBC_COMMON

#include <string>
#include <vector>

int Random(int min, int max);
double Median(std::vector<double> vec);
std::string DoubleToString(double num, int decimal_places = 0);

#endif