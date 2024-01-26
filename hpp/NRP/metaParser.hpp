#pragma once

#include <string_view>
#include <vector>
#include <string>

// Forward declaration
namespace pugi { class xml_document; }

namespace NightRP::Meta {
	enum class ScriptType {
		SERVER,
		CLIENT,
		SHARED
	};
	constexpr std::string ScriptTypeToString(const ScriptType& string) noexcept;
	constexpr ScriptType ScriptTypeFromString(std::string string) noexcept;

	struct MetaInfo {
		std::string author = "";
		float version = 1.0;
		std::string name = "";
		std::string description = "";
		std::string type = "";
		std::vector<std::string> gamemodes;

		static constexpr bool IsEmpty(const MetaInfo& script) noexcept;
	};
	struct MetaScript {
		std::string source = "";
		ScriptType type = ScriptType::SERVER;
		bool cache = true;
		bool validate = true;
	};
	struct MetaMap {
		std::string source = "";
		int dimension;
	};
	struct MetaFile {
		std::string source = "";
		bool download = true;
	};
	struct MetaInclude {
		std::string resource = "";
		std::string minversion = "";
		std::string maxversion = "";
	};
	struct MetaConfig {
		std::string source = "";
		ScriptType type = ScriptType::SERVER;
	};
	struct MetaExport {
		std::string function = "";
		ScriptType type = ScriptType::SERVER;
		bool http = true;
	};
	struct MetaHTML {
		std::string source = "";
		bool defaultFile;
		bool raw;
	};
	struct MetaSetting {
		std::string name = "";
		std::string value = "";
		std::string friendlyName = "";
		std::string accept = "";
		std::string examples = "";
		std::string description = "";
	};
	struct MetaMinVersion {
		std::string client = "";
		std::string server = "";
	};
	struct MetaACLRight {
		std::string name = "";
		bool access;
	};

	class Meta {
		MetaInfo m_info{};
		std::vector<MetaScript> m_scripts{};
		std::vector<MetaMap> m_maps{};
		std::vector<MetaFile> m_files{};
		std::vector<MetaInclude> m_includes{};
		MetaConfig m_config{};
		std::vector<MetaExport> m_exports{};
		std::vector<MetaHTML> m_html{};
		std::vector<MetaSetting> m_settings{};
		MetaMinVersion m_minVersion{};
		std::vector<MetaACLRight> m_aclRequests{};

		bool m_syncElementData = true;
		bool m_oop = false;
		int m_downloadPriority = 0;

		static Meta ParseXML(pugi::xml_document& metaXML) noexcept;
	public:
		Meta() noexcept;

		void ToFile(const char* path) const noexcept;
		std::string ToString() const noexcept;

		static Meta Parse(std::string content) noexcept;
		static Meta ParseFile(const char* path) noexcept;

		constexpr MetaInfo& GetInfo() noexcept;
		constexpr const MetaInfo& GetInfo() const noexcept;

		constexpr std::vector<MetaScript>& GetScripts() noexcept;
		constexpr const std::vector<MetaScript>& GetScripts() const noexcept;

		constexpr std::vector<MetaMap>& GetMaps() noexcept;
		constexpr const std::vector<MetaMap>& GetMaps() const noexcept;

		constexpr std::vector<MetaFile>& GetFiles() noexcept;
		constexpr const std::vector<MetaFile>& GetFiles() const noexcept;

		constexpr std::vector<MetaInclude>& GetIncludes() noexcept;
		constexpr const std::vector<MetaInclude>& GetIncludes() const noexcept;

		constexpr MetaConfig& GetConfigs() noexcept;
		constexpr const MetaConfig& GetConfigs() const noexcept;

		constexpr std::vector<MetaExport>& GetExports() noexcept;
		constexpr const std::vector<MetaExport>& GetExports() const noexcept;

		constexpr std::vector<MetaHTML>& GetHTML() noexcept;
		constexpr const std::vector<MetaHTML>& GetHTML() const noexcept;

		constexpr std::vector<MetaSetting>& GetSettings() noexcept;
		constexpr const std::vector<MetaSetting>& GetSettings() const noexcept;
		
		constexpr MetaMinVersion& GetMinVersion() noexcept;
		constexpr const MetaMinVersion& GetMinVersion() const noexcept;

		constexpr std::vector<MetaACLRight>& GetACLRequests() noexcept;
		constexpr const std::vector<MetaACLRight>& GetACLRequests() const noexcept;

		constexpr bool& GetSyncedElementData() noexcept;
		constexpr const bool& GetSyncedElementData() const noexcept;

		constexpr bool& GetOOP() noexcept;
		constexpr const bool& GetOOP() const noexcept;

		constexpr int& GetDownloadPriority() noexcept;
		constexpr const int& GetDownloadPriority() const noexcept;
	};
}