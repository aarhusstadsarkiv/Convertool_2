#include "ArchiveFile.h"
#include <stdlib.h>
#include <stdio.h>

void update_db(ArchiveFile *converted_file, sqlite3 *db){
    sqlite3_stmt *stmt = NULL;
    int rc = 0;
    char sql_query[200];
    
    // Build the sql query.
    snprintf(sql_query, 200, "INSERT INTO _ConvertedFiles VALUES (%ld, \"%s\")", converted_file->id, converted_file->uuid);
    
    rc = sqlite3_prepare_v2(
        db, sql_query,
       -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare SQL: %s\n", sqlite3_errmsg(db)); 
    }

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        // Database update error handling.
    }

    sqlite3_finalize(stmt);
}
