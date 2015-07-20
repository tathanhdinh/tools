#include <stdlib.h>

int if_struct_origin(int input)
{
  int x = input;

  if (x == 0) x += 1;
  else if (x == 1) x += 3;
  else if (x == 2) x -= 1;
  else if (x == 3) x /= 2;
  else if (x == 4) x -= 2;
  else if (x == 5) x = 10;
  else if (x == 6) x = 17;
  else x = 10;

  return input + x;
}

int main(int argc, char* argv[])
{
  int input = strtoul(argv[1], 0, 0);
  return if_struct_origin(input);
}
