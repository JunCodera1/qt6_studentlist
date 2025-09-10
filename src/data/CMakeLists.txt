qt_add_library(data STATIC
    dbconnect.cpp
    dbconnect.h
)

target_link_libraries(data PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql)
target_include_directories(data INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
