typedef struct ARGSPDFTIF{
    char device[50];
    char compression[50];
    char dpi [10];
} ARGSPDFTIF;

typedef struct ARGSPDF{
    char file[300];
    char outdir[300];
    char root_path[400];
} ARGSPDF;

void convert_to_pdf_a(void *args);