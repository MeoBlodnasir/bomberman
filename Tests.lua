#!lua

function DefineTestsProject(ProjectName, ProjectDir, ProjectToTestName, ProjectToTestDir)
	
	project (ProjectName)
		location (ProjectDir)
		
		configuration {}
			kind		"WindowedApp"
			language	"C++"
			
			targetdir	(RootDir..BinDir)
			debugdir	(ProjectDir)
			
			includedirs	{EngineDir..CodeDir, ProjectToTestDir..CodeDir, ProjectDir..CodeDir}

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
			links		{ProjectToTestName}
		configuration {"Debug"}
			links		{ProjectToTestName.."_d"}
		configuration {"Release"}
			links		{ProjectToTestName.."_r"}
		
		configuration {}
end
