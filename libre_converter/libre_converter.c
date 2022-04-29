#include <stdlib.h>
#include <stdio.h>

#include "libre_converter.h"
#include "../lib/paths/paths.h"

void libre_convert(char *file, char *outdir, char *root_path, int format_specifier){
    char * file_path = get_combined_path(root_path, file);
    char *format;

    switch (format_specifier)
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
            format, file_path, outdir, '\0');

    system(cmd);
    
    free(file_path);
}