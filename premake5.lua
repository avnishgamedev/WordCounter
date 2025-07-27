workspace "WordCounter"
   configurations { "Debug", "Release" }
   architecture "x64"
   cppdialect "C++20"
   location "solution"

project "WordCounter"
   kind "WindowedApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "bin-int/%{cfg.buildcfg}"
   debugdir "bin/%{cfg.buildcfg}"

   files { "**.h", "**.cpp" }

   includedirs { "WordCounter/Gui/ThirdParty/ImGui", "WordCounter/Gui/ThirdParty/ImGui/backends" }

   removefiles {
      "WordCounter/Gui/ThirdParty/ImGui/misc/**",
      "WordCounter/Gui/ThirdParty/ImGui/examples/**",
      "WordCounter/Gui/ThirdParty/ImGui/backends/**",
      "WordCounter/Gui/Platform/**/**" -- Exclude everything in any sub-folder of Platform, keep Platform.h and Platform.cpp
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "RELEASE" }
      optimize "On"
      
   filter "system:windows"
      defines { "PLATFORM_WINDOWS "}
      links { "d3d9" }
      files { "WordCounter/Gui/Platform/Windows/**" }
      files { 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_dx9.h", 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_win32.h",
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_dx9.cpp", 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_win32.cpp"
      }
      postbuildcommands {
         "{COPYDIR} ../WordCounter/Assets %{cfg.targetdir}/Assets"
      }

   filter "system:macosx"
      defines { "PLATFORM_MAC "}
      links {
         "Cocoa.framework",
         "GameController.framework",
         "Metal.framework",
         "MetalKit.framework"
      }
      files { "WordCounter/Gui/Platform/Mac/**" }
      files { 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_metal.h", 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_osx.h",
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_metal.mm", 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_osx.mm"
      }
      prebuildcommands {
         "mkdir -p ${BUILT_PRODUCTS_DIR}/${PRODUCT_NAME}.app/Contents/Resources/Assets",
         "cp -rf ../WordCounter/Assets/* ${BUILT_PRODUCTS_DIR}/${PRODUCT_NAME}.app/Contents/Resources/Assets/"
      }
      xcodebuildsettings {
         ["INFOPLIST_FILE"] = "$(SRCROOT)/../WordCounter/Gui/Platform/Mac/Info-macOS.plist"
     }
     filter "system:linux"
      defines { "PLATFORM_LINUX "}
      links {
         "X11",
         "GL",
         "glfw",
         "Xext"
      }
      files { "WordCounter/Gui/Platform/Linux/**" }
      files { 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_opengl3.h", 
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_opengl3.cpp",
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_glfw.h",
         "WordCounter/Gui/ThirdParty/ImGui/backends/imgui_impl_glfw.cpp"
      }
      postbuildcommands {
         "{COPYDIR} ../WordCounter/Assets %{cfg.targetdir}/Assets"
      }
     