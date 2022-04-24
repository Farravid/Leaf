project "LeafEditor"
	kind "WindowedApp"
	language "C#"
	clr "On"

	targetdir   ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir      ("%{wks.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	dotnetframework "net6.0"

	nuget { "Avalonia:0.10.13", "Avalonia.Desktop:0.10.13",
			"Avalonia.Diagnostics:0.10.13", "Avalonia.ReactiveUI:0.10.13",
			"XamlNameReferenceGenerator:1.3.4"
		  }

	files
	{
		"src/**.cs",
		"src/**.xaml",
		"src/**.xaml.cs",
	}

	links
	{
		"Microsoft.CodeAnalysis.CSharp.NetAnalyzers",
		"Microsoft.CodeAnalysis.NetAnalyzers",
		"System.Text.Json.SourceGeneration",
		"Microsoft.NETCore.APP",
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
