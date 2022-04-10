project "LeafGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir   ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir      ("%{wks.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.hpp",
		"src/**.cpp",
		"src/**.tpp",
	}

	includedirs
	{
		"src",
		"%{wks.location}/Leaf/src",
		"%{wks.location}/Leaf/src/Leaf",
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"Leaf"
	}

filter "configurations:Debug"
	defines "LF_DEBUG"
	runtime "Debug"
	symbols "on"

filter "configurations:Release"
	defines "LF_RELEASE"
	runtime "Release"
	optimize "full"
