qt_add_executable(server_executable
    main.cpp
    appserver.cpp
    appserver.h
)

target_link_libraries(server_executable PRIVATE Qt6::Core Qt6::Network businesslogic)
target_include_directories(server_executable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
