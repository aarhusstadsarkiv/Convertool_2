#include "convert.h"
#include <string.h>
#include <stdio.h>
#include "lib/paths/paths.h"
int main(int argc, const char *argv[])
{   
    char db_path[200];
    char outdir [200];
    char root_path[200];

    strcpy(db_path, argv[1]);
    strcpy(outdir, argv[2]);

    get_parent_path(root_path, db_path, strlen(db_path));
    get_parent_path(root_path, root_path, strlen(root_path));
    strcat(root_path, "/");
    
    
    /*ConverterMap maps[10];
    strcpy(maps[0].puid, "fmt/45");
    strcpy(maps[0].converter, "libre");
    */
    ConvertArgs args = {.db_path=db_path, .file_count=120000, .max_errs=100, .outdir=outdir,
                        .root_data_path=root_path};
    
    convert(&args);
    return 0;
}