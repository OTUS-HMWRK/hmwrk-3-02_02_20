cmake_minimum_required(VERSION 3.12...3.16)

project( test_version
    VERSION 0.0.${TRAVIS_BUILD_NUMBER}
    DESCRIPTION "hmwrk-1-23_12_19"
    HOMEPAGE_URL "https://otus.ru/lessons/razrabotchik-cpp/?int_source=courses_catalog&int_term=programming"
    LANGUAGES CXX
)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

option(PROJECT_VERSION_PATCH ON)


set(
    HEADERS_TEST
    ${PROJECT_SOURCE_DIR}/../../include/version/version.h
)
add_executable(test_version ${HEADERS_TEST} test_version.cpp )


set_target_properties(
    test_version PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK 
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(
    test_version
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/../../include/version
    ${Boost_INCLUDE_DIR}
)

target_link_libraries(
    test_version 
    PRIVATE
    ${Boost_LIBRARIES}
    version
)

if (MSVC)
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
add_test(test_version test_version)