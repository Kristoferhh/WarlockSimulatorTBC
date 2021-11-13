#include "common.h"

#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <vector>

int Random(int min, int max) { return min + (rand() % (max - min + 1)); }

double Median(std::vector<double> vec) {
  size_t size = vec.size();

  if (size > 0) {
    std::sort(vec.begin(), vec.end());
    if (size % 2 == 0) {
      return (vec[size / 2 - 1] + vec[size / 2]) / 2;
    } else {
      return vec[size / 2];
    }
  } else {
    return 0;
  }
}

std::string TruncateTrailingZeros(std::string num, int decimal_places) {
  if (decimal_places == 0) {
    return num.substr(0, num.find("."));
  }
  return num.substr(0, num.find(".") + decimal_places + 1);
}