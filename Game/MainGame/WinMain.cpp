#include "GamePCH.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SlitherFramework = new Framework();
	GameCore* Game = new GameCore(SlitherFramework);
	SlitherFramework->Init(0, 0, hInstance);

	SlitherFramework->Run(Game);

	SlitherFramework->Shutdown();

	SafeDelete(Game);
	SafeDelete(SlitherFramework);

	return 0;
}

