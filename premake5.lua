__SolutionName = 'NRP_Module'

if _ACTION ~= "vs2022" then
    print("This project needs to be compiled for VisualStudio 2022")
end

local SOLUTION_DIR
local PLATFORM_TARGET
local CONFIGURATION
local BUILD_PATH
local INTERMEDIATE_PATH

if os.host() == 'windows' then
    SOLUTION_DIR = '$(SolutionDir)'
    PLATFORM_TARGET = '$(PlatformTarget)'
    CONFIGURATION = '$(Configuration)'
    INTERMEDIATE_PATH = '!$(OutDir)Intermediate/'
else
    SOLUTION_DIR = path.getabsolute('.')..'/'
    PLATFORM_TARGET = '%{cfg.platform}'
    CONFIGURATION = '%{cfg.buildcfg}'
    INTERMEDIATE_PATH = ('!%sIntermediate/'):format(BUILD_PATH)
end

BUILD_PATH = ('%s%s/%s/'):format(SOLUTION_DIR, PLATFORM_TARGET, CONFIGURATION)

workspace (__SolutionName)
	configurations { "Debug", "Release" }
    platforms { "x86", "x64" }

    project (__SolutionName)
        kind "SharedLib"
        cppdialect "C++20"
        cdialect "C17"
        language "C++"

        vpaths {
            ["Headers/*"] = { "hpp/**.hpp", "include/**.h", "include/**.hpp" },
            ["Sources/*"] = { "src/**.cpp", "lib/**.c", "lib/**.cpp" },
            ["*"] = "premake5.lua"
        }

        files {
            "**.c",
            "**.cpp",
            "**.h",
            "**.hpp",
        }

        targetdir (BUILD_PATH)
        objdir (INTERMEDIATE_PATH)

        filter {}
            defines { "CURL_STATICLIB" }
            editandcontinue "On"
            functionlevellinking "On"

            staticruntime "On"
            flags {
                "MultiProcessorCompile",
            }

            includedirs {
                SOLUTION_DIR.."hpp",
                SOLUTION_DIR.."include"
            }
            links { "lua5.1", "zlib" }
            ignoredefaultlibraries { "libcmt" }

        filter "platforms:x64"
            libdirs {
                SOLUTION_DIR.."lib/x64"
            }

        filter "platforms:x86"
            libdirs {
                SOLUTION_DIR.."lib/x86"
            }

        filter "configurations:Debug"
            defines { "_DEBUG" }
            symbols "On"
            optimize "Off"
            runtime "Debug"
            links { "libcurl-d" }

        filter "configurations:Release"
            defines { "NDEBUG" }
            symbols "Off"
            optimize "Full"
            runtime "Release"
            links { "libcurl" }

            flags {
                "LinkTimeOptimization",
                "NoIncrementalLink"
            }

	    filter "system:windows"
            defines { "_WIN32" }
            
            postbuildcommands {
                'xcopy /Y /I "$(OutDir)$(TargetName)$(TargetExt)" "$(SolutionDir)Build\\$(TargetName)$(TargetExt)"*'
            }
            links { "ws2_32", "crypt32" }

        filter "system:not windows"
            postbuildcommands {
                ('cp -p "%s%s" "%sBuild/%s"'):format(BUILD_PATH, __SolutionName, SOLUTION_DIR, __SolutionName)
            }