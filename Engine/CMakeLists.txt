cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17) # Enable c++17 standard
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/") # add custom cmake modules

set(BUILD_FOLDER_PATH "${CMAKE_SOURCE_DIR}//..//")

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR "${BUILD_FOLDER_PATH}/Sandbox/")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

project(Engine_${PROJECT_TARGET}) # Project's name

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Source/Public)

SET( CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "/NODEFAULTLIB:libcmt" )

add_executable(${PROJECT_NAME} Source/AegisEngineStartup.cpp)

find_package(ExternalLibraries)

set(APWINNT_LIBS

  winmm.lib

  imm32.lib

  Version.lib
)

target_link_libraries(${PROJECT_NAME} ${APWINNT_LIBS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if (MSVC)
		add_compile_options("/std:c++latest")
endif()

# Add Each Library in ALL_LIBRARIES List to Project
foreach(LIBRARY ${LOCAL_LIBRARIES})
    message(STATUS ${EXTERNAL_LIBRARY_DIRECTORIES}/${LIBRARY})

    target_link_libraries(
            ${PROJECT_NAME}
                ${EXTERNAL_LIBRARY_DIRECTORIES}/${LIBRARY}
    )
endforeach()

foreach(LIBRARY "${SYSTEM_LIBRARIES}")
    message(STATUS "${LIBRARY}")

    target_link_libraries(
            ${PROJECT_NAME}
                "${LIBRARY}"
    )
endforeach()

foreach(LIBRARY "${WINDOWS_LIBRARIES}")
    message(STATUS "${LIBRARY}")

    target_link_libraries(
            ${PROJECT_NAME}
                "${LIBRARY}"
    )
endforeach()

foreach(INCLUDE "${ALL_INCLUDES}")
    message(STATUS "${INCLUDE}")

    target_include_directories(
            ${PROJECT_NAME}
                PUBLIC
                    "${INCLUDE}"
    )
endforeach()

target_include_directories(
        ${PROJECT_NAME}
            PRIVATE
                "${PROJECT_INCLUDE_DIR}"
				"${PROJECT_INCLUDE_DIR}/Models"
				"${PROJECT_INCLUDE_DIR}/Controllers"
                "${CMAKE_SOURCE_DIR}/Vendor/Public/"
        )

target_sources(
	${PROJECT_NAME}${DEBUG}
        PRIVATE
		"${CMAKE_SOURCE_DIR}/Vendor/Public/glad/glad.c"
        "${CMAKE_SOURCE_DIR}/Source/AegisEngineStartup.cpp"
        )