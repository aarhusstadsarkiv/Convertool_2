typedef struct ArchiveFile{
    char uuid[36];
    char checksum[64];
    char relative_path[200];
    int is_binary;
    int file_size_in_bytes;
    char puid[10];
    char signature[50];
    char warning [100];
} ArchiveFile;