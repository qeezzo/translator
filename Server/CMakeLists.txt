cmake_minimum_required(VERSION 3.15.0)

project(server LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(QOnlineTranslator)
add_subdirectory(History)

find_package(Qt5 REQUIRED COMPONENTS Core)

add_executable(server
    main.cpp
    Server.cpp
    Translator.cpp
    ServerConnection.cpp
    Query.cpp
)

target_link_libraries(
  server PUBLIC
  his
  Qt5::Core
  QOnlineTranslator
)
