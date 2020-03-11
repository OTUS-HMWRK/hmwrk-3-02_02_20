cmake_minimum_required(VERSION 3.12...3.16)

project( version_lib
    VERSION 0.0.${TRAVIS_BUILD_NUMBER}
    DESCRIPTION "hmwrk-1-23_12_19"
    HOMEPAGE_URL "https://otus.ru/lessons/razrabotchik-cpp/?int_source=courses_catalog&int_term=programming"
    LANGUAGES CXX
)

#find_package(Boost COMPONENTS unit_test_framework REQUIRED)

option(PROJECT_VERSION_PATCH ON)

configure_file(
    ${PROJECT_SOURCE_DIR}/../../conf/version/cmake_values.h.in 
    ${PROJECT_SOURCE_DIR}/../../include/version/cmake_values.h
    )

add_library(
    version 
    version.cpp 
    ${PROJECT_SOURCE_DIR}/../../include/version/cmake_values.h 
    ${PROJECT_SOURCE_DIR}/../../include/version/version.h)

set_target_properties(
    version 
    PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
target_include_directories(version
    PRIVATE ${PROJECT_SOURCE_DIR}/../../include/version
)

if (MSVC)
    target_compile_options(version PRIVATE
        /W4
    )

else ()
    target_compile_options(version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

endif()
