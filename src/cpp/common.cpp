#include "common.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>

int random(int min, int max)
{
    return rand() % max + min;
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
    newStr.erase(std::remove(newStr.begin(), newStr.end(), ' '), newStr.end());
    newStr[0] = tolower(newStr[0]);
    
    return newStr;
}