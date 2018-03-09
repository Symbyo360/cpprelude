function contains(list, name)
	for index, item in pairs(list) do
		if item == name then
			return true
		end
	end
	return false
end

function process_files(files, name)
	if name == _OPTIONS["docs_target"] then
		printf("Generating Docs for project '%s'", name)
		local visited = {}
		local visited_ix = 0
		for k, v in pairs(files) do
			if string.find(v, ".h", 1, true) and contains(visited, v) == false then
				visited[visited_ix] = v
				visited_ix = visited_ix + 1
				os.execute("python Docit.py autodoc " .. v .. " ".. name)
			end
		end
	end
end

newoption {
   trigger     = "docs_target",
   value       = "project",
   description = "Choose a particular project to generate the documentation to",
}

premake.modules.docs = {}
local m = premake.modules.docs

local p = premake

newaction {
	trigger = "docs",
	description = "Generates markdown documentation that's extracted from the .h files",

	onProject = function(prj)
		process_files(prj.files, prj.name)
	end
}

return m