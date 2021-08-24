#pragma once

#include <map>
#include <iostream>

struct PlayerSettings
{
  std::map<std::string, bool> auras;
  std::map<std::string, bool> talents;
  std::map<std::string, int> sets;
};