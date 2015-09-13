project(torrijas-project)
cmake_minimum_required(VERSION 2.8)

# Render with OpenGL 2:
add_definitions(-DTRJ_CFG_GL2)

# Render with OpenGL 3:
# add_definitions(-DTRJ_CFG_GL3)

# Render with OpenGL ES 2. Not tested, so it shouldn't work:
# add_definitions(-DTRJ_CFG_GLES3)

# Render with OpenGL ES 3. Not tested, so it shouldn't work:
# add_definitions(-DTRJ_CFG_GLES2)

# Init GLEW at startup:
add_definitions(-DTRJ_CFG_ENABLE_GLEW)

# Enable NanoVG display lists. Disable it if you see something weird:
add_definitions(-DTRJ_CFG_ENABLE_RENDER_CACHE)

# Show compiler output:
# set(CMAKE_VERBOSE_MAKEFILE ON)

# Enable Clang static analyzer on debug mode:
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --analyze")

# Add C++11 support:
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
elseif(CMAKE_BUILD_TOOL MATCHES "make")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

# Replace make -O3 with -O2:
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
elseif(CMAKE_BUILD_TOOL MATCHES "make")
    string(REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "-O3" "-O2" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
endif()

# Set compiler warning level:
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	string(REGEX REPLACE "/W[0-4]" "/W1" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1")
    endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
	string(REGEX REPLACE "/W[0-4]" "/W1" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    else(CMAKE_C_FLAGS MATCHES "/W[0-4]")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W1")
    endif(CMAKE_C_FLAGS MATCHES "/W[0-4]")
elseif(CMAKE_BUILD_TOOL MATCHES "make")
    if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wall")
    if(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    endif(NOT CMAKE_CXX_FLAGS MATCHES "-Wextra")
    if(NOT CMAKE_C_FLAGS MATCHES "-Wall")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
    endif(NOT CMAKE_C_FLAGS MATCHES "-Wall")
    if(NOT CMAKE_C_FLAGS MATCHES "-Wextra")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
    endif(NOT CMAKE_C_FLAGS MATCHES "-Wextra")
endif()

# Torrijas debug flag:
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DTRJ_DEBUG")

subdirs(torrijas torrijas-test torrijas-template)
