#include "convert.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "lib/paths/paths.h"
#include "pdf_converter/pdf_converter.h"

// Function prototypes.
char** get_converted_files(sqlite3 *db, int max, int* uuid_list_size, int *rows);
void dealloc_uuids(char ** uuids, int row_count);

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
    
    //printf("Map 0 puid: %s\n", args->convertermaps[0].puid);
    //printf("Map 0 converter: %s\n", args->convertermaps[0].converter);
    char file_path_buffer[300];
    char log_file_path[300];
    sprintf(log_file_path, "%s/log.txt", args->outdir);

    FILE *fp = fopen(log_file_path, "w");
    
    size_t root_data_path_length = strlen(args->root_data_path);

    // Open database:
    sqlite3 *db;
    int error_flag = sqlite3_open(args->db_path, &db);
    if(error_flag) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
    }


    args->files = malloc(sizeof(ArchiveFile)*args->file_count);
    char * query_get_non_converted_pdfs = "SELECT * FROM Files WHERE Files.uuid not in (Select uuid From _ConvertedFiles)"
                    " AND Files.puid in ('fmt/18', 'fmt/276', 'fmt/19', 'fmt/20');";

    char * query_get_pdfs = "SELECT * FROM Files WHERE Files.puid in ('fmt/18', 'fmt/276', 'fmt/19', 'fmt/20');";
    
    get_archivefile_entries(db, args->files, args->file_count, query_get_pdfs);
    
    sqlite3_close(db);

    printf("Finished parsing all the archive files. Starting conversion to PDFA.\n");
    
    int count = 0;

    for (size_t i = 0; i < args->file_count; i++)
    {
        // Clear the file_path_buffer.
        memset(file_path_buffer, 0, strlen(file_path_buffer));
        
        // Create the path root + relative_path.
        insert_combined_path(file_path_buffer, args->root_data_path, args->files[i].relative_path);
        
        // Create output dir.
        char out_dir[300];
        char folder[10];
        strcpy(out_dir, args->outdir);
        snprintf(folder, 10, "/%ld/", i);
        strcat(out_dir, folder);
        mkdir(out_dir, 0777);
        
        // Format the file_path and run conversion.
        format_string(args->files[i].relative_path);
        convert_to_pdf_a(args->files[i].relative_path, out_dir, args->root_data_path);
        
        // Log to the file.
        fprintf(fp, "%s\n", args->files[i].uuid); 
        count++;
        if((count % 1000) == 0){
            printf("Converted %d files.\n", &count);    
        }
    }

    fclose(fp);
    free(args->files);

    return 0;
    
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
        memcpy(file->uuid, sqlite3_column_text(stmt, 1), sizeof(file->uuid));
        memcpy(file->relative_path, sqlite3_column_text(stmt, 2), sizeof(file->relative_path));
        
        /* Not currently used.
        memcpy(file->checksum,  sqlite3_column_text(stmt, 3), sizeof(file->checksum));
        memcpy(file->puid, sqlite3_column_text(stmt, 4), sizeof(file->puid));
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
   