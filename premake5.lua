workspace "cpprelude"
	configurations {"debugShared", "releaseShared", "debugStatic", "releaseStatic"}
	platforms {"x86", "x86_64"}
	location "build"
	startproject "scratch"

project "cpprelude"
	language "C++"
	targetdir "bin/%{cfg.platform}/%{cfg.buildcfg}"
	location "build/cpprelude"

	files {"cpprelude/inc/**.h", "cpprelude/src/**.cpp"}

	includedirs {"cpprelude/inc/"}

	filter "action:gmake"
		buildoptions {"-std=c++14"}
		linkoptions {"-pthread"}

	filter "configurations:debugShared"
		kind "SharedLib"
		defines {"DEBUG", "COMPILE_DYNAMIC_LIB"}
		symbols "On"

	filter "configurations:releaseShared"
		kind "SharedLib"
		defines {"NDEBUG", "COMPILE_DYNAMIC_LIB"}
		optimize "On"

	filter "configurations:debugStatic"
		kind "StaticLib"
		defines {"DEBUG", "STATIC_LIB"}
		symbols "On"

	filter "configurations:releaseStatic"
		kind "StaticLib"
		defines {"NDEBUG", "STATIC_LIB"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x86_64"
		architecture "x64"
		
include "scratch"
include "unittest"