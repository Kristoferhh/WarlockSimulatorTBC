#include "common.h"

#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

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

std::string DoubleToString(double num, int decimal_places) {
  std::string returnStr = std::to_string(num);

  if (decimal_places > 0) {
    return returnStr.substr(0, returnStr.find(".") + decimal_places + 1);
  } else {
    returnStr.erase(returnStr.find_last_not_of('0') + 1, std::string::npos);
    returnStr.erase(returnStr.find_last_not_of('.') + 1, std::string::npos);
    return returnStr;
  }
}