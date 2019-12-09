#include "SlitherEnginePCH.h"

LRESULT CALLBACK
MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return Framework::SlitherFramework->WndProc(hwnd, msg, wParam, lParam);
}

Framework::Framework()
{
	m_GameCore = nullptr;

	m_UseEscapeToQuit = USE_ESC_TO_QUIT;
	m_CloseProgramRequested = false;

	m_InitialWindowWidth = -1;
	m_InitialWindowHeight = -1;

	m_CurrentWindowWidth = -1;
	m_CurrentWindowHeight = -1;

	m_hWnd = nullptr;
	m_hRenderingContext = nullptr;
	m_hDeviceContext = nullptr;
	m_hInstance = nullptr;

	// Horrid key handling.
	for (int i = 0; i < 256; i++)
	{
		m_KeyStates[i] = false;
		m_OldKeyStates[i] = false;
	}

	for (int i = 0; i < 3; i++)
		m_MouseButtonStates[i] = false;

	m_WindowIsActive = false;
	m_FullscreenMode = false;
}

void Framework::Init(int width, int height, HINSTANCE hInstance)
{
	OutputMessage("Framework : Init \n");

	(width > WINDOW_MIN_WIDTH) ? m_InitialWindowWidth = width : m_InitialWindowWidth = WINDOW_WIDTH;
	(height > WINDOW_MIN_HEIGHT) ? m_InitialWindowHeight = height : m_InitialWindowHeight = WINDOW_HEIGHT;

	m_CurrentWindowWidth = m_InitialWindowWidth;
	m_CurrentWindowHeight = m_InitialWindowHeight;

	m_WindowIsActive = true;

	/*RenderingSubSystem* directXSystem = 
		GetGameCore()->GetSubSystemManager()->CreateSubSystem<RenderingSubSystem>("DirectX", SubSystemID::DirectXSystem);*/

	TimerSubSystem* timerSystem =
		GetGameCore()->GetSubSystemManager()->CreateSubSystem<TimerSubSystem>("TimerSystem", SubSystemID::Timers);	

	WorldsSubSystem* worldSystem =
		GetGameCore()->GetSubSystemManager()->CreateSubSystem<WorldsSubSystem>("WorldSystem", SubSystemID::World);

	/*if (directXSystem)
	{
		directXSystem->SetCallBack(Framework::WndProc);
		directXSystem->SetAppInst(hInstance);
		directXSystem->Init();
	}	*/

	VulkanSubsystem* vulkanSystem =
		GetGameCore()->GetSubSystemManager()->CreateSubSystem<VulkanSubsystem>("VulkanSystem", SubSystemID::VulkanSystem);

	if (vulkanSystem)
	{
		vulkanSystem->Init();
	}
}

int Framework::Run(GameCore* pGameCore)
{
	m_GameCore = pGameCore;
	m_GameCore->OnSurfaceChanged(m_CurrentWindowWidth, m_CurrentWindowHeight);
	m_GameCore->LoadContent();

	m_GameTimer.Reset();

	double lastTime = GetSystemTime();

	// Our game loop.
	MSG msg;
	bool done = false;

	// Keep referances of subsystems here
	TimerSubSystem* timerSystem = 
		reinterpret_cast<TimerSubSystem*>(m_GameCore->GetSubSystemManager()->GetSubSystemByType(Timers));
	EventHandlerSubSystem* eventHandler = 
		reinterpret_cast<EventHandlerSubSystem*>(m_GameCore->GetSubSystemManager()->GetSubSystemByType(EventSystem));
	WorldsSubSystem* worldSystem =
		reinterpret_cast<WorldsSubSystem*>(m_GameCore->GetSubSystemManager()->GetSubSystemByType(SubSystemID::World));
	VulkanSubsystem* vulkanSystem =
		reinterpret_cast<VulkanSubsystem*>(m_GameCore->GetSubSystemManager()->GetSubSystemByType(SubSystemID::VulkanSystem));
	// ---------------------------------

	if (CALL_OBJECT_CLEANUP_BY_TIMER)
	{/*
		TimerHandle* objCleanupTimer = new TimerHandle(10.0f, TimerPriority::Low,
			"Object Cleaner", true, worldSystem, &WorldsSubSystem::CleanupWorlds);

		timerSystem->MakeNewTimer(objCleanupTimer,true);*/

	}

	OutputMessage("Framework : Starting Gameloop \n");
	while (!done)
	{
		GLFWwindow* window = vulkanSystem->GetWindow();
		if (!glfwWindowShouldClose(window))
		{
			vulkanSystem->Draw();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT || msg.message == WM_CLOSE || msg.message == WM_DESTROY)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			double currentTime = GetSystemTime();
			float deltaTime = (float)(currentTime - lastTime);
			lastTime = currentTime;

			if (m_WindowIsActive)
			{
				if (m_CloseProgramRequested)
				{
					done = true;
				}
				else
				{
					m_GameTimer.Tick();
					eventHandler->DispatchEvents(m_GameCore);

					OutputMessage("TimerSubSystem : Updating High Priority Timers \n");
					timerSystem->UpdateHighPriotityTimers(deltaTime);

					//m_GameCore->GetFramework()->WndProc(m_hWnd, msg.message, m_wParam, m_lParam);

					//m_GameCore->GetSubSystemManager()->Update(deltaTime);

					m_GameCore->Update(deltaTime);

					m_GameCore->Draw();

					//m_GameCore->GetSubSystemManager()->Draw();

					SwapBuffers(m_hDeviceContext);

					for (int i = 0; i < 256; i++)
					{
						m_OldKeyStates[i] = m_KeyStates[i];
					}

					OutputMessage("TimerSubSystem : Updating Low Priority Timers \n");
					timerSystem->UpdateLowPriotityTimers(deltaTime);
					OutputMessage("TimerSubSystem : Cleanup inactive Timers \n");
					timerSystem->CleanupInActiveTimers();

					if (!CALL_OBJECT_CLEANUP_BY_TIMER)
						worldSystem->CleanupWorlds();
				}
			}
		}
	}

	return (int)msg.wParam;
}

void Framework::Shutdown()
{
	// TODO call our DX12 subsystem to close window
	OutputMessage("Framework : Shutting down. \n");
}

void Framework::SetWindowSize(int width, int height)
{
}

void Framework::ResizeWindow(int width, int height)
{
	if (height <= 0) height = 1;
	if (width <= 0) width = 1;

	m_CurrentWindowWidth = width;
	m_CurrentWindowHeight = height;

	if (m_GameCore)
		m_GameCore->OnSurfaceChanged(width, height);

	OutputMessage("Framework : Resizing window to %f , %f \n", width, height);
}

bool Framework::IsKeyDown(int value)
{
	assert(value >= 0 && value < 256);
	return m_OldKeyStates[value] == false && m_KeyStates[value] == true;
}

bool Framework::WasKeyPressed(int value)
{
	assert(value >= 0 && value < 256);
	return m_OldKeyStates[value] == false && m_KeyStates[value] == true;
}

bool Framework::IsMouseButtonDown(int id)
{
	assert(id >= 0 && id < 3);
	return m_MouseButtonStates[id];
}

void Framework::GetMouseCoordinates(int* mx, int* my)
{
	POINT p;
	if (GetCursorPos(&p))
	{
		if (ScreenToClient(m_hWnd, &p))
		{
			*mx = p.x;
			*my = p.y;
		}
	}
}

void Framework::SetMousePosition(ivec2 pos)
{
	POINT point;
	point.x = pos.x;
	point.y = pos.y;
	ClientToScreen(m_hWnd, &point);
	SetCursorPos(point.x, point.y);
}

LRESULT Framework::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (SlitherFramework == nullptr)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	// References to subsystems
	EventHandlerSubSystem* eventSystem = 
		reinterpret_cast<EventHandlerSubSystem*>(SlitherFramework->GetGameCore()->GetSubSystemManager()->GetSubSystemByType(EventSystem));
	//-------------------------

	switch (uMsg)
	{
		case WM_NCCREATE:
			{
				// Set the user data for this hWnd to the Framework* we passed in, used on first line of this method above.
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG>(SlitherFramework));

				SlitherFramework->m_hWnd = hWnd;
			}
			return 1;
		case WM_SIZE:
			{
				//RenderSystem->GetRenderer()->SetWidth(LOWORD(lParam));
				//RenderSystem->GetRenderer()->SetHeight(HIWORD(lParam));
				//RenderSystem->GetRenderer()->OnResize();

				SlitherFramework->ResizeWindow(LOWORD(lParam), HIWORD(lParam));
			}
			return 0;
		case WM_EXITSIZEMOVE:
			{
				//RenderSystem->GetRenderer()->OnResize();
			}		
			return 0;
		case WM_DESTROY:
			{
				SlitherFramework->m_hWnd = nullptr;
				SlitherFramework->m_CloseProgramRequested = true;
				OutputMessage("Framework : Destroy event \n");
			}
			return 0;
		case WM_ACTIVATE:
			{
				if (!HIWORD(wParam))
					{
						SlitherFramework->m_WindowIsActive = true;
						OutputMessage("Framework : Window is still active. \n");
					}
				else
					{
						SlitherFramework->m_WindowIsActive = false;
						OutputMessage("Framework : Window is not active. \n");
					}
			}
			return 0;
		case WM_SYSCOMMAND:
			{
				switch (wParam)
					{
						// Don't let screensaver or monitor power save mode kick in.
						case SC_SCREENSAVE:
						case SC_MONITORPOWER:
							return 0;
					}
			}
			break;
		case WM_CLOSE:
			{
				PostQuitMessage(0);
				SlitherFramework->m_CloseProgramRequested = true;		
				VulkanSubsystem* vulkan = reinterpret_cast<VulkanSubsystem*>(SlitherFramework->GetGameCore()->GetSubSystemManager()->GetSubSystemByType(SubSystemID::VulkanSystem));
				if (vulkan)
				{
					vulkan->Cleanup();
				}
				OutputMessage("Framework : Close Program Requested \n");
			}
			return 0;
		case WM_KEYDOWN:
			{
				bool keyWasPressedLastTimeMessageArrived = lParam & (1 << 30);

				if (keyWasPressedLastTimeMessageArrived == false)
					{
						if (wParam == VK_ESCAPE && SlitherFramework->m_UseEscapeToQuit)
						{
							SlitherFramework->m_CloseProgramRequested = true;
							OutputMessage("Framework : Close Program Requested \n");
						}	

						SlitherFramework->m_KeyStates[wParam] = true;
					}

				Event* pEvent = new InputEvent(InputDeviceType_Keyboard, InputState_Pressed, (int)wParam, 0);
				eventSystem->QueueEvent(pEvent);
			}
			return 0;
		case WM_KEYUP:
			{
				SlitherFramework->m_KeyStates[(int)wParam] = false;
				if (wParam == VK_ESCAPE && SlitherFramework->m_UseEscapeToQuit)
				{
					PostQuitMessage(0);
					SlitherFramework->m_CloseProgramRequested = true;
				}
				else
				{
					Event* pEvent = new InputEvent(InputDeviceType_Keyboard, InputState_Released, (int)wParam, 0);
					eventSystem->QueueEvent(pEvent);
				}
				
			}
			return 0;
		case WM_MOUSEMOVE:
			{
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);

				Event* pEvent = new InputEvent(InputDeviceType_Mouse, InputState_Held, -1, vec2((float)x, (float)y));
				eventSystem->QueueEvent(pEvent);
			}
		return 0;
        case WM_LBUTTONDOWN:
			{
				SlitherFramework->m_MouseButtonStates[0] = true;

				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);

				Event* pEvent = new InputEvent(InputDeviceType_Mouse, InputState_Pressed, 0, vec2((float)x, (float)y));
				eventSystem->QueueEvent(pEvent);
			}
        return 0;
		case WM_LBUTTONUP:
			{
				SlitherFramework->m_MouseButtonStates[0] = false;

				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);

				Event* pEvent = new InputEvent(InputDeviceType_Mouse, InputState_Released, 0, vec2((float)x, (float)y));
				eventSystem->QueueEvent(pEvent);
			}
			return 0;
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);
		case WM_QUIT:
			SlitherFramework->m_CloseProgramRequested = true;
			return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Framework::HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
		case CTRL_SHUTDOWN_EVENT:
			m_CloseProgramRequested = true;
			return true;
	}

	return true;
}

Framework* Framework::SlitherFramework;