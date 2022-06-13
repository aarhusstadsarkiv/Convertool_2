#include <stdlib.h>
#include <pthread.h>

#define FORMAT_PDF 0
#define FORMAT_ODT 1
#define FORMAT_ODS 2

typedef struct LibreArgs{
    char *file;
    char *outdir;
    char *root_path; 
    int format_specifier;
    pthread_cond_t *done;
} LibreArgs;

void *libre_convert(void *libre_args);
