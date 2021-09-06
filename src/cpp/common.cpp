#include "common.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>

int random(int min, int max)
{
    return min + (rand() % (max - min + 1));
}

double median(std::vector<double> vec)
{
    size_t size = vec.size();

    if (size > 0)
    {
        std::sort(vec.begin(), vec.end());
        if (size % 2 == 0)
        {
            return (vec[size / 2 - 1] + vec[size / 2]) / 2;
        }
        else 
        {
            return vec[size / 2];
        }
    }
    else
    {
        return 0;
    }
}

std::string camelCase(std::string& str)
{
    std::string newStr = str;
    newStr.erase(std::remove(newStr.begin(), newStr.end(), '\''), newStr.end());
    newStr.erase(std::remove(newStr.begin(), newStr.end(), '-'), newStr.end());
    newStr.erase(std::remove(newStr.begin(), newStr.end(), ':'), newStr.end());
    // Capitalize each word in the string
    for (int i = 1; i < newStr.length(); i++)
    {
        if (isspace(newStr[i-1]))
        {
            newStr[i] = toupper(newStr[i]);
        }
    }
    newStr.erase(std::remove(newStr.begin(), newStr.end(), ' '), newStr.end());
    newStr[0] = tolower(newStr[0]);
    
    return newStr;
}

std::string truncateTrailingZeros(std::string num, int decimalPlaces)
{
    if (decimalPlaces == 0)
    {
        return num.substr(0, num.find("."));
    }
    return num.substr(0, num.find(".") + decimalPlaces + 1);
}