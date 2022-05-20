typedef struct ArchiveFile{
    long id;
    char uuid[36];
    char checksum[64];
    char relative_path[200];
    char puid[10];
    // The following attributes are not currently used.
    //int is_binary;
    //int file_size_in_bytes;
    //char signature[50];
    //char warning [100];
} ArchiveFile;