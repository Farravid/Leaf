require "premake-qt/qt"
local qt = premake.extensions.qt

project "LeafEditor"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	entrypoint "mainCRTStartup"

	targetdir   ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}" )
	objdir      ("%{wks.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	qt.enable()
	qtmodules { "core", "gui", "widgets", "opengl" }
	qtprefix "Qt6"

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
	}

	links
	{
		"Leaf"
	}

filter "system:windows"
	defines "LF_WINDOWS"
	qtpath "C:/Qt/6.3.0/msvc2019_64"
	
filter "system:linux"
	defines "LF_LINUX"

filter "configurations:Debug"
	kind "ConsoleApp"
	defines "LF_DEBUG"
	runtime "Debug"
	symbols "on"
	qtsuffix "d"
	postbuildcommands
	{
		"{MKDIR} %{cfg.buildtarget.directory}/platforms",
		"{COPY} %{cfg.qtpath}/plugins/platforms %{cfg.buildtarget.directory}/platforms",
		"{COPY} %{cfg.qtpath}/bin/Qt6Cored.dll %{cfg.buildtarget.directory}",
		"{COPY} %{cfg.qtpath}/bin/Qt6Widgetsd.dll %{cfg.buildtarget.directory}",
		"{COPY} %{cfg.qtpath}/bin/Qt6Guid.dll %{cfg.buildtarget.directory}"
	}

filter "configurations:Release"
	defines "LF_RELEASE"
	runtime "Release"
	optimize "full"
	postbuildcommands
	{
		"{MKDIR} %{cfg.buildtarget.directory}/platforms",
		"{COPY} %{cfg.qtpath}/plugins/platforms %{cfg.buildtarget.directory}/platforms",
		"{COPY} %{cfg.qtpath}/bin/Qt6Core.dll %{cfg.buildtarget.directory}",
		"{COPY} %{cfg.qtpath}/bin/Qt6Widgets.dll %{cfg.buildtarget.directory}",
		"{COPY} %{cfg.qtpath}/bin/Qt6Gui.dll %{cfg.buildtarget.directory}"
	}
	
filter {"system:linux", "configurations:not Debug"}
	buildoptions { "-mwindow" }
	
filter "action:vs2022"
	buildoptions "/Zc:__cplusplus "
	qtpath "C:/Qt/6.3.0/msvc2019_64"

filter "action:gmake2"
	qtpath "C:/Qt/6.3.0/mingw_64"

newaction
{
	trigger     =   "clean_editor",
	description =	"Clean the generated project files and temporary objects",
	execute		=	function()
		os.remove("LeafEditor/*.vcxproj.*")
		os.remove("LeafEditor/Makefile")
	end
}

