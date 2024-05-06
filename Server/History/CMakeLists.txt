find_package(Qt5 REQUIRED COMPONENTS Core)

add_library(
  history STATIC

  history.h
  history.cpp

  Engine.h
  Engine.cpp
  History.h
  History.cpp
  Language.h
  Language.cpp
  Translation.h
  Translation.cpp
)

target_include_directories(
  history
  PUBLIC .
  /usr/include/mysql
  /usr/include/mysql++
  ../QOnlineTranslator/src
)
target_link_libraries(history PRIVATE mysqlpp)
