project "Leaf"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir   ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir      ("%{wks.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "lfpch.hpp"
	pchsource "src/lfpch.cpp"

	callingconvention ("FastCall")
	floatingpoint "Fast"
	
	files
	{
		"src/**.hpp",
		"src/**.cpp",
		"src/**.tpp",
	}

	includedirs
	{
		"src",
		"src/%{prj.name}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"glfw",
		"imgui"
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

newaction
{
	trigger     =   "clean_engine",
	description =	"Clean the generated project files and temporary objects",
	execute		=	function()
		print("==== Cleaning vendor/glfw ====")
			os.execute([["Premake\premake5 clean_glfw"]])
		print("==== Cleaning vendor/imgui ====")
			os.execute([["Premake\premake5 clean_imgui"]])

			os.remove("Leaf/*.vcxproj.*")
			os.remove("Leaf/Makefile")
	end
}
