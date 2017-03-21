#!lua

---------------------------------------------------

function LinkExternalLib_SFML()

	configuration {}
		includedirs	{ExternalDir}
	configuration {}
		libdirs		{ExternalDir.."SFML"}
	configuration {"Debug"}
		links		{"sfml-main-d", "sfml-system-d", "sfml-window-d", "sfml-graphics-d"}
	configuration {"Release"}
		links		{"sfml-main", "sfml-system", "sfml-window", "sfml-graphics"}
	configuration {}
end

---------------------------------------------------

function LinkExternalLib_GLM()

	configuration {}
		includedirs	{ExternalDir}
	configuration {}
end

---------------------------------------------------

bGlewStatic = true;

function LinkExternalLib_OpenGL()

	configuration {"x32"}
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
