#############################################################################
# Name:        samples/minimal/CMakeListst.txt
# Purpose:     Sample CMake file to show usage of cmake for wxWidgets users
# Author:      Tobias Taschner
# Created:     2016-10-23
# Copyright:   (c) 2016 wxWidgets development team
# Licence:     wxWindows licence
#############################################################################

#
# This file is just a sample to show using cmake from an application
# If you want to build the minimal and other samples please use the
# wxBUILD_SAMPLES option when using cmake on the library
#

# Declare the minimum required CMake version
cmake_minimum_required(VERSION 2.8.12)

# Name the project
project(samples)

# Request the required wxWidgets libs
find_package(wxWidgets 3.1 COMPONENTS core base ribbon webview net html aui stc REQUIRED)
include(${wxWidgets_USE_FILE})
add_compile_definitions(WXUSINGDLL)

# --------------------------------------------------------------------------
# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
if(WIN32)
    if(NOT BORLAND)
        if(NOT CYGWIN)
            if(NOT MINGW)
                if(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
                    add_definitions(
                    -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
                    -D_CRT_IS_WCTYPE_NO_DEPRECATE
                    -D_CRT_MANAGED_FP_NO_DEPRECATE
                    -D_CRT_NONSTDC_NO_DEPRECATE
                    -D_CRT_SECURE_NO_DEPRECATE
                    -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
                    -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
                    -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
                    -D_CRT_VCCLRIT_NO_DEPRECATE
                    -D_SCL_SECURE_NO_DEPRECATE
                    )
                endif()
                add_definitions(-DNOMINMAX)
            endif()
        endif()
    endif()
add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

add_subdirectory(ribbon)
add_subdirectory(webview)
add_subdirectory(wizard)
add_subdirectory(aui)
add_subdirectory(artprov)


