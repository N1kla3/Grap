cmake_minimum_required(VERSION 3.16)

project(grap LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        source/arrow.cpp
        source/graph.cpp
        source/mainwindow.cpp
        source/mininode.cpp
        source/node.cpp
        source/paintscene.cpp
        source/unarrow.cpp
        source/BaseArrow.cpp
        )

set(HEADER_FILES
        header/arrow.h
        header/graph.h
        header/mainwindow.h
        header/mininode.h
        header/node.h
        header/paintscene.h
        header/unarrow.h
        header/BaseArrow.h
        )


find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

qt5_wrap_ui(QT_TEST_UI_HEADERS  source/mainwindow.ui)


    add_executable(grap
            main.cpp
            ${SOURCE_FILES}
            ${HEADER_FILES}
            resource.qrc
            ${QT_TEST_UI_HEADERS}
            )

target_link_libraries(grap PRIVATE Qt5::Widgets Qt5::Core)
