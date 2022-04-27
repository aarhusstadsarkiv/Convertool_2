#include<stdio.h>
#include <stdlib.h>
#include "../lib/paths/paths.h"
#include "pdf_converter.h"


void convert_to_pdf_a(char* file, char* outdir, char* root_path){
    char * output_file = get_combined_path(outdir, "1.pdf");    
    char * file_path = get_combined_path(root_path, file);

    char cmd[1000];
    snprintf(cmd, 1000, "gs -dBATCH -dNOPAUSE -dPDFA=3 dPDFACompatibilityPolicy=1 -sDEVICE=pdfwrite" 
                        " -sColorConversionStrategy=RGB -sOutputFile=%s \"%s\" > /dev/null 2>/dev/null",
                        output_file, file_path
            );
    
    system(cmd);

    // Free dynamic memory after we run ghostscript.
    free(output_file);
    free(file_path);
}