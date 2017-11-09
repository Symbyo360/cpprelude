-- [[ function returning the sdk version of windows 10 --]]
function win10_sdk_version()
	cmd_file = io.popen("reg query \"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\" /v CurrentBuildNumber | C:\\Windows\\System32\\find.exe \"CurrentBuildNumber\"", 'r')
	output = cmd_file:read("*all")
	cmd_file:close()
	out_name, out_type, out_number = output:match("%s*([^%s]+)%s*([^%s]+)%s*([^%s]+)%s*")
	return "10.0." .. out_number .. ".0"
end

bin_path 		= path.getabsolute("bin")
build_path 		= path.getabsolute("build")
cpprelude_path 	= path.getabsolute("cpprelude")

workspace "cpprelude"
	configurations {"debug", "release"}
	platforms {"x86", "x64"}
	location "build"
	startproject "scratch"

	include "cpprelude/cpprelude.lua"
	include "scratch/scratch.lua"
	include "unittest/unittest.lua"