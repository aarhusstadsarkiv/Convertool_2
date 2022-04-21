#include "convert.h"
#include <string.h>
#include <stdio.h>

int main(int argc, const char *argv[])
{   
    char db_path[200] = "/mnt/d/Staging/Processing/AVID.AARS.22.1/OriginalDocuments/_metadata/files.db";
    char outdir [200] = "/mnt/e/Staging/Processing/convertool_2";
    //strcpy(db_path, argv[1]);
    //strcpy(outdir, argv[2]);

    ConverterMap maps[10];
    strcpy(maps[0].puid, "fmt/45");
    strcpy(maps[0].converter, "libre");
    
    ConvertArgs args = {.db_path=db_path, .file_count=10, .max_errs=5, .outdir=outdir,
                        .root_data_path="/mnt/d/Staging/Processing/AVID.AARS.22.1/OriginalDocuments/", .convertermaps=maps};
    convert(&args);
    return 0;
}