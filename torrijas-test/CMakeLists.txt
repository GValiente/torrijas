set(SRC_LIST
    source/main.cpp
    include/actionstest.h
    source/actionstest.cpp
    include/boundingboxtest.h
    source/boundingboxtest.cpp
    include/eyesbenchmark.h
    source/eyesbenchmark.cpp
    include/filestest.h
    source/filestest.cpp
    include/framebuffertest.h
    source/framebuffertest.cpp
    include/imagestest.h
    source/imagestest.cpp
    include/keyboardtest.h
    source/keyboardtest.cpp
    include/linestest.h
    source/linestest.cpp
    include/mousetest.h
    source/mousetest.cpp
    include/test.h
    source/test.cpp
    include/texttest.h
    source/texttest.cpp
    include/torrijotest.h
    source/torrijotest.cpp
    include/transformationstest.h
    source/transformationstest.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/torrijas/include)
include_directories(include)

add_executable(torrijas-test ${SRC_LIST})
target_link_libraries(torrijas-test torrijas)
