cmake_minimum_required(VERSION 3.31)

project(aom-sd-controller-fix)

if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    message(FATAL_ERROR "[${PROJECT_NAME}] Make sure to instruct cmake to use the toolchain file!")
endif()

include(FetchContent)
FetchContent_Declare(lime
  GIT_REPOSITORY https://github.com/Curve/lime.git
  GIT_TAG        v6.0)
FetchCOntent_Declare(spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.15.1)
set(SPDLOG_USE_STD_FORMAT OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(lime spdlog)

add_library(${PROJECT_NAME} SHARED main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "VERSION")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if (MINGW)
  target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,--enable-stdcall-fixup")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE cr::lime spdlog::spdlog)
lime_mingw_generate_proxy(${PROJECT_NAME} "exports.lime")
