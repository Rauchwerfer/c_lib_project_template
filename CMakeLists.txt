cmake_minimum_required(VERSION 3.30.5)
project(examplec C) 

# Set C standard to C17
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detect compiler name
if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    set(COMPILER_NAME "MSVC")
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(COMPILER_NAME "MinGW")
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(COMPILER_NAME "Clang")
else()
    set(COMPILER_NAME "UnknownCompiler")
endif()

message(STATUS "OS: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
message(STATUS "CMAKE_C_STANDARD: ${CMAKE_C_STANDARD}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")


file(GLOB_RECURSE LIB_API "inc/*.h" )
file(GLOB_RECURSE LIB_SOURCES "src/*.c")
file(GLOB_RECURSE LIB_HEADERS "src/*.h")

message(STATUS "Lib Public (include) Headers files: ${LIB_API}")
message(STATUS "Lib Private Headers files: ${LIB_HEADERS}")
message(STATUS "Lib Sources files: ${LIB_SOURCES}")

file(GLOB_RECURSE TEST_APP_SOURCES "test_app_src/*.c")
file(GLOB_RECURSE TEST_APP_HEADERS "test_app_src/*.h")

message(STATUS "Test app Headers files: ${TEST_APP_HEADERS}")
message(STATUS "Test app Sources files: ${TEST_APP_SOURCES}")


# Options to enable/disable building static/shared libs
option(BUILD_STATIC_LIB "Build static library" ON)
option(BUILD_STATIC_LIB_TEST_APP "Build static library test app" ON)

option(BUILD_SHARED_LIB "Build shared library" ON)
option(BUILD_SHARED_LIB_TEST_APP "Build shared library test app" ON)

# Set output directories
set(SHIP_DIR ${CMAKE_SOURCE_DIR}/output/${COMPILER_NAME})

set(STATIC_LIB_DIR ${SHIP_DIR}/static)
set(SHARED_LIB_DIR ${SHIP_DIR}/shared)

set(STATIC_LIB_TEST_APP_DIR ${SHIP_DIR}/static_test_app)
set(SHARED_LIB_TEST_APP_DIR ${SHIP_DIR}/shared_test_app)



# === Static Library ===
if (BUILD_STATIC_LIB)
    # Ensure output directory exist
    file(MAKE_DIRECTORY ${STATIC_LIB_DIR})

    add_library(${PROJECT_NAME}_static STATIC ${LIB_API} ${LIB_SOURCES} ${LIB_HEADERS})

    target_compile_definitions(${PROJECT_NAME}_static PRIVATE EXAMPLEC_STATIC_DEFINE)

    target_include_directories(${PROJECT_NAME}_static
            PUBLIC  ${CMAKE_SOURCE_DIR}/inc
            PRIVATE ${CMAKE_SOURCE_DIR}/src
        )

    set_target_properties(${PROJECT_NAME}_static PROPERTIES 
            LIBRARY_OUTPUT_DIRECTORY ${STATIC_LIB_DIR} 
            RUNTIME_OUTPUT_DIRECTORY ${STATIC_LIB_DIR} 
            ARCHIVE_OUTPUT_DIRECTORY ${STATIC_LIB_DIR}
        )
    
    if (MSVC) 
        set_property(TARGET ${PROJECT_NAME}_static PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()


    if (BUILD_STATIC_LIB_TEST_APP)
        add_executable(${PROJECT_NAME}_static_test_app ${TEST_APP_SOURCES} ${TEST_APP_HEADERS})

        target_compile_definitions(${PROJECT_NAME}_static_test_app PRIVATE EXAMPLEC_STATIC_DEFINE)

        target_include_directories(${PROJECT_NAME}_static_test_app
            PRIVATE ${CMAKE_SOURCE_DIR}/inc
        )

        target_link_libraries(${PROJECT_NAME}_static_test_app PRIVATE ${PROJECT_NAME}_static)

        set_target_properties(${PROJECT_NAME}_static_test_app PROPERTIES 
            LIBRARY_OUTPUT_DIRECTORY ${STATIC_LIB_TEST_APP_DIR} 
            RUNTIME_OUTPUT_DIRECTORY ${STATIC_LIB_TEST_APP_DIR} 
            ARCHIVE_OUTPUT_DIRECTORY ${STATIC_LIB_TEST_APP_DIR}
        )

        if (MSVC) 
            set_property(TARGET ${PROJECT_NAME}_static_test_app PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
        endif()
    endif()

endif()


# === Shared Library ===
if (BUILD_SHARED_LIB)
    # Ensure output directory exist
    file(MAKE_DIRECTORY ${SHARED_LIB_DIR})

    add_library(${PROJECT_NAME}_shared SHARED ${LIB_API} ${LIB_SOURCES} ${LIB_HEADERS})

    target_compile_definitions(${PROJECT_NAME}_shared PRIVATE EXAMPLEC_EXPORTS)

    target_include_directories(${PROJECT_NAME}_shared
            PUBLIC  ${CMAKE_SOURCE_DIR}/inc
            PRIVATE ${CMAKE_SOURCE_DIR}/src
        )
    
    set_target_properties(${PROJECT_NAME}_shared PROPERTIES 
            LIBRARY_OUTPUT_DIRECTORY ${SHARED_LIB_DIR} 
            RUNTIME_OUTPUT_DIRECTORY ${SHARED_LIB_DIR}  
            ARCHIVE_OUTPUT_DIRECTORY ${SHARED_LIB_DIR} 
        ) 
        
    if (MSVC)
        message(STATUS "Found MSVC compiler of version ${MSVC_VERSION}: parameters for shared library: /W4")
        target_compile_options(${PROJECT_NAME}_shared PRIVATE /W4)  # Equivalent to -Wall but stricter
    else()
        message(STATUS "Found GNU or Clang compiler: parameters for shared library: -Wall")
        target_compile_options(${PROJECT_NAME}_shared PRIVATE -Wall)
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # Equivalent to -fPIC
    endif()

    if (MSVC) 
        set_property(TARGET ${PROJECT_NAME}_shared PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()

    if (BUILD_SHARED_LIB_TEST_APP)
        add_executable(${PROJECT_NAME}_shared_test_app ${TEST_APP_SOURCES} ${TEST_APP_HEADERS})

        target_include_directories(${PROJECT_NAME}_shared_test_app
            PRIVATE ${CMAKE_SOURCE_DIR}/inc
        )

        target_link_libraries(${PROJECT_NAME}_shared_test_app PRIVATE ${PROJECT_NAME}_shared)

        set_target_properties(${PROJECT_NAME}_shared_test_app PROPERTIES 
            LIBRARY_OUTPUT_DIRECTORY ${SHARED_LIB_TEST_APP_DIR} 
            RUNTIME_OUTPUT_DIRECTORY ${SHARED_LIB_TEST_APP_DIR} 
            ARCHIVE_OUTPUT_DIRECTORY ${SHARED_LIB_TEST_APP_DIR}
        )

        # Post-build copy of shared lib to test app dir
        # add_custom_command(TARGET examplec_shared POST_BUILD
        #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #         "$<TARGET_FILE:examplec_shared>"
        #         "${SHARED_LIB_TEST_APP_DIR}"
        #     COMMENT "Copying shared library to test app directory"
        # )
    endif()

endif()


