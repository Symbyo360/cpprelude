project "cpprelude"
	language "C++"
	kind "SharedLib"
	targetdir (bin_path .. "/%{cfg.platform}/%{cfg.buildcfg}/")
	location  (build_path .. "/%{prj.name}/")
	
	files
	{
		"include/**.h",
		"src/**.cpp",
		tracy_path .. "/Tracy.hpp",
		tracy_path .. "/TracyClient.cpp"
	}

	includedirs 
	{
		"include/",
		tracy_path
	}

	--language configuration
	exceptionhandling "OFF"
	rtti "OFF"
	warnings "Extra"
	cppdialect "c++14"


	--linux configuration
	filter "system:linux"
		defines { "OS_LINUX" }
		linkoptions {"-pthread"}

	filter { "system:linux", "configurations:debug" }
		linkoptions {"-rdynamic"}

	filter { "system:linux", "platforms:x86"}
		files
		{
			"src/sewing-fcontext/asm/make_i386_sysv_elf_gas.S",
			"src/sewing-fcontext/asm/jump_i386_sysv_elf_gas.S"
		}

	filter { "system:linux", "platforms:x64" }
		files
		{
			"src/sewing-fcontext/asm/make_x86_64_sysv_elf_gas.S",
			"src/sewing-fcontext/asm/jump_x86_64_sysv_elf_gas.S"
		}


	--windows configuration
	filter "system:windows"
		defines { "OS_WINDOWS" }
		if os.getversion().majorversion == 10.0 then
			systemversion(win10_sdk_version())
		end

	filter { "system:windows", "action:vs*"}
		files {"tools/vs/cpprelude.natvis"}
		
	filter { "system:windows", "configurations:debug" }
		links {"dbghelp"}

	filter { "system:windows", "platforms:x86" }
		files
		{
			"src/sewing-fcontext/asm/make_i386_ms_pe_masm.asm",
			"src/sewing-fcontext/asm/jump_i386_ms_pe_masm.asm"
		}

	filter { "system:windows", "platforms:x64" }
		files
		{	
			"src/sewing-fcontext/asm/make_x86_64_ms_pe_masm.asm",
			"src/sewing-fcontext/asm/jump_x86_64_ms_pe_masm.asm"
		}

	filter { "files:**.asm", "system:windows", "platforms:x86" }
		exceptionhandling "SEH"


	--os agnostic configuration
	filter "configurations:debug"
		targetsuffix "d"
		defines {"DEBUG", "CPPR_DLL", "TRACY_ENABLE"}
		symbols "On"

	filter "configurations:release"
		defines {"NDEBUG", "CPPR_DLL"}
		optimize "On"

	filter "platforms:x86"
		architecture "x32"

	filter "platforms:x64"
		architecture "x64"