cmake_minimum_required(VERSION 3.10)
# set the project name
project(Convertool)


# add the executable
add_executable(Convertool main.c convert.c ArchiveFile/ArchiveFile.c pdf_converter/pdf_converter.c lib/paths/paths.c lib/paths/paths.h)

find_package (SQLite3  REQUIRED)
find_package(Threads REQUIRED)

if (SQLITE3_FOUND)
	  include_directories(${SQLITE3_INCLUDE_DIRS})
	  target_link_libraries (Convertool ${SQLite3_LIBRARIES} Threads::Threads)
else()
		message("Sqlite3 not found")
endif (SQLITE3_FOUND)
