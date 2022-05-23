#include <sqlite3.h>
#include "../ArchiveFile/ArchiveFile.h"
#include <pthread.h>

typedef struct ARGSPDFTIF{
    char device[50];
    char compression[50];
    char dpi [10];
} ARGSPDFTIF;

typedef struct ARGSPDF{
    ArchiveFile *archive_file;
    char outdir[300];
    char *root_path;
    pthread_mutex_t *db_update_mutex;
    sqlite3 *db;
} ARGSPDF;

void convert_to_pdf_a(void *args);