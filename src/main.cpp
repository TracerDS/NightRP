#include <main.hpp>
#include <NRP/resourceManager.hpp>
#include <NRP/metaParser.hpp>
#include <NRP/CurlWrapper.hpp>

MTAEXPORT bool InitModule(
	ILuaModuleManager10* pManager,
	char* szModuleName,
	char* szAuthor,
	float* fVersion
) {
	pModuleManager = pManager;
	MTAPath = fs::current_path();
	ResourcesPath = MTAPath / "mods/deathmatch/resources";

	Helper::strncpy_s(szModuleName, MODULE_NAME, MAX_INFO_LENGTH); // Set module name
	Helper::strncpy_s(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH); // Set module author
	*fVersion = MODULE_VERSION; // Set module version

	pModuleManager->Printf("Module \"%s\" has been startedfd!\n", MODULE_NAME);

	NightRP::Curl::CurlWrapper wrapper;
	wrapper.Get(MODULE_GITHUB_REPO);
	

	return true;
}

MTAEXPORT bool DoPulse() { return true; }

MTAEXPORT bool ShutdownModule() {
	pModuleManager->Printf("Module \"%s\" is shutting down!\n", MODULE_NAME);
	return true;
}

MTAEXPORT void RegisterFunctions(lua_State* luaVM) {}
MTAEXPORT void ResourceStopping(lua_State* luaVM) {}
MTAEXPORT void ResourceStopped(lua_State* luaVM) {}