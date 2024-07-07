add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_requires("wxwidgets","libxlsxwriter")

target("204-prject")
--  set_languages("c++20")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("wxwidgets","libxlsxwriter")