newoption {
    trigger = "architecture",
    value = "ARCH",
    description = "Architecture to target",
    default = "x64"
}
workspace "image-steganography"
    architecture (_OPTIONS["architecture"])
    targetdir "bin"
    configurations {
        "Debug",
        "Release"
    }
    flags {
        "MultiProcessorCompile"
    }
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
group "my code"
project "common"
    location "common"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    files {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/**.h"
    }
    includedirs {
        "%{prj.name}/internal"
    }
    sysincludedirs {
        "third-party"
    }
    pchheader "pch.h"
    pchsource "%{prj.name}/src/pch.cpp"
    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"
local function encodingproject(name)
    project (name)
        location (name)
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
        files {
            "%{prj.name}/**.cpp"
        }
        sysincludedirs {
            "common"
        }
        links {
            "common"
        }
        filter "configurations:Debug"
            symbols "On"
        filter "configurations:Release"
            optimize "On"
end
encodingproject("encoder")
encodingproject("decoder")
group ""