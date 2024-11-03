# rm -rf build
# cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
# cmake --build build
# for LFU tests:
# cmake --build ./build --target test ARGS="-C LFU"
# for PC tests:
# cmake --build ./build --target test ARGS="-C PC"


cmake_minimum_required(VERSION 3.11)

project(HWcache)
set(LFU_SRC ${CMAKE_SOURCE_DIR}/LFU/src/LFU.cpp)
set(PC_SRC ${CMAKE_SOURCE_DIR}/PC/src/perfect_cache.cpp)

set(LFU_exec LFU.out)
add_executable(${LFU_exec})
target_sources(${LFU_exec} PRIVATE ${LFU_SRC})
target_compile_features(${LFU_exec} PUBLIC cxx_std_11)
target_include_directories(${LFU_exec} PUBLIC ${CMAKE_SOURCE_DIR}/LFU/include)

set(PC_exec PC.out)
add_executable(${PC_exec})
target_sources(${PC_exec} PRIVATE ${PC_SRC})
target_compile_features(${PC_exec} PUBLIC cxx_std_11)
target_include_directories(${PC_exec} PUBLIC ${CMAKE_SOURCE_DIR}/PC/include)

include(CTest)
enable_testing()

file(GLOB testfiles "${CMAKE_SOURCE_DIR}/test/*.dat")
foreach(file ${testfiles})
    add_test (NAME LFU:${file}
        CONFIGURATIONS LFU
        COMMAND bash ${CMAKE_SOURCE_DIR}/test/test.sh ${CMAKE_SOURCE_DIR} ${file})
    set_tests_properties(LFU:${file} PROPERTIES DEPENDS ${LFU_exec})
endforeach()

foreach(file ${testfiles})
    add_test(NAME PC:${file}
        CONFIGURATIONS PC
        COMMAND bash ${CMAKE_SOURCE_DIR}/test/perfect_cache.sh ${CMAKE_SOURCE_DIR} ${file})
    set_tests_properties(PC:${file} PROPERTIES DEPENDS ${PC_exec})
endforeach()
