cmake_minimum_required(VERSION 3.15)

project(HWcache)
set(LFU_SRC ${CMAKE_SOURCE_DIR}/LFU/src/LFU.cpp)
set(PC_SRC ${CMAKE_SOURCE_DIR}/PC/src/perfect_cache.cpp)


add_executable(../LFU.out)
target_sources(../LFU.out PRIVATE ${LFU_SRC})
target_compile_features(../LFU.out PUBLIC cxx_std_11)
target_include_directories(../LFU.out PUBLIC ${CMAKE_SOURCE_DIR}/LFU/include)

add_executable(../perfect_cache)
target_sources(../perfect_cache PRIVATE ${PC_SRC})
target_include_directories(../perfect_cache PUBLIC ${CMAKE_SOURCE_DIR}/PC/include)
target_compile_features(../perfect_cache PUBLUC cxx_std_11)


