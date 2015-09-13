set(SRC_LIST
    source/main.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/torrijas/include)
include_directories(include)

add_executable(torrijas-template ${SRC_LIST})
target_link_libraries(torrijas-template torrijas)
