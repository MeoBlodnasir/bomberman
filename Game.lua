#!lua

function DefineGameProject(ProjectName, ProjectDir)
	
	project (ProjectName)
		location (ProjectDir)
		
		configuration {}
			kind		"WindowedApp"
			language	"C++"
			
			targetdir	(RootDir..BinDir)
			debugdir	(RootDir)
			
			includedirs	{EngineDir..CodeDir, ProjectDir..CodeDir}

			files		{ProjectDir..CodeDir.."**.h",
						 ProjectDir..CodeDir.."**.hpp",
						 ProjectDir..CodeDir.."**.c",
						 ProjectDir..CodeDir.."**.cpp",
						 ProjectDir..CodeDir.."**.tpp",
						 ProjectDir..CodeDir.."**.inl"}

			vpaths		{["*"] = ProjectDir..CodeDir}
			libdirs		{RootDir..BinDir}
			excludes	{ProjectDir..CodeDir.."**/*.bak"}
			
			buildoptions {"-std=c++0x"}
			
			LinkEngineLib()
			
		configuration {"Debug"}
			targetname	(ProjectName.."_d")
			flags		{"Symbols"}
		configuration {"Release"}
			targetname	(ProjectName.."_r")
			flags		{"Optimize", "ExtraWarnings", "FatalWarnings"}
		
		configuration {}
end
