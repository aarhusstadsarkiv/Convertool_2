#include "convert.h"
#include <string.h>
#include <stdio.h>
#include "lib/paths/paths.h"
#include <sqlite3.h>

int setup_converted_files(char *db_path);

int main(int argc, const char *argv[])
{   



    char db_path[200];
    char outdir [200];
    char root_path[200];

    strcpy(db_path, argv[1]);
    strcpy(outdir, argv[2]);
    long thread_count = strtol(argv[3], NULL, 10);
    long file_count = strtol(argv[4], NULL, 10);
    
    get_parent_path(root_path, db_path, strlen(db_path));
    get_parent_path(root_path, root_path, strlen(root_path));
    strcat(root_path, "/");
    
    
    /*ConverterMap maps[10];
    strcpy(maps[0].puid, "fmt/45");
    strcpy(maps[0].converter, "libre");
    */


    int setup_status = setup_converted_files(db_path);

    if(setup_status != 0){
        printf("Setup of database did not execute correctly. Aborting program execution.\n");
        return 1;
    }
    
    ConvertArgs args = {.db_path=db_path, .file_count=file_count, .max_errs=100, .outdir=outdir,
                        .root_data_path=root_path, .thread_count=thread_count};
    

    
    convert(&args);
    return 0;
}


int setup_converted_files(char *db_path){
   // Open database:
    sqlite3 *db;
    int error_flag = sqlite3_open(db_path, &db);
    if(error_flag) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return 1;
    }

     // Create _ConvertedFiles table.
    char * create_table_query = "CREATE TABLE IF NOT EXISTS \"_ConvertedFiles\""
                                " (file_id INTEGER NOT NULL, uuid VARCHAR NOT NULL, FOREIGN KEY(file_id) REFERENCES \"Files\" (id));";
    
    char *err_msg;

    int result_code = sqlite3_exec(db, create_table_query, NULL, NULL, &err_msg);
    if(result_code != SQLITE_OK){
        printf("Could not create table _ConvertedFiles %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}