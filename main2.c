#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pdf_converter/pdf_converter.h"

int main(int argc, char const *argv[])
{
    char *root = "/mnt/d";
    char* pdf_file = "Annotation.pdf";
    convert_to_pdf_a(pdf_file, "/mnt/d/AARS_test_root", root);
    size_t result_length = strlen(root) + strlen(pdf_file);

    return 0;
}
