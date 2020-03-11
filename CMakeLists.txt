cmake_minimum_required(VERSION 3.12...3.16)

set(
    MY_PROJECT_NAME
        HelloWorld   
)
set(
    DESCRIPTION
        hmwrk-3-02_02_20    
)

set(
    GIT_USER
        OTUS-HMWRK
)

if (NOT DEFINED $ENV{TRAVIS_BUILD_NUMBER})
    set(
        TRAVIS_BUILD_NUMBER
            0
    )
else()
    set(
        TRAVIS_BUILD_NUMBER 
            $ENV{TRAVIS_BUILD_NUMBER}
    )
endif()

set(
    CMAKE_TOOLCHAIN_FILE
    $ENV{VCPKG_PATH}
)

set(
    CMAKE_BUILD_TYPE
    Debug
)

message("$ENV{VCPKG_PATH}")

project( ${MY_PROJECT_NAME}
    VERSION 0.0.${TRAVIS_BUILD_NUMBER}
    DESCRIPTION "hmwrk-3-02_02_20"
    HOMEPAGE_URL "https://otus.ru/lessons/razrabotchik-cpp/?int_source=courses_catalog&int_term=programming"
    LANGUAGES CXX
)

add_subdirectory(lib/version)
add_subdirectory(app)
add_subdirectory(tests/boost)

option(PROJECT_VERSION_PATCH ON)

configure_file(
    ${PROJECT_SOURCE_DIR}/conf/git_create_repo.txt.in 
    ${PROJECT_SOURCE_DIR}/conf/git_create_repo.txt)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")