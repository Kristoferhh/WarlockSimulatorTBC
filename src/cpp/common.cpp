#include "common.h"
#include <stdlib.h>

int random(int min, int max)
{
  return rand() % max + min;
}