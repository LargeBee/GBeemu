#include "Emulator.h"

int main()
{
	Emulator emulator;

	if (!emulator.loadROM("pokemon.gb"))
	{
		return 1;
	}

	emulator.run();

	return 0;
}