cmake_minimum_required(VERSION 3.10)

# set the project name
project(Convertool)


set(SOURCE_FILES 
		main.c 
		convert.c 
		ArchiveFile/ArchiveFile.c 
		pdf_converter/pdf_converter.c
		libre_converter/libre_converter.c 
		lib/paths/paths.c  
		lib/ThreadPool/mythreads.c 
		)

set(HEADER_FILES lib/paths/paths.h lib/ThreadPool/mythreads.h)


# add the executable
#add_executable(Convertool main.c convert.c ArchiveFile/ArchiveFile.c pdf_converter/pdf_converter.c libre_converter/libre_converter.c lib/paths/paths.c lib/paths/paths.h lib/ThreadPool/mythreads.c lib/ThreadPool/mythreads.h)
add_executable(Convertool ${SOURCE_FILES} ${HEADER_FILES})

find_package (SQLite3  REQUIRED)
find_package(Threads REQUIRED)

if (SQLITE3_FOUND)
	  include_directories(${SQLITE3_INCLUDE_DIRS})
	  target_link_libraries (Convertool ${SQLite3_LIBRARIES} Threads::Threads)
else()
		message("Sqlite3 not found")
endif (SQLITE3_FOUND)
