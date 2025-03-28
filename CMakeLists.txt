cmake_minimum_required(VERSION 3.10)

project(Asynco)

# Postavi verziju projekta
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pronađi Boost biblioteku (ako nije uobičajeni direktorijum, postavi put)
find_package(Boost REQUIRED COMPONENTS system)

# Dodaj direktorijume sa zaglavljima
include_directories(lib)

# Dodaj biblioteku
add_library(asynco STATIC
    src/engine.cpp
    src/timers.cpp
)

# Linkaj Asynco biblioteku sa Boost-om
target_link_libraries(asynco Boost::system)

# Dodaj testove
add_subdirectory(test)


# Instaliraj biblioteku
# install(TARGETS asynco DESTINATION lib)
# install(FILES lib/asynco.hpp lib/define.hpp lib/engine.hpp lib/filesystem.hpp lib/timers.hpp lib/trigger.hpp DESTINATION include/asynco)
# 