solution "gmsv_sysinfo"

	language "C++"
	location ( os.get() .."-".. _ACTION )
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE", "Unicode" }
	targetdir ( "lib/" .. os.get() .. "/" )
	includedirs { "include/" }
	
	if os.is("windows") then
		targetsuffix ("_win32")

	end

	configurations
	{ 
		"Release"
	}
	
	configuration "Release"
		defines { "NDEBUG" }
		flags{ "Optimize", "FloatFast" }
	
	project "gmsv_sysinfo"
		defines { "GMMODULE" }
		files { "src/**.*", "include/**.*" }
		kind "SharedLib"
		