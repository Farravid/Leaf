project "LeafEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

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
		"%{IncludeDir.spdlog}",
		"C:/Qt/6.3.0/mingw_64/include/"
	}

	links
	{
		"Leaf",
		"Qt6Widgets",
		"Qt6Gui",
		"Qt6Core"
	}

	postbuildcommands
	{
		"mkdir %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/platforms",
		"{COPY} C:/Qt/6.3.0/msvc2019_64/plugins/platforms %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/platforms",
		"{COPY} C:/Qt/6.3.0/msvc2019_64/bin/Qt6Core.dll %{wks.location}/bin/" .. outputdir .. "/%{prj.name}",
		"{COPY} C:/Qt/6.3.0/msvc2019_64/bin/Qt6Widgets.dll %{wks.location}/bin/" .. outputdir .. "/%{prj.name}",
		"{COPY} C:/Qt/6.3.0/msvc2019_64/bin/Qt6Gui.dll %{wks.location}/bin/" .. outputdir .. "/%{prj.name}"
	}

filter "system:Windows"
	defines "LF_WINDOWS"
	
	
filter "system:Unix"
	defines "LF_LINUX"

filter "configurations:Debug"
	defines "LF_DEBUG"
	runtime "Debug"
	symbols "on"

filter "configurations:Release"
	defines "LF_RELEASE"
	runtime "Release"
	optimize "full"

filter "action:vs2022"
	buildoptions "/Zc:__cplusplus "
	libdirs { "C:/Qt/6.3.0/msvc2019_64/lib" }

