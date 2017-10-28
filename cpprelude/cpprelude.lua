project "cpprelude"
	language "C++"
	kind "SharedLib"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/cpprelude/")

	files {"include/**.h", "src/**.cpp"}

	includedirs {"include/"}

	filter "action:gmake"
		buildoptions {"-std=c++14", "-Wall", "-fno-rtti", "-fno-exceptions"}
		linkoptions {"-pthread"}

	filter "action:vs*"
		buildoptions {"-utf-8"}

	filter "configurations:debug"
		targetsuffix "d"
		defines {"DEBUG", "COMPILE_DYNAMIC_LIB"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG", "COMPILE_DYNAMIC_LIB"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x64"
		architecture "x64"