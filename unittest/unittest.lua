local catch_path = "deps/Catch"

project "unittest"
	language "C++"
	kind "ConsoleApp"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/unittest/")

	files {"include/**.h", "src/**.cpp"}

	includedirs {
		"include/",
		cpprelude_path .. "/include/",
		catch_path .. "/include/"
	}

	links {"cpprelude"}

	if os.istarget("linux") then

		buildoptions {"-std=c++14", "-Wall"}
		linkoptions {"-pthread"}

	elseif os.istarget("windows") then

		if os.getversion().majorversion == 10.0 then
			systemversion(win10_sdk_version())
		end

		buildoptions {"/utf-8"}
	end

	filter "action:vs*"
		buildoptions {"-utf-8"}

	filter "action:vs2017"
		systemversion(win10_sdk_version())

	filter "configurations:debug"
		defines {"DEBUG"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x64"
		architecture "x64"