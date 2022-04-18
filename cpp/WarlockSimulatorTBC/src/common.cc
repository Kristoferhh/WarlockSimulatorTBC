#include "../include/common.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

double Median(std::vector<double> vec) {

  if (const size_t kSize = vec.size(); kSize > 0) {
    std::sort(vec.begin(), vec.end());

    if (kSize % 2 == 0) {
      return (vec[kSize / 2 - 1] + vec[kSize / 2]) / 2;
    }

    return vec[kSize / 2];
  }

  return 0;
}

std::string DoubleToString(const double kNum, const int kDecimalPlaces) {
  auto str = std::to_string(kNum);
  return str.substr(0, str.find('.') + (kDecimalPlaces > 0 ? kDecimalPlaces + 1 : 0));
}