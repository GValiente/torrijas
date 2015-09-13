# Include OpenGL:
find_package(OpenGL REQUIRED)

# Find glfw:
if(WIN32)
    set(GLFW3_ROOT "C:/glfw")
    include_directories("${GLFW3_ROOT}/include")
else()
    # Default glfw search dirs:
    set(_glfw3_HEADER_SEARCH_DIRS
	"/usr/include"
	"/usr/local/include")

    set(_glfw3_LIB_SEARCH_DIRS
	"/usr/lib"
	"/usr/local/lib")

    # Check environment for glfw root search directory:
    set(_glfw3_ENV_ROOT $ENV{GLFW3_ROOT})
    if(NOT GLFW3_ROOT AND _glfw3_ENV_ROOT)
	set(GLFW3_ROOT ${_glfw3_ENV_ROOT})
    endif()

    # Put user specified location at beginning of glfw search:
    if(GLFW3_ROOT)
	list(INSERT _glfw3_HEADER_SEARCH_DIRS 0 "${GLFW3_ROOT}/include")
	list(INSERT _glfw3_LIB_SEARCH_DIRS 0 "${GLFW3_ROOT}/lib")
    endif()

    # Search for the glfw header:
    FIND_PATH(GLFW3_INCLUDE_DIR "GLFW/glfw3.h"
	PATHS ${_glfw3_HEADER_SEARCH_DIRS})

    # Search for the glfw library:
    FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw3 glfw
	PATHS ${_glfw3_LIB_SEARCH_DIRS})

    # Include glfw:
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	GLFW3 DEFAULT_MSG
	GLFW3_LIBRARY
	GLFW3_INCLUDE_DIR)
endif()

# Find glew:
if(WIN32)
    set(GLEW_ROOT "C:/glew")
    include_directories("${GLEW_ROOT}/include")
else()
    find_package(GLEW REQUIRED STATIC)
endif()

# Include NanoVG:
include_directories("nanovg/src")
FILE(GLOB NANOVG_SRC_LIST "nanovg/src/nanovg.c")

# Include stb_image_write:
include_directories(stb_image_write)

# List torrijas files:
set(SRC_LIST
    include/trjaction.h
    source/trjaction.cpp
    include/trjapplication.h
    source/trjapplication.cpp
    include/trjapplicationconfig.h
    include/trjarcshape.h
    include/trjboxgradientpen.h
    include/trjcallbackaction.h
    source/trjcallbackaction.cpp
    include/trjcolor.h
    source/trjcolor.cpp
    include/trjcolorpen.h
    include/trjcommon.h
    include/trjconfig.h
    include/trjdebug.h
    source/trjdebug.cpp
    include/trjellipseshape.h
    include/trjexception.h
    source/trjexception.cpp
    include/trjfile.h
    source/trjfile.cpp
    include/trjfolder.h
    include/trjfont.h
    source/trjfont.cpp
    include/trjimage.h
    source/trjimage.cpp
    include/trjimagedata.h
    source/trjimagedata.cpp
    include/trjimagenode.h
    source/trjimagenode.cpp
    include/trjimagepatternpen.h
    include/trjkeyboard.h
    source/trjkeyboard.cpp
    include/trjlineargradientpen.h
    include/trjlineshapes.h
    include/trjmain.h
    include/trjmouse.h
    source/trjmouse.cpp
    include/trjmoveaction.h
    source/trjmoveaction.cpp
    include/trjnode.h
    source/trjnode.cpp
    include/trjoptional.h
    include/trjpen.h
    source/trjpen.cpp
    include/trjperfgraph.h
    source/trjperfgraph.cpp
    include/trjpoint.h
    source/trjpoint.cpp
    include/trjptr.h
    include/trjradialgradientpen.h
    include/trjrect.h
    source/trjrect.cpp
    include/trjrectshape.h
    include/trjrendercontext.h
    include/trjrepeataction.h
    source/trjrepeataction.cpp
    include/trjrotateaction.h
    source/trjrotateaction.cpp
    include/trjscaleaction.h
    source/trjscaleaction.cpp
    include/trjsequenceaction.h
    source/trjsequenceaction.cpp
    include/trjshape.h
    source/trjshape.cpp
    include/trjshapegroup.h
    source/trjshapegroup.cpp
    include/trjsize.h
    source/trjsize.cpp
    include/trjstring.h
    source/trjstring.cpp
    include/trjtextnode.h
    source/trjtextnode.cpp
    include/trjtriangleshape.h
    include/trjwaitaction.h
    source/trjwaitaction.cpp
    include/private/trjimagemanager.h
    source/private/trjimagemanager.cpp
    include/private/trjdisplaylistmanager.h
    source/private/trjdisplaylistmanager.cpp
)

# Build torrijas:
include_directories(include)
add_library(torrijas ${NANOVG_SRC_LIST} ${SRC_LIST})

# Link required libraries:
if(WIN32)
    target_link_libraries(torrijas
	${OPENGL_LIBRARIES}
	${GLFW3_ROOT}/lib/glfw3.lib
	${GLEW_ROOT}/lib/Release/x64/glew32s.lib
	glu32
	opengl32)
endif(WIN32)

if(UNIX)
    target_link_libraries(torrijas
	${OPENGL_LIBRARIES}
	${GLFW3_LIBRARY}
	${GLEW_LIBRARY})
endif(UNIX)
