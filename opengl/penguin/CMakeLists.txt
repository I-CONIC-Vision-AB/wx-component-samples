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

# Define a variable containing a list of source files for the project
set(SRC_FILES
    penguin.cpp
	dxfrenderer.cpp
	trackball.c
)

if(WIN32)
    # Include a RC file for windows
    list(APPEND SRC_FILES ../../sample.rc)
elseif(APPLE)
    # Add an icon for the apple .app file
#    list(APPEND SRC_FILES ../../src/osx/carbon/wxmac.icns)
endif()

# Define the build target for the executable
add_executable(opengl_penguin WIN32 MACOSX_BUNDLE ${SRC_FILES})

target_include_directories(opengl_penguin
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# Link required libraries to the executable
target_link_libraries(opengl_penguin ${wxWidgets_LIBRARIES} OpenGL::GL)

if(APPLE)
    set_target_properties(opengl_penguin PROPERTIES
        RESOURCE "../../src/osx/carbon/wxmac.icns"
        MACOSX_BUNDLE_ICON_FILE wxmac.icns
        MACOSX_BUNDLE_COPYRIGHT "Copyright wxWidgets"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.wxwidgets.webview"
        )
endif()
