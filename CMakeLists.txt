CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(scaffold)

SET(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS "-Wall")

ADD_DEFINITIONS(-DDEBUG)
ADD_DEFINITIONS(-DCMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}")
INCLUDE_DIRECTORIES(Dependency Header)

# SSL support
IF(ENABLE_SSL)
	ADD_DEFINITIONS(-DMG_ENABLE_SSL=1)
	FIND_PACKAGE(OpenSSL REQUIRED)
	INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
ENDIF()

SET(SOURCE_FILES
    Dependency/mongoose/mongoose.c
    Foundation/scaffold.cpp Foundation/Router.cpp
    Foundation/Request.cpp Foundation/Response.cpp
    Utility/HttpStatusMapping.cpp Utility/RequestHelper.cpp
    Utility/StringOperation.cpp Utility/ResponseHelper.cpp
    Utility/MimeTypeMapping.cpp Utility/DateTimeHelper.cpp)

#ADD_LIBRARY(scaffold_static STATIC ${SOURCE_FILES})
#SET_TARGET_PROPERTIES(scaffold_static PROPERTIES OUTPUT_NAME "scaffold")
#SET_TARGET_PROPERTIES(scaffold_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

ADD_LIBRARY(scaffold_shared SHARED ${SOURCE_FILES})
SET_TARGET_PROPERTIES(scaffold_shared PROPERTIES OUTPUT_NAME "scaffold")
SET_TARGET_PROPERTIES(scaffold_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(scaffold_shared PROPERTIES VERSION 0 SOVERSION 0)

#INSTALL(TARGETS scaffold_static scaffold_shared
#		ARCHIVE DESTINATION lib
#		LIBRARY DESTINATION lib)
INSTALL(TARGETS scaffold_shared
        LIBRARY DESTINATION lib)
INSTALL(DIRECTORY Header/ DESTINATION include/scaffold)
