cmake_minimum_required(VERSION 3.16)
project(Dictionary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# <-- DÒNG QUAN TRỌNG NHẤT: THÊM Widgets!!!
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_add_executable(Dictionary
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    logindialog.cpp
    logindialog.h
    logindialog.ui

    user.cpp
    user.h
)

# <-- THÊM DÒNG NÀY ĐỂ LINK Widgets CHẮC CHẮN 100%
target_link_libraries(Dictionary PRIVATE Qt6::Widgets Qt6::Core)

qt_finalize_executable(Dictionary)
