#include "VirtualizerSDK.h"
#include <stdlib.h>

int compare(int x, int y) 
{
  VIRTUALIZER_START

  if (x > y) return 1;
  else if (x < y) return -1;
  else return 0;

  VIRTUALIZER_END
}

int main(int argc, char* argv[])
{
  int x = strtol(argv[1], 0, 0);
  int y = strtol(argv[2], 0, 0);

  return compare(x, y);
}