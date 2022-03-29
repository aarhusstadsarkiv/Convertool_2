#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/paths/paths.h"
#include <unistd.h>

void build_command(char cmd[], char *base_cmd[], char output_file_argument[], char file_path[]){
    strcpy(cmd, "gs ");

    for (size_t i = 0; i < 6; i++)
    {
    strcat(cmd, base_cmd[i]);  
    }


    strcat(cmd, output_file_argument);

    // Use array index instead of strcat for " sign.
    strcat(cmd, "\"");
    strcat(cmd, file_path);

    strcat(cmd, "\"");
}

void run_command(char *cmd, char *file_path){

    pid_t process_id = fork();
    if(process_id < 0){
        perror("Fork returned an error: ");
    }

    else if(process_id == 0){
        exec()
    }
}

void convert_to_pdf_a(char* file, char* outdir, char* root_path){
    char * output_file = get_combined_path(outdir, "out.pdf");    
    char * file_path = get_combined_path(root_path, file);
    printf("File path: %s\n", file_path);

    char* args [8]= {"-dBATCH ",
                    "-dNOPAUSE ",
                    "-dPDFA=3 ",
                    "-dPDFACompatibilityPolicy=1 ",
                    "-sDEVICE=pdfwrite ",
                    "-sColorConversionStrategy=UseDeviceIndependentColor "
    };

    char * output_file_specifier;
    sprintf(output_file_specifier, "-sOutputFile=%c", output_file);
    args[7] = "-sOutputFile=";
    
    // size_t output_file_length = strlen(output_file); 
    // size_t output_file_specifier_length = strlen(output_file_specifier);
    // We must include a space and a null byte in the string, thus + 2.
    // size_t output_file_argument_length = output_file_specifier_length + output_file_length + 2;

    // char output_file_argument[output_file_argument_length];
    // strcpy(output_file_argument, output_file_specifier);
    // strcat(output_file_argument, output_file);

    // Set the space at the end and the null byte to terminate the string.
    // output_file_argument[output_file_argument_length-2] = ' ';
    // output_file_argument[output_file_argument_length - 1] = '\0';

    char cmd[1000];

    build_command(cmd, cmd_1, output_file_argument, file_path);
    
    // Free dynamic memory after we run ghostscript.
    run_command(cmd, file_path);

    free(output_file);
    free(file_path);

}