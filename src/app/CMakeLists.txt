qt_add_executable(StudentList
    main.cpp
    mainwindow.cpp
    studentclient.cpp
)

qt_add_resources(StudentList "app_images"
    PREFIX "/"
    FILES
        images/copy.png
        images/cut.png
        images/new.png
        images/open.png
        images/paste.png
        images/save.png
)

target_link_libraries(StudentList PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    businesslogic
)

target_link_libraries(StudentList PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
)