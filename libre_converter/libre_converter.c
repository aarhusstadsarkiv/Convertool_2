#include <stdio.h>
#include "libre_converter.h"
#include "../lib/paths/paths.h"





void *libre_convert(void *libre_args){

    int oldtype;
    /* allow the thread to be killed at any time */
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);

    LibreArgs *args = (LibreArgs*) libre_args;
    char * file_path = get_combined_path(args->root_path, args->file);
    char *format;

    switch (args->format_specifier)
    {
    case FORMAT_PDF:
        format = "pdf";
        break;
    case FORMAT_ODT:
        format = "odt";
        break;
    case FORMAT_ODS:
        format = "ods";
        break;
    default:
        format = "pdf";
        break;
    }
    
    char cmd[500];

    snprintf(cmd, 500, "libreoffice --headless --convert-to %s \"%s\" --outdir %s > /dev/null 2>/dev/null%c", 
            format, file_path, args->outdir, '\0');

    free(file_path);
    system(cmd);   
    pthread_cond_signal(args->done);
    
    return NULL;
    
}