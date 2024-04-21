cmake_minimum_required(VERSION 3.15.0)

project(server LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
add_subdirectory(QOnlineTranslator)
find_package(Qt5 REQUIRED COMPONENTS Core)

add_executable(server
    main.cpp
    Server.cpp
    Translator.cpp
    ServerConnection.cpp
    Query.cpp
)

target_link_libraries(server PRIVATE Qt5::Core QOnlineTranslator)
