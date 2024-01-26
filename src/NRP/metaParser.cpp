#include <mta/mta_main.hpp>
#include <NRP/metaParser.hpp>
#include <NRP/utils.hpp>
#include <pugixml/pugixml.hpp>

namespace NightRP::Meta {
	constexpr bool MetaInfo::IsEmpty(const MetaInfo& info) noexcept {
		return info.author == "" && info.description == ""
			&& info.gamemodes.empty() && info.name == ""
			&& info.type == "" && info.version == 1.0;
	}

	constexpr ScriptType ScriptTypeFromString(std::string string) noexcept {
		std::transform(string.begin(), string.end(), string.begin(),
			[](const auto& c) { return std::tolower(c); });

		if (string == "client")
			return ScriptType::CLIENT;
		if (string == "shared")
			return ScriptType::SHARED;
		return ScriptType::SERVER;
	}

	constexpr std::string ScriptTypeToString(const ScriptType& type) noexcept {
		switch (type) {
			case ScriptType::CLIENT:
				return "client";
			case ScriptType::SHARED:
				return "shared";
			case ScriptType::SERVER:
			default:
				return "server";
		}
	}

	Meta::Meta() noexcept {}

	constexpr MetaInfo& Meta::GetInfo() noexcept {
		return m_info;
	}
	constexpr const MetaInfo& Meta::GetInfo() const noexcept {
		return m_info;
	}

	constexpr std::vector<MetaScript>& Meta::GetScripts() noexcept {
		return m_scripts;
	}
	constexpr const std::vector<MetaScript>& Meta::GetScripts() const noexcept {
		return m_scripts;
	}

	constexpr std::vector<MetaMap>& Meta::GetMaps() noexcept {
		return m_maps;
	}
	constexpr const std::vector<MetaMap>& Meta::GetMaps() const noexcept {
		return m_maps;
	}

	constexpr std::vector<MetaFile>& Meta::GetFiles() noexcept {
		return m_files;
	}
	constexpr const std::vector<MetaFile>& Meta::GetFiles() const noexcept {
		return m_files;
	}

	constexpr std::vector<MetaInclude>& Meta::GetIncludes() noexcept {
		return m_includes;
	}
	constexpr const std::vector<MetaInclude>& Meta::GetIncludes() const noexcept {
		return m_includes;
	}

	constexpr MetaConfig& Meta::GetConfigs() noexcept {
		return m_config;
	}
	constexpr const MetaConfig& Meta::GetConfigs() const noexcept {
		return m_config;
	}

	constexpr std::vector<MetaExport>& Meta::GetExports() noexcept {
		return m_exports;
	}
	constexpr const std::vector<MetaExport>& Meta::GetExports() const noexcept {
		return m_exports;
	}

	constexpr std::vector<MetaHTML>& Meta::GetHTML() noexcept {
		return m_html;
	}
	constexpr const std::vector<MetaHTML>& Meta::GetHTML() const noexcept {
		return m_html;
	}

	constexpr std::vector<MetaSetting>& Meta::GetSettings() noexcept {
		return m_settings;
	}
	constexpr const std::vector<MetaSetting>& Meta::GetSettings() const noexcept {
		return m_settings;
	}

	constexpr MetaMinVersion& Meta::GetMinVersion() noexcept {
		return m_minVersion;
	}
	constexpr const MetaMinVersion& Meta::GetMinVersion() const noexcept {
		return m_minVersion;
	}

	constexpr std::vector<MetaACLRight>& Meta::GetACLRequests() noexcept {
		return m_aclRequests;
	}
	constexpr const std::vector<MetaACLRight>& Meta::GetACLRequests() const noexcept {
		return m_aclRequests;
	}

	constexpr bool& Meta::GetSyncedElementData() noexcept {
		return m_syncElementData;
	}
	constexpr const bool& Meta::GetSyncedElementData() const noexcept {
		return m_syncElementData;
	}

	constexpr bool& Meta::GetOOP() noexcept {
		return m_oop;
	}
	constexpr const bool& Meta::GetOOP() const noexcept {
		return m_oop;
	}

	constexpr int& Meta::GetDownloadPriority() noexcept {
		return m_downloadPriority;
	}
	constexpr const int& Meta::GetDownloadPriority() const noexcept {
		return m_downloadPriority;
	}

	Meta Meta::Parse(std::string content) noexcept {
		pugi::xml_document metaXML;
		auto res = metaXML.load_buffer(content.c_str(), content.size());
		if (!res) return {};
		return Meta::ParseXML(metaXML);
	}
	Meta Meta::ParseFile(const char* path) noexcept {
		pugi::xml_document metaXML;
		auto res = metaXML.load_file(path);
		if (!res) return {};
		return Meta::ParseXML(metaXML);
	}

	Meta Meta::ParseXML(pugi::xml_document& metaXML) noexcept {
		Meta meta;

		auto rootNode = metaXML.child("meta");

		for (const auto& node : rootNode.children()) {
			auto nodeName = node.name();
			if (!strcmp(nodeName, "info")) {
				if (!MetaInfo::IsEmpty(meta.m_info))
					continue;

				auto infoTag = rootNode.child("info");
				meta.m_info.author = infoTag.attribute("author").as_string();
				meta.m_info.version = infoTag.attribute("version").as_float();
				meta.m_info.name = infoTag.attribute("name").as_string();
				meta.m_info.description = infoTag.attribute("description").as_string();
				meta.m_info.type = infoTag.attribute("type").as_string();
				meta.m_info.gamemodes = Utils::stringSplit(
					infoTag.attribute("gamemodes").as_string(), ","
				);
				continue;
			}
			if (!strcmp(nodeName, "script")) {
				MetaScript script;
				script.source = node.attribute("src").as_string();
				script.cache = node.attribute("cache").as_bool();
				script.type = ScriptTypeFromString(node.attribute("type").as_string());
				script.validate = node.attribute("validate").as_bool();

				meta.m_scripts.push_back(script);
				continue;
			}
			if (!strcmp(nodeName, "map")) {
				MetaMap map;
				map.source = node.attribute("src").as_string();
				map.dimension = Utils::Clamp(
					node.attribute("dimension").as_uint(),
					Utils::ToUINT32(0),
					Utils::ToUINT32(USHRT_MAX)
				);

				meta.m_maps.push_back(map);
				continue;
			}
			if (!strcmp(nodeName, "file")) {
				MetaFile file;
				file.source = node.attribute("src").as_string();
				file.download = node.attribute("download").as_bool();

				meta.m_files.push_back(file);
				continue;
			}
			if (!strcmp(nodeName, "include")) {
				MetaInclude include;
				include.resource = node.attribute("resource").as_string();
				include.minversion = node.attribute("minversion").as_string();
				include.maxversion = node.attribute("maxversion").as_string();

				meta.m_includes.push_back(include);
				continue;
			}
			if (!strcmp(nodeName, "config")) {
				MetaConfig config;
				config.source = node.attribute("source").as_string();
				config.type = ScriptTypeFromString(node.attribute("type").as_string());

				meta.m_config = config;
				continue;
			}
			if (!strcmp(nodeName, "export")) {
				MetaExport exportFunc;
				exportFunc.function = node.attribute("function").as_string();
				exportFunc.http = node.attribute("http").as_string();
				exportFunc.type = ScriptTypeFromString(node.attribute("type").as_string());

				meta.m_exports.push_back(exportFunc);
				continue;
			}
			if (!strcmp(nodeName, "html")) {
				MetaHTML html;
				html.source = node.attribute("source").as_string();
				html.raw = node.attribute("raw").as_bool();
				html.defaultFile = node.attribute("defaultFile").as_bool();

				meta.m_html.push_back(html);
				continue;
			}
			if (!strcmp(nodeName, "settings")) {
				if (!meta.m_settings.empty())
					continue;
				for (const auto& child : node.children()) {
					if (child.name() != "setting")
						break;

					MetaSetting setting;
					setting.name = child.attribute("name").as_string();
					setting.value = child.attribute("value").as_string();
					setting.friendlyName = child.attribute("friendlyName").as_string();
					setting.accept = child.attribute("accept").as_string();
					setting.examples = child.attribute("examples").as_string();
					setting.description = child.attribute("description").as_string();

					meta.m_settings.push_back(setting);
				}
				continue;
			}
			if (!strcmp(nodeName, "min_mta_version")) {
				MetaMinVersion minversion;
				minversion.client = node.attribute("client").as_string();
				minversion.server = node.attribute("server").as_string();

				meta.m_minVersion = minversion;
				continue;
			}
			if (!strcmp(nodeName, "aclrequest")) {
				for (const auto& child : node.children()) {
					if (child.name() != "right")
						break;

					MetaACLRight right;
					right.name = child.attribute("name").as_string();
					right.access = child.attribute("access").as_bool();

					meta.m_aclRequests.push_back(right);
				}
				continue;
			}
			if (!strcmp(nodeName, "sync_map_element_data")) {
				meta.m_syncElementData =
					Utils::stringLower(node.value()) == "true" ? true : false;
				continue;
			}
			if (!strcmp(nodeName, "oop")) {
				meta.m_oop =
					Utils::stringLower(node.value()) == "true" ? true : false;
				continue;
			}
			if (!strcmp(nodeName, "download_priority_group")) {
				meta.m_downloadPriority = std::atoi(node.value());
				continue;
			}
		}

		return meta;
	}

	std::string Meta::ToString() const noexcept {
		const auto author = std::format("\t<info name='{}' type='{}' "
			"version='{}' author='{}' description='{}' gamemodes='{}' />\n",
			m_info.name, m_info.type, m_info.version, m_info.author,
			m_info.description, Utils::Join(m_info.gamemodes, ",")
		);

		std::string script = "";
		for (const auto& scriptFile : m_scripts) {
			auto scriptFormat = std::format(
				"<script src='{}' type='{}' cache='{}' validate='{}' />",
				scriptFile.source, ScriptTypeToString(scriptFile.type),
				scriptFile.cache, scriptFile.validate
			);
			script = std::format("{}\n\t{}", script, scriptFormat);
		}

		std::string map = "";
		for (const auto& mapFile : m_maps) {
			auto mapFormat = std::format(
				"<map src='{}' dimension='{}' />",
				mapFile.source, mapFile.dimension
			);
			map = std::format("{}\n\t{}", map, mapFormat);
		}
		if (map != "") map += "\n";

		std::string file = "";
		for (const auto& fileObj : m_files) {
			auto fileFormat = std::format(
				"<file src='{}' download='{}' />",
				fileObj.source, fileObj.download
			);
			file = std::format("{}\n\t{}", file, fileFormat);
		}
		if (file != "") file += "\n";

		std::string include = "";
		for (const auto& includeFunc : m_includes) {
			auto includeFormat = std::format(
				"<include resource='{}' minversion='{}' maxversion='{}' />",
				includeFunc.resource, includeFunc.minversion, includeFunc.maxversion
			);
			include = std::format("{}\n\t{}", include, includeFormat);
		}
		if (include != "") include += "\n";

		const auto config = std::format("\n\n\t<config src='{}' type='{}' />\n",
			m_config.source, ScriptTypeToString(m_config.type)
		);

		std::string exportFunc = "";
		for (const auto& exportFuncObj : m_exports) {
			auto exportFormat = std::format(
				"<export function='{}' type='{}' http='{}' />",
				exportFuncObj.function, ScriptTypeToString(exportFuncObj.type),
				exportFuncObj.http
			);
			exportFunc = std::format("{}\n\t{}", exportFunc, exportFormat);
		}
		if (exportFunc != "") exportFunc += "\n";

		std::string html = "";
		for (const auto& htmlFile : m_html) {
			auto htmlFormat = std::format(
				"<html src='{}' default='{}' raw='{}' />",
				htmlFile.source, htmlFile.defaultFile, htmlFile.raw
			);
			html = std::format("{}\n\t{}", html, htmlFormat);
		}
		if (html != "") html += "\n";

		std::string settings = "";
		for (const auto& setting : m_settings) {
			auto settingFormat = std::format(
				"<setting name='{}' value='{}' friendlyname='{}'"
				" accept='{}' examples='{}' desc='{}' />",
				setting.name, setting.value, setting.friendlyName,
				setting.accept, setting.examples, setting.description
			);
			
			settings = std::format("{}\n\t\t{}", settings, settingFormat);
		}
		settings = settings == "" ? "" : "\n\t" + settings;

		const auto minVersion = std::format("\n\t<min_mta_version "
			"client='{}' server='{}' />\n",
			m_minVersion.client, m_minVersion.server
		);

		std::string aclRequest = "";
		for (const auto& request : m_aclRequests) {
			auto requestFormat = std::format(
				"<right name='{}' access='{}' />",
				request.name, request.access
			);

			aclRequest = std::format("{}\n\t\t{}", aclRequest, requestFormat);
		}
		aclRequest = aclRequest == "" ? "" : "\n\t" + aclRequest;

		const auto syncElementData = std::format(
			"\n\t<sync_map_element_data>{}</sync_map_element_data>\n",
			m_syncElementData
		);

		const auto oop = std::format(
			"\t<oop>{}</oop>\n",
			m_oop
		);

		const auto downloadPriority = std::format(
			"\t<download_priority_group>{}</download_priority_group>\n",
			m_downloadPriority
		);

		return std::format(
			"<meta>\n"
			"{}"
			"{}"
			"{}"
			"{}"
			"{}"
			"{}"
			"{}"
			"{}"
			"\n\t<settings>{}</settings>\n"
			"{}"
			"\n\t<aclrequest>{}</aclrequest>\n"
			"{}"
			"{}"
			"{}"
			"</meta>", author, script, map, file, include, config, exportFunc,
			html, settings, minVersion, aclRequest, syncElementData, oop, downloadPriority
		);
	}

	void Meta::ToFile(const char* path) const noexcept {
		//std::
		
		ToString();
	}
}