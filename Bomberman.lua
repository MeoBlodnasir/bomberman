#!lua

CodeName				= "Code"
CodeDir 				= CodeName.."/"
BinName					= "Bin"
BinDir					= BinName.."/"

RootName				= "."
RootDir					= "./"..RootName.."/"
	EngineName			= "Engine"
	EngineDir			= RootDir..EngineName.."/"
	GameName			= "Game"
	GameDir				= RootDir..GameName.."/"
	ExternalName		= "External"
	ExternalDir			= RootDir..ExternalName.."/"
	TestsName			= "Tests"
	TestsDir			= RootDir..TestsName.."/"

dofile("External.lua")
dofile("Engine.lua")
dofile("Game.lua")
dofile("Tests.lua")

solution ("Bomberman")
	location (RootDir)
	configurations {"Debug", "Release"}
	platforms "x32"
	configuration {"x32"}
		defines	{"__FT_WIN32__=1"}
	configuration {"Debug"}
		defines	{"__FT_DEBUG__=1"}
	configuration {"Release"}
		defines	{"__FT_RELEASE__=1"}
	configuration {}
	
	DefineEngineProject(EngineName, EngineDir, "StaticLib")
	
	DefineTestsProject("Tests", TestsDir, EngineName, EngineDir)
		LinkExternalLib("SFML", "GLM")
	--	LinkExternalLib("SFML", "GLM", "OpenGL")
	
	DefineGameProject("Game", GameDir)
		LinkExternalLib("SFML", "GLM")
	--	LinkExternalLib("SFML", "GLM", "OpenGL")
		