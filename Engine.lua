#!lua

function DefineEngineProject(ProjectName, ProjectDir, ProjectKind)
	
	project (ProjectName)
		location (ProjectDir)
		
		configuration {}
			kind		(ProjectKind)
			language	"C++"
			
			targetdir	(RootDir..BinDir)
			debugdir	(RootDir)
			
			includedirs	{ProjectDir..CodeDir}

			files		{ProjectDir..CodeDir.."**.h",
						 ProjectDir..CodeDir.."**.hpp",
						 ProjectDir..CodeDir.."**.c",
						 ProjectDir..CodeDir.."**.cpp",
						 ProjectDir..CodeDir.."**.tpp",
						 ProjectDir..CodeDir.."**.inl"}

			vpaths		{["*"] = ProjectDir..CodeDir}
			libdirs		{}
			excludes	{ProjectDir..CodeDir.."**/*.bak"}
			
			buildoptions {"-std=c++0x"}
			
			LinkExternalLib("SFML", "glm", "OpenGL", "assimp")
			
		configuration {"Debug"}
			targetname	(ProjectName.."_d")
			flags		{"Symbols"}
		configuration {"Release"}
			targetname	(ProjectName.."_r")
			flags		{"Optimize", "ExtraWarnings", "FatalWarnings"}

		configuration {}
end

function LinkEngineLib()

	configuration {}
		links {EngineName}
	configuration {"Debug"}
		links {EngineName.."_d"}
	configuration {"Release"}
		links {EngineName.."_r"}
	configuration {}
end
