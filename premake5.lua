include "Dependencies.lua"

workspace "LeafEngine"
    architecture "x64"
    startproject "LeafGame"

    configurations { "Debug", "Release" }
    flags { "MultiProcessorCompile" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Leaf/vendor/glfw"
    include "Leaf/vendor/imgui"
group ""

include "Leaf"
include "LeafGame"