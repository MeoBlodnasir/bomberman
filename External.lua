#!lua

---------------------------------------------------

function LinkExternalLib_SFML()

	configuration {}
		includedirs	{ExternalDir}
	configuration {}
		libdirs		{ExternalDir.."SFML"}
	configuration {"Debug"}
		if (IS_WIN_BUILD) then
			links		{"sfml-system-d", "sfml-window-d", "sfml-graphics-d", "sfml-main-d"}
		else
			links		{"sfml-system-d", "sfml-window-d", "sfml-graphics-d"}
		end
	configuration {"Release"}
		if (IS_WIN_BUILD) then
			links		{"sfml-system", "sfml-window", "sfml-graphics", "sfml-main"}
		else
			links		{"sfml-system", "sfml-window", "sfml-graphics"}
		end
	configuration {}
end

---------------------------------------------------

function LinkExternalLib_GLM()

	configuration {}
		includedirs	{ExternalDir}
	configuration {}
end

---------------------------------------------------

bGlewStatic = false;

function LinkExternalLib_OpenGL()

	if (IS_WIN_BUILD) then
		includedirs	{ExternalDir.."OpenGL/glew/include"}
		libdirs		{ExternalDir.."OpenGL", ExternalDir.."OpenGL/glew/lib"}
		links		{"OpenGL32"}
		if (bGlewStatic) then
			defines	{"GLEW_STATIC=1"}
			configuration {"Debug"}
				links		{"glew32sd"}
			configuration {"Release"}
				links		{"glew32s"}
		else
			configuration {"Debug"}
				links		{"glew32d"}
			configuration {"Release"}
				links		{"glew32"}
		end
	end
	configuration {}
end

---------------------------------------------------

LinkExternalLibFunctions =  {
	["SFML"]	= LinkExternalLib_SFML,
	["GLM"]		= LinkExternalLib_GLM,
	["OpenGL"]	= LinkExternalLib_OpenGL,
}

function LinkExternalLib(...)
	
	for i, libName in ipairs(arg) do
		LinkExternalLibFunctions[libName]()
	end
end
