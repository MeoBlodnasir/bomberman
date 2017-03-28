#!lua

IS_WIN_BUILD = (_ACTION == "vs2012" or _ACTION == "vs2015")
IS_UNIX_BUILD = not IS_WIN_BUILD

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
	configuration {"Debug"}
		defines	{"__FT_DEBUG__=1"}
	configuration {"Release"}
		defines	{"__FT_RELEASE__=1"}
	configuration {}
	
	DefineEngineProject(EngineName, EngineDir, "StaticLib")
	
	DefineTestsProject("Tests", TestsDir, EngineName, EngineDir)
		LinkExternalLib("SFML", "glm", "OpenGL", "assimp")
	
	DefineGameProject("Game", GameDir)
		LinkExternalLib("SFML", "glm", "OpenGL", "assimp")
		
	DefineGameProject("Djobi", RootDir.."Djobi/")
        LinkExternalLib("SFML", "glm", "OpenGL", "assimp")

    DefineGameProject("Gonzo", RootDir.."Gonzo/")
        LinkExternalLib("SFML", "glm", "OpenGL", "assimp")

