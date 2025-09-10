qt_add_library(businesslogic STATIC
    businesslogic.cpp
    businesslogic.h
)

target_link_libraries(businesslogic PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql data)
target_include_directories(businesslogic INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
