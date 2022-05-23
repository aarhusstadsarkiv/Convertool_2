#include "ArchiveFile/ArchiveFile.h"
#include <stdlib.h>
#include <sqlite3.h>
#include "convertermap.h"



#define QUERY_GET_NON_CONVERTED_PDFS "SELECT * FROM Files WHERE Files.puid in ('fmt/16', 'fmt/17', 'fmt/18', 'fmt/276', 'fmt/19', 'fmt/20')"\
                                      " AND Files.uuid not in (Select uuid From _ConvertedFiles);"

#define QUERY_GET_NON_CONVERTED_WORD_FILES "SELECT * FROM Files WHERE Files.puid in ('fmt/40', 'fmt/412', 'fmt/61', 'fmt/214', "\
                                            "'fmt/126', 'fmt/609', 'fmt/215', 'fmt/39', 'fmt/445')"\
                                            " AND Files.uuid not in (Select uuid From _ConvertedFiles);"

typedef struct ConvertArgs{
    size_t file_count;
    ArchiveFile *files;
    ConverterMap *convertermaps;
    char *db_path;
    char *outdir;
    char *root_data_path;
    const int max_errs;
    long thread_count;
} ConvertArgs;

int get_archivefile_entries(sqlite3 *db, ArchiveFile *files, int max, char* sql_query);

int compare_puids(char *puid, char *puids[], size_t puids_length);

int convert(ConvertArgs *args);

/*
char * query_get_non_converted_pdfs = "SELECT * FROM Files WHERE Files.puid in ('fmt/16', 'fmt/17', 'fmt/18', 'fmt/276', 'fmt/19', 'fmt/20')"
                                            " AND Files.uuid not in (Select uuid From _ConvertedFiles);";

char * query_get_pdfs = "SELECT * FROM Files WHERE Files.puid in ('fmt/18', 'fmt/276', 'fmt/19', 'fmt/20');";

char * query_get_non_converted_word_files = "SELECT * FROM Files WHERE Files.puid in ('fmt/40', 'fmt/412')"
                                            " AND Files.uuid not in (Select uuid From _ConvertedFiles);";
    
char * query_get_non_converted_files =  "SELECT * FROM Files WHERE Files.puid in ('fmt/16', 'fmt/17', 'fmt/18', "
                                            "'fmt/276', 'fmt/19', 'fmt/20', 'fmt/40', 'fmt/412', 'fmt/61', 'fmt/214', "
                                            "'fmt/126', 'fmt/609', 'fmt/215', 'fmt/39', 'fmt/445')"
                                            " AND Files.uuid not in (SELECT uuid FROM _ConvertedFiles);";
*/