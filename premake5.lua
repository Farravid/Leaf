include "Dependencies.lua"

workspace "LeafEngine"
	startproject "LeafEditor"
	architecture "x64"

	configurations { "Debug", "Release", "Shipping" }
	platforms { "x64" }
	flags { "MultiProcessorCompile" }
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.platform}"

group "Dependencies"
	include "Leaf/vendor/glfw"
	include "Leaf/vendor/imgui"
group ""

include "Leaf"
include "LeafEditor"
include "LeafGame"

-- Clean ACTION --
newaction
{
	trigger     =   "clean",
	description =	"Clean the generated project files and temporary objects",
	execute		=	function()
		print("==== Cleaning Leaf ====")
		os.execute([["Premake\premake5 clean_engine"]])
		
		print("==== Cleaning Leaf Editor ====")
		os.execute([["Premake\premake5 clean_editor"]])

		print("==== Cleaning Leaf Game ====")
		os.execute([["Premake\premake5 clean_game"]])
		
		print("==== Cleaning Engine ====")
			os.rmdir("./bin")
			os.rmdir("./bin-obj")
			os.remove("LeafEngine.sln")
			os.remove("Makefile")
	end
}