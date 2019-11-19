#pragma once

class GameCore;

class Framework
{
public:
	Framework();

	void Init(int width, int height, HINSTANCE hInstance);
	int Run(GameCore* pGameCore);
	void Shutdown();

	void SetWindowSize(int width, int height);
	void ResizeWindow(int width, int height);
	bool IsKeyDown(int value);
	bool WasKeyPressed(int value);
	bool IsMouseButtonDown(int id);
	void GetMouseCoordinates(int* mx, int* my);
	void SetMousePosition(ivec2 pos);

	unsigned int GetWindowWidth() { return m_CurrentWindowWidth; }
	unsigned int GetWindowHeight() { return m_CurrentWindowHeight; }

	GameCore* GetGameCore() { return m_GameCore; }

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static Framework* SlitherFramework;
	
protected:
	GameCore* m_GameCore = nullptr;

	bool m_UseEscapeToQuit = true;
	bool m_CloseProgramRequested;

	int m_InitialWindowWidth;
	int m_InitialWindowHeight;

	int m_CurrentWindowWidth;
	int m_CurrentWindowHeight;

	HWND m_hWnd;
	HGLRC m_hRenderingContext;
	HDC m_hDeviceContext;
	HINSTANCE m_hInstance;
	WPARAM m_wParam;
	LPARAM m_lParam;

	bool m_KeyStates[256];
	bool m_OldKeyStates[256];
	bool m_MouseButtonStates[3];
	bool m_WindowIsActive;
	bool m_FullscreenMode;

	friend GameCore;

	bool HandlerRoutine(DWORD dwCtrlType);
};

