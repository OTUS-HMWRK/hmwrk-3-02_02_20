cmake_minimum_required(VERSION 3.12...3.16)

set(
    MY_PROJECT_NAME
        HelloWorld_CLI    
)

project( ${MY_PROJECT_NAME}
    VERSION 0.0.${TRAVIS_BUILD_NUMBER}
    DESCRIPTION "hmwrk-1-23_12_19"
    HOMEPAGE_URL "https://otus.ru/lessons/razrabotchik-cpp/?int_source=courses_catalog&int_term=programming"
    LANGUAGES CXX
)

option(PROJECT_VERSION_PATCH ON)

set(
    HEADERS
    ${PROJECT_SOURCE_DIR}/../include/version/version.h
)
set(
    SOURCES
        app.cpp
)


add_executable(${MY_PROJECT_NAME} ${HEADERS} ${SOURCES})

set_target_properties(
    ${MY_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${MY_PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/../include/version
)

target_link_libraries(HelloWorld_CLI PRIVATE
    version
)


message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
if (MSVC)
    target_compile_options(HelloWorld_CLI PRIVATE
        /W4
    )
else ()
    target_compile_options(HelloWorld_CLI PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS HelloWorld_CLI RUNTIME DESTINATION bin)