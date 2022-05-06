include "Dependencies.lua"

workspace "LeafEngine"
	startproject "LeafEditor"

	configurations { "Debug", "Release" }
	platforms { "x64" }
	flags { "MultiProcessorCompile" }
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.platform}"

group "Dependencies"
	include "Leaf/vendor/glfw"
	include "Leaf/vendor/imgui"
group ""

include "Leaf"
include "LeafEditor"
--include "LeafGame"

-- Clean ACTION --
newaction
{
	trigger     =   "clean",
	description =	"Clean the generated project files and temporary objects",
	execute		=	function()
		print("==== Cleaning Leaf ====")
			os.remove("Leaf/Leaf.vcxproj")
			os.remove("Leaf/Leaf.vcxproj.filters")
			os.remove("Leaf/Leaf.vcxproj.user")
			os.remove("Leaf/Makefile")
		print("==== Cleaning Leaf Editor ====")
			os.rmdir("LeafEditor/bin")
			os.rmdir("LeafEditor/obj")
			os.remove("LeafEditor/LeafEditor.csproj")
		print("==== Cleaning Leaf Game ====")
			os.remove("LeafGame/LeafGame.vcxproj")
			os.remove("LeafGame/LeafGame.vcxproj.user")
			os.remove("LeafGame/Makefile")
		print("==== Cleaning Engine ====")
			os.rmdir("./bin")
			os.rmdir("./bin-obj")
			os.remove("LeafEngine.sln")
			os.remove("Makefile")
	end
}