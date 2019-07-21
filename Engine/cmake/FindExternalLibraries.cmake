# Find Athena External Libraries

# Set up paths
set(EXTERNAL_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Vendor/Public")

find_package(OpenGL REQUIRED)

find_package(Threads)

set(THREADS_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

set(LOCAL_LIBRARIES_TEMP SDL2main SDL2 glfw3 freetype lua5.1) # assimp soil2

set(
        SYSTEM_LIBRARIES
			${OPENGL_LIBRARIES}
            ${THREADS_LIBRARIES}
            ${CMAKE_DL_LIBS}
        )

if (UNIX AND NOT APPLE) # for Linux, BSD, Solaris, Minix
        set(EXTERNAL_LIBRARY_DIRECTORIES "${CMAKE_SOURCE_DIR}/Vendor/Libraries/Linux")
        set(LIB_EXTENSION ".a")
elseif (MSVC) # Microsoft compiler, both 32 and 64 bit builds
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8) # win64
                set(EXTERNAL_LIBRARY_DIRECTORIES "${CMAKE_SOURCE_DIR}/Vendor/Libraries/Win64")
        else () # win32
                set(EXTERNAL_LIBRARY_DIRECTORIES "${CMAKE_SOURCE_DIR}/Vendor/Libraries/Win32")
        endif ()
        set(LIB_EXTENSION ".lib")
endif()

string(STRIP "${SYSTEM_LIBRARIES}" SYSTEM_LIBRARIES)
string(STRIP "${LOCAL_LIBRARIES}" LOCAL_LIBRARIES)
string(STRIP "${ALL_INCLUDES}" ALL_INCLUDES)

foreach(LIBRARY ${LOCAL_LIBRARIES_TEMP})
        list(APPEND LOCAL_LIBRARIES ${LIBRARY}${LIB_EXTENSION})
        message(STATUS ${LIBRARY}${LIB_EXTENSION})
endforeach()

set (ALL_INCLUDES
        "${EXTERNAL_INCLUDE_DIRECTORIES}"
		"${OPENGL_INCLUDE_DIRS}"
        )

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
        ExternalLibraries
            REQUIRED_VARS
                SYSTEM_LIBRARIES
                LOCAL_LIBRARIES
                ALL_INCLUDES
                EXTERNAL_LIBRARY_DIRECTORIES
        )
