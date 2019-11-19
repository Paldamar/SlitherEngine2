------------------------------------------------ Solution
workspace "Game"
    configurations  { "Debug", "Release" }
    location        "build"
    startproject    "MainGame"

	staticruntime "on"

    filter "system:windows"
        platforms       { "x64" }
        --characterset    "MBCS"

	vpaths {
        -- Place these files in the root of the project.
        [""] = {},
        -- Place the SourceCommon and SourceEditor folders in the root of the project.
        ["Source"] = {
            "**.cpp",
        },
        ["Headers"] = {
            "**.h",
        },
    }

------------------------------------------------ Game Project
project "MainGame"
    location    "build"
    dependson   { "SlitherEngine", "Math", "SubSystem", "World", "DirectX", "ECS","XAudio", "PhsyX", "EngineMacros" }
    kind        "WindowedApp"
    language    "C++"
    debugdir    "."
    pchheader   "GamePCH.h"
    pchsource   "Game/MainGame/GamePCH.cpp"

    includedirs {
		"Libraries/FBX_SDK/include"
    }

    files {
        "Game/MainGame/**.cpp",
        "Game/MainGame/**.h",
        "premake5.lua",
        "GenerateBuildFiles.bat",
        --".gitignore",
    }

	vpaths {
        -- Place these files in the root of the project.
        [""] = {
			"premake5.lua",
			"GenerateBuildFiles.bat",
			".gitignore",
		},
	}

    links {
        "SlitherEngine",
        "Math",
		"DirectX",
		"SubSystem",
		
    }

    filter "configurations:Debug"
        defines         "_DEBUG"
        symbols         "on"
		links {
			"Libraries/FBX_SDK/lib/vs2015/x64/debug/libfbxsdk-mt",
		}

    filter "configurations:Release"
        defines         "NDEBUG"
        optimize        "Full"
		links {
			"Libraries/FBX_SDK/lib/vs2015/x64/release/libfbxsdk-mt",
		}

------------------------------------------------ SlitherEngine Project
project "SlitherEngine"
    location    "build"
    dependson   { "Math", "DirectX","SubSystem","XAudio", "EngineMacros" }
    kind        "StaticLib"
    language    "C++"
    pchheader   "SlitherEnginePCH.h"
    pchsource   "Engine/SlitherEngine/SlitherEnginePCH.cpp"

    includedirs {
		"Libraries/FBX_SDK/include"
    }

    files {
        "Engine/SlitherEngine/**.cpp",
        "Engine/SlitherEngine/**.h",
    }
	
    links {
        "Math",
		"DirectX",
		"SubSystem",
		"XAudio"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    --filter "files:Libraries/Framework/Libraries/**"
    --    flags   "NoPCH"

    --filter "files:Libraries/Framework/Libraries/LodePNG/**"
    --    disablewarnings { "4551", "4334", "4267" }
	
	    filter "configurations:Debug"
        defines         "_DEBUG"
        symbols         "on"
		links {
			"Libraries/FBX_SDK/lib/vs2015/x64/debug/libfbxsdk-mt",
		}

    filter "configurations:Release"
        defines         "NDEBUG"
        optimize        "Full"
		links {
			"Libraries/FBX_SDK/lib/vs2015/x64/release/libfbxsdk-mt",
		}

------------------------------------------------ Math Project
project "Math"
    location    "build"
	dependson   { "Math", "EngineMacros" }
    kind        "StaticLib"
    language    "C++"
    pchheader   "MathPCH.h"
    pchsource   "Engine/Math/MathPCH.cpp"

    includedirs {
    }

    files {
        "Engine/Math/**.cpp",
        "Engine/Math/**.h",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ DirectX Project
project "DirectX"
    location    "build"
	dependson   { "Math", "EngineMacros" }
    kind        "StaticLib"
    language    "C++"
    pchheader   "DX12PCH.h"
    pchsource   "Engine/DX12/DX12PCH.cpp"

    includedirs {
		"Libraries/FBX_SDK/include"
    }

    files {
        "Engine/DX12/**.cpp",
        "Engine/DX12/**.h",
    }
	
	links {
        "Math",
		"SubSystem"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ Subsystem Project
project "SubSystem"
    location    "build"
	dependson   { "DirectX", "Events", "XAudio", "World", "EngineMacros", "Timers" }
    kind        "StaticLib"
    language    "C++"
    pchheader   "SubsystemPCH.h"
    pchsource   "Engine/Subsystem/SubsystemPCH.cpp"

    includedirs {
		"Libraries/FBX_SDK/include"
    }

    files {
        "Engine/Subsystem/**.cpp",
        "Engine/Subsystem/**.h",
    }
	
	links {
        "Math",
		"Events",
		"Timers"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ ECS Project
project "ECS"
    location    "build"
	dependson   {"EngineMacros"}
    kind        "StaticLib"
    language    "C++"
    pchheader   "ECSPCH.h"
    pchsource   "Engine/ECS/ECSPCH.cpp"

    includedirs {
    }

    files {
        "Engine/ECS/**.cpp",
        "Engine/ECS/**.h",
    }
	
	links {
        "Math",
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ XAudio Project
project "XAudio"
    location    "build"
	dependson   {"EngineMacros"}
    kind        "StaticLib"
    language    "C++"
    pchheader   "XAudioPCH.h"
    pchsource   "Engine/XAudio/XAudioPCH.cpp"

    includedirs {
    }

    files {
        "Engine/XAudio/**.cpp",
        "Engine/XAudio/**.h",
    }
	
	links {
        "Math",
		"Subsystem"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ World Project
project "World"
    location    "build"
	dependson   {"EngineMacros"}
    kind        "StaticLib"
    language    "C++"
    pchheader   "WorldPCH.h"
    pchsource   "Engine/World/WorldPCH.cpp"

    includedirs {
		"Libraries/FBX_SDK/include"
    }

    files {
        "Engine/World/**.cpp",
        "Engine/World/**.h",
    }
	
	links {
        "Math",
		"Subsystem",
		"XAudio"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ PhsyX Project
project "PhsyX"
    location    "build"
	dependson   {"EngineMacros"}
    kind        "StaticLib"
    language    "C++"
    pchheader   "PhsyXPCH.h"
    pchsource   "Engine/PhsyX/PhsyXPCH.cpp"

    includedirs {
    }

    files {
        "Engine/PhsyX/**.cpp",
        "Engine/PhsyX/**.h",
    }
	
	links {
        "Math",
		"Subsystem",
		"XAudio"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ Events Project
project "Events"
    location    "build"
	dependson   {"EngineMacros"}
    kind        "StaticLib"
    language    "C++"
    pchheader   "EventsPCH.h"
    pchsource   "Engine/Events/EventsPCH.cpp"

    includedirs {
    }

    files {
        "Engine/Events/**.cpp",
        "Engine/Events/**.h",
    }
	
	links {
        "Math"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ EngineMacros Project
project "EngineMacros"
    location    "build"
    kind        "StaticLib"
    language    "C++"
    pchheader   "EngineMacrosPCH.h"
    pchsource   "Engine/EngineMacros/EngineMacrosPCH.cpp"

    includedirs {
    }

    files {
        "Engine/EngineMacros/**.cpp",
        "Engine/EngineMacros/**.h",
    }
	
	links {
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
------------------------------------------------ Timers Project
project "Timers"
    location    "build"
	dependson   {"EngineMacros"}
    kind        "StaticLib"
    language    "C++"
    pchheader   "TimersPCH.h"
    pchsource   "Engine/Timers/TimersPCH.cpp"

    includedirs {
    }

    files {
        "Engine/Timers/**.cpp",
        "Engine/Timers/**.h",
    }
	
	links {
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"