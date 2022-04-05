project "Leaf"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir   ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir      ("%{wks.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "lfpch.hpp"
	pchsource "src/lfpch.cpp"
	
	files
	{
		"src/**.hpp",
		"src/**.cpp",
		"src/**.tpp",
	}

	includedirs
	{
		"src",
		"src/Leaf",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.imgui}"
	}

	links
	{
		"glfw",
		"imgui"
	}

filter "configurations:Debug"
	defines "OG_DEBUG"
	runtime "Debug"
	symbols "on"

filter "configurations:Release"
	defines "OG_RELEASE"
	runtime "Release"
	optimize "full"
