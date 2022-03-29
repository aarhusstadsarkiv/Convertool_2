typedef struct ARGSPDF{
    char device[50];
    char compression[50];
    char dpi [10];
} ARGSPDF;

void convert_to_pdf_a(char* file, char* outdir, char* root_path);