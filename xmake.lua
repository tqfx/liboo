-- set project name
set_project("oo")

-- set xmake minimum version
set_xmakever("2.5.0")

-- set project version
set_version("0.1.0", {build = "%Y%m%d%H%M"})

-- set language: c11 c++17
set_languages("c11", "c++17")

-- set warning everything
set_warnings("everything")

-- disable some compiler errors
if is_plat("windows") then
    add_cxflags("/wd4514", "/wd4710", "/wd4711", "/wd5039", "/wd5045")
    add_defines("_CRT_SECURE_NO_WARNINGS")
end
add_cxflags("-Wno-reserved-identifier", "-Wno-used-but-marked-unused")
add_cflags("-Wno-declaration-after-statement")
add_cxxflags("-Wno-c++98-compat-pedantic")

-- add build modes
add_rules("mode.check", "mode.debug", "mode.release")
if is_mode("check") and not is_plat("mingw") then
    local flags = {
        "-fsanitize=address,undefined",
        "-fsanitize-recover=address",
        "-fno-omit-frame-pointer",
        "-fsanitize=leak",
    }
    add_cxflags(flags)
    add_ldflags(flags)
end

target("oo")
    set_kind("$(kind)")
    add_files("src/o_*_o.c")
target_end()

-- include test sources
includes("test")
