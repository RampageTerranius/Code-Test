#include "Randomize.h"
#include <stdlib.h>

Randomize::Randomize()
{
	two = three = oneHundred = 0;
}


void Randomize::Random()
{
	two = rand() % 2;
	three = rand() % 3;
	oneHundred = rand() % 100;
}