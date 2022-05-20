#include<stdio.h>
#include <stdlib.h>
#include "../lib/paths/paths.h"
#include "pdf_converter.h"


void convert_to_pdf_a(void *args){
    ARGSPDF *pdf_args = (ARGSPDF*) args;
    char * output_file = get_combined_path(pdf_args->outdir, "1.pdf");    
    char * file_path = get_combined_path(pdf_args->root_path, pdf_args->file);

    char cmd[1000];
    snprintf(cmd, 1000, "gs -dBATCH -dNOPAUSE -dPDFA=3 -dPDFACompatibilityPolicy=1 -sDEVICE=pdfwrite" 
                        " -sColorConversionStrategy=RGB -sOutputFile=%s \"%s\" > /dev/null 2>/dev/null",
                        output_file, file_path
            );
    
    system(cmd);

    // Free dynamic memory after we run ghostscript.
    free(output_file);
    free(file_path);
}