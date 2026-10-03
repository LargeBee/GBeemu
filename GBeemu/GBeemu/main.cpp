#include "Emulator.h"

int main()
{
	Emulator emulator;

	if (!emulator.loadROM("D:\DEV\EmulatorRoms\pokered.gb"))
	{
		return 1;
	}

	emulator.run();

	return 0;
}