-- premake5.lua
workspace "ExecutorApp"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ExecutorApp"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vendor/Walnut/Build-Walnut-External.lua"
include "ExecutorApp/Build-Executor-App.lua"