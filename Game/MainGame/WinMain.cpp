#include "GamePCH.h"
#include "MainGame.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Framework::SlitherFramework = new Framework();
	GameCore* Game = new MainGame(Framework::SlitherFramework);
	Framework::SlitherFramework->Init(0, 0, hInstance);

	Framework::SlitherFramework->Run(Game);

	Framework::SlitherFramework->Shutdown();

	SafeDelete(Game);
	SafeDelete(Framework::SlitherFramework);

	return 0;
}

