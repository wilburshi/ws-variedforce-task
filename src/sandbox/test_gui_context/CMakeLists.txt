project(test_gui_context)

add_executable(${PROJECT_NAME}
        main.cpp
        training.hpp
        training.cpp)
target_link_libraries(${PROJECT_NAME} ws)
target_compile_definitions(${PROJECT_NAME} PRIVATE
        WS_RES_DIR="${PROJECT_SOURCE_DIR}/../../../res"
        WS_DATA_DIR="${PROJECT_SOURCE_DIR}/../../../data")