#include "ArchiveFile/ArchiveFile.h"
#include <stdlib.h>
#include <sqlite3.h>
#include "convertermap.h"

typedef struct ConvertArgs{
    size_t file_count;
    ArchiveFile *files;
    ConverterMap *convertermaps;
    char *db_path;
    char *outdir;
    char *root_data_path;
    const int max_errs;
} ConvertArgs;

int get_archivefile_entries(sqlite3 *db, ArchiveFile *files, int max, char* sql_query);

int compare_puids(char *puid, char *puids[], size_t puids_length);

int convert(ConvertArgs *args);