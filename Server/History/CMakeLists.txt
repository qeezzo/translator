find_package(Qt5 REQUIRED COMPONENTS Core)

add_library(
  his STATIC

  History.h
  History.cpp

  Tables/Engine.h
  Tables/Engine.cpp
  Tables/History.h
  Tables/History.cpp
  Tables/Language.h
  Tables/Language.cpp
  Tables/Translation.h
  Tables/Translation.cpp

  Tables/Example/Example.h
  Tables/Example/Example.cpp
  Tables/Example/ExampleType.h
  Tables/Example/ExampleType.cpp

  Tables/Option/Option.h
  Tables/Option/Option.cpp
  Tables/Option/OptionType.h
  Tables/Option/OptionType.cpp
  Tables/Option/OptionTranslations.h
  Tables/Option/OptionTranslations.cpp
)

target_include_directories(
  his
  PUBLIC .
  /usr/include/mysql
  /usr/include/mysql++
)
target_link_libraries(his PUBLIC mysqlpp)
