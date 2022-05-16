#include "convert.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "lib/paths/paths.h"
#include "pdf_converter/pdf_converter.h"
#include "libre_converter/libre_converter.h"

// Function prototypes.
char** get_converted_files(sqlite3 *db, int max, int* uuid_list_size, int *rows);
void dealloc_uuids(char ** uuids, int row_count);
int convert_sequential(FILE *fp, ConvertArgs *args);

void format_string(char * file_path){
    size_t file_path_length = strlen(file_path);
    for (size_t x = 0; x < file_path_length; x++)
        {
            if(file_path[x] == '\\'){
                 file_path[x] = '/';
            }   
        }
}
int convert(ConvertArgs *args){
    
    
    char log_file_path[300];
    sprintf(log_file_path, "%s/log.txt", args->outdir);

    FILE *fp = fopen(log_file_path, "w");
    
    // Open database:
    sqlite3 *db;
    int error_flag = sqlite3_open(args->db_path, &db);
    if(error_flag) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
    }

    // pdf_puids
    char *pdf_puids[] = {"fmt/16", "fmt/17", "fmt/18", "fmt/276", "fmt/19", "fmt/20"};

    // libre_puids
    char *libre_puids[] = {
                            "fmt/40", "fmt/412", "fmt/61", "fmt/214",
                            "fmt/126", "fmt/609", "fmt/215", "fmt/39", "fmt/445"
                        };

    // excel puids
    char *excel_puids[] = {"fmt/214", "fmt/445"};

    size_t pdf_puids_length = sizeof(pdf_puids) / sizeof(char *);
    size_t libre_puids_length = sizeof(libre_puids) / sizeof(char *);
    size_t excel_puids_length = sizeof(excel_puids) / sizeof(char *);

    args->files = malloc(sizeof(ArchiveFile)*args->file_count);
    
    char * query;
    if(args->thread_count == 1)
        query = QUERY_GET_NON_CONVERTED_WORD_FILES;
        
    else if(args->thread_count > 1){
        query = QUERY_GET_NON_CONVERTED_PDFS;
    }

    else{
        printf("Need at least 1 thread to run the program.\n");
        exit(1);
    }
    
    get_archivefile_entries(db, args->files, args->file_count, query);
    sqlite3_close(db);

    printf("Finished parsing all the archive files. Starting conversion to PDFA.\n");
    
    if(args->thread_count == 1){
        convert_sequential(fp, args);
    }

    else{
        // Run multithreaded conversion.
    }
    
    
    fclose(fp);
    free(args->files);

    
}

   int get_archivefile_entries(sqlite3 *db, ArchiveFile *files, int max, char sql_query []) {
    
    sqlite3_stmt *stmt = NULL;
    int rc = 0;
    int i = 0;
    
    rc = sqlite3_prepare_v2(
        db, sql_query,
       -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL: %s\n", sqlite3_errmsg(db));
        return 1; 
    }

    do {
        ArchiveFile *file = &files[i++];
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            printf("No more rows ...\n");
            break;
        } else if (rc != SQLITE_ROW) {
            fprintf(stderr, "Problem: %s\n", sqlite3_errmsg(db)); 
            sqlite3_finalize(stmt);
            return 1;
        }

        // TODO: Move this block of code to a seperate function.
        //memcpy(file->uuid, sqlite3_column_text(stmt, 1), sizeof(file->uuid));
        //memcpy(file->relative_path, sqlite3_column_text(stmt, 2), sizeof(file->relative_path));
        
        strcpy(file->uuid, sqlite3_column_text(stmt, 1));
        strcpy(file->relative_path, sqlite3_column_text(stmt, 2));
        strcpy(file->puid, sqlite3_column_text(stmt, 4));
        

        /* Not currently used.
        memcpy(file->checksum,  sqlite3_column_text(stmt, 3), sizeof(file->checksum));
        memcpy(file->signature, sqlite3_column_text(stmt, 5), sizeof(file->signature));
        file->is_binary = sqlite3_column_int(stmt, 6);
        file->file_size_in_bytes = sqlite3_column_int(stmt, 7);
        if( sqlite3_column_text(stmt, 8) != NULL)
            memcpy(file->warning, sqlite3_column_text(stmt, 8), sizeof(file->warning));
        */
    } 
    

    while (i < max);

    sqlite3_finalize(stmt);
    return 0; 
}

char** get_converted_files(sqlite3 *db, int max, int *uuid_list_size, int *rows){

    sqlite3_stmt *stmt = NULL;
    int rc = 0;
    int i = 0;
    int uuid_string_size = 100; 
    char **uuids = malloc(sizeof(char*) * 200);
    
    rc = sqlite3_prepare_v2(
        db, "SELECT uuid from _ConvertedFiles",
        -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL: %s\n", sqlite3_errmsg(db));
        return NULL; 
    }

    while(i < max){
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            printf("No more rows ...\n");
            break;
        } else if (rc != SQLITE_ROW) {
            fprintf(stderr, "Problem: %s\n", sqlite3_errmsg(db)); 
            sqlite3_finalize(stmt);
            return NULL;
        }

        if (i > (*uuid_list_size)){
           // Realloc uuids.
            void *temp = realloc(uuids, (*uuid_list_size)+10000);
            if(temp != NULL){
                uuids = (char **) temp;
                *(uuid_list_size)+=10000;
            }

           
        }

        uuids[i] = malloc(sizeof(char)*uuid_string_size);
        strcpy(uuids[i], sqlite3_column_text(stmt, 0));
        (*rows)++;
        i++;
        
    }

    sqlite3_finalize(stmt);
    return uuids;
}

void dealloc_uuids(char ** uuids, int row_count){
    for (int i = 0; i < row_count; i++)
    {
        free(uuids[i]);
    }
    free(uuids);
}


int compare_puids(char *puid, char *puids[], size_t puids_length){
    for (size_t i = 0; i < puids_length; i++)
    {
        if(strcmp(puid, puids[i]) == 0)
            return 1;
    }

    // If the puid is not in the array, we return false.
    return 0;
    
}

int convert_sequential(FILE *fp, ConvertArgs *args){
    int count = 0; 

    // Buffers
    char file_path_buffer[300];
    char out_dir[300];
    char destination_folder[200];

     // libre_puids
    char *libre_puids[] = {
                            "fmt/40", "fmt/412", "fmt/61", "fmt/214",
                            "fmt/126", "fmt/609", "fmt/215", "fmt/39", "fmt/445"
                        };

    // excel puids
    char *excel_puids[] = {"fmt/214", "fmt/445"};

    size_t libre_puids_length = sizeof(libre_puids) / sizeof(char *);
    size_t excel_puids_length = sizeof(excel_puids) / sizeof(char *);

    for (size_t i = 0; i < args->file_count; i++)
    {
        // Clear the file_path_buffer.
        memset(file_path_buffer, 0, strlen(file_path_buffer));
        
        memset(out_dir, 0, 300);
        memset(destination_folder, 0, 200);

        // Create the path root + relative_path.
        insert_combined_path(file_path_buffer, args->root_data_path, args->files[i].relative_path);
       
        /* 
            Format the file_path and get the destination folder.
            Destination folder is equal to the parent of the relative path.
        */

        format_string(args->files[i].relative_path);
        size_t relative_path_length = strlen(args->files[i].relative_path);

        if(relative_path_length < 5){
            printf(" No more files to convert\n");
            exit(0);
        }

        get_parent_path(destination_folder, args->files[i].relative_path,
                        relative_path_length);

        
        strcpy(out_dir, args->outdir);
        strcat(out_dir, destination_folder);

        make_output_dir(out_dir);
        
        char * puid = args->files[i].puid;
        
       
        if(compare_puids(puid, libre_puids, libre_puids_length)){
            libre_convert(args->files[i].relative_path, out_dir, args->root_data_path, FORMAT_PDF);

            // If the file is also an excel file, we convert it to ods alongside the generated pdf.
            if(compare_puids(puid, excel_puids, excel_puids_length))
                libre_convert(args->files[i].relative_path, out_dir, args->root_data_path, FORMAT_ODS);
        }
        
        // Log to the file.
        fprintf(fp, "%s\n", args->files[i].uuid); 
        count++;
        if((count % 500) == 0){
            printf("Converted %d files.\n", count);    
        }
    }

    return 0;
}