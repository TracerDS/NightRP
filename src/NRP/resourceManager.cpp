#include <mta/mta_main.hpp>
#include <NRP/resourceManager.hpp>
#include <fstream>

namespace NightRP {
	Resource::Resource(const char* name) noexcept : m_name(name) {
		if (fs::exists(ResourcesPath / name))
			return;

		if (!fs::create_directory(ResourcesPath / name)) {
			pModuleManager->ErrorPrintf("Cannot create resource \"%s\"!\n", name);
			return;
		}
		std::ofstream file(ResourcesPath / name / "meta.xml", std::ios::binary);
		if (!file) {
			pModuleManager->ErrorPrintf("Cannot create meta.xml file for resource \"%s\"!\n", name);
			return;
		}
		file << "<meta>\n\n</meta>";
		file.close();
	}
}