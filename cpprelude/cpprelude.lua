project "cpprelude"
	language "C++"
	kind "SharedLib"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/cpprelude/")

	files
	{
		"include/**.h",
		"src/**.cpp"
	}

	includedirs {"include/"}

	exceptionhandling "OFF"
	rtti "OFF"
	warnings "Extra"
	cppdialect "c++14"

	local sewing_os = ""
	local sewing_abi = ""
	local sewing_asm = ""

	filter "system:linux"
		sewing_os = "sysv"
		sewing_abi = "elf"
		sewing_asm = "gas.S"

		linkoptions {"-pthread"}

		filter "configurations:debug"
			linkoptions {"-rdynamic"}

	filter "system:windows"
		sewing_os = "ms"
		sewing_abi = "pe"
		sewing_asm = "masm.asm"

		if os.getversion().majorversion == 10.0 then
			systemversion(win10_sdk_version())
		end

		filter "action:vs*"
			files {"tools/vs/cpprelude.natvis"}
		
		filter "configurations:debug"
			links {"dbghelp"}

	filter "configurations:debug"
		targetsuffix "d"
		defines {"DEBUG", "CPPR_DLL"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG", "CPPR_DLL"}
		optimize "On"

	filter "platforms:x86"
		files
		{
			"src/sewing-fcontext/asm/make_i386_" .. sewing_os .. "_" .. sewing_abi .. "_" .. sewing_asm,
			"src/sewing-fcontext/asm/jump_i386_" .. sewing_os .. "_" .. sewing_abi .. "_" .. sewing_asm
		}
		architecture "x32"

	filter "platforms:x64"
		files
		{
			"src/sewing-fcontext/asm/make_x86_64_" .. sewing_os .. "_" .. sewing_abi .. "_" .. sewing_asm,
			"src/sewing-fcontext/asm/jump_x86_64_" .. sewing_os .. "_" .. sewing_abi .. "_" .. sewing_asm
		}
		architecture "x64"

	filter {"files:**.asm", "system:windows", "platforms:x86"}
		exceptionhandling "SEH"