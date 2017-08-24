CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(scaffold)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-Wall")

ADD_DEFINITIONS(-DCMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}")
INCLUDE_DIRECTORIES(Dependency Header)

# Solve dependencies
# OpenSSL
IF(NOT DISABLE_SSL)
	ADD_DEFINITIONS(-DMG_ENABLE_SSL=1)
	FIND_PACKAGE(OpenSSL REQUIRED)
	IF(OpenSSL_FOUND)
		INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIRS})
	ENDIF()
ENDIF()
# RapidJSON
FIND_PACKAGE(RapidJSON REQUIRED)
# Boost
IF(NOT DISABLE_MYSQL)
	FIND_PACKAGE(Boost REQUIRED)
ENDIF()

# Necessary source files
SET(SOURCE_FILES
	Dependency/mongoose/mongoose.c
	Foundation/Scaffold.cpp Foundation/Router.cpp
	Foundation/Request.cpp Foundation/Response.cpp
    Foundation/Logger.cpp
	Utility/StringOperation.cpp Utility/ResponseHelper.cpp
	Utility/MimeTypeMapping.cpp Utility/DateTimeHelper.cpp
	Utility/HttpStatusMapping.cpp Utility/RequestHelper.cpp
	Utility/HashAlgorithms.cpp Utility/FileSystem.cpp)

# Plugin options
IF(NOT DISABLE_SESSION) # Session
	ADD_DEFINITIONS("-DENABLE_SESSION=1")
	SET(SOURCE_FILES Plugin/Session.cpp ${SOURCE_FILES})
ENDIF()
IF(NOT DISABLE_MYSQL) # MySQL
	ADD_DEFINITIONS("-DENABLE_MYSQL=1")
	SET(SOURCE_FILES Plugin/MySQL/MySQL.cpp ${SOURCE_FILES})
ENDIF()

# Build options
IF(NOT DYNAMIC_ONLY)
	ADD_LIBRARY(scaffold_static STATIC ${SOURCE_FILES})
	SET_TARGET_PROPERTIES(scaffold_static PROPERTIES OUTPUT_NAME "scaffold")
	SET_TARGET_PROPERTIES(scaffold_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
	IF(NOT DISABLE_SSL)
		TARGET_LINK_LIBRARIES(scaffold_static ssl crypto)
	ENDIF()
	IF(NOT DISABLE_MYSQL)
		TARGET_LINK_LIBRARIES(scaffold_static mysqlcppconn-static)
	ENDIF()
ENDIF()

ADD_LIBRARY(scaffold_shared SHARED ${SOURCE_FILES})
SET_TARGET_PROPERTIES(scaffold_shared PROPERTIES OUTPUT_NAME "scaffold")
SET_TARGET_PROPERTIES(scaffold_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(scaffold_shared PROPERTIES VERSION 0.0.1 SOVERSION 0.0.1)
IF(NOT DISABLE_SSL)
	TARGET_LINK_LIBRARIES(scaffold_shared ssl crypto)
ENDIF()
IF(NOT DISABLE_MYSQL)
	TARGET_LINK_LIBRARIES(scaffold_shared mysqlcppconn)
ENDIF()

IF(DYNAMIC_ONLY)
	INSTALL(TARGETS scaffold_shared
			LIBRARY DESTINATION lib)
ELSE()
	INSTALL(TARGETS scaffold_static scaffold_shared
			ARCHIVE DESTINATION lib
			LIBRARY DESTINATION lib)
ENDIF()
INSTALL(DIRECTORY Header/ DESTINATION include/scaffold)

