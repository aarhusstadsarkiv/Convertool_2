#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
/*
    The returned string from this function
    is dynamically allocated and must
    be deallocated with free.
*/
char* get_combined_path(char *root, char *to_append){

    size_t root_length =  strlen(root);
    size_t to_append_length = strlen(to_append);

    size_t result_path_length = root_length + 2 + to_append_length;

    char *result_path = malloc(sizeof(char) * result_path_length); 
    memcpy(result_path, root, root_length);
    result_path[root_length] = '/';
    result_path[root_length+1] = '\0';
    
    strcat(result_path, to_append);
    return result_path;
}

char* insert_combined_path(char *buffer, char *root, char *to_append){

    size_t root_length =  strlen(root);
    memcpy(buffer, root, root_length);
    buffer[root_length] = '/';
    buffer[root_length+1] = '\0';
    
    strcat(buffer, to_append);
}

void get_parent_path(char* buffer, char* original_path, size_t length){
    size_t index = 0;
    for (size_t i = 0; i < length-1; i++)
    {
        if(original_path[i] == '/')
            index = i;
    }
    
    strncpy(buffer, original_path, index);
    buffer[index] = '\0';
}

void make_output_dir(char* outdir){
    int return_code = mkdir(outdir, S_IRWXU);
    if(return_code == -1){
        //printf("%s\n", outdir);
        //perror("Could not create outdir: ");
        // Try to make the docCollection folder, since it might not exist.
        char parent_path[500];
        get_parent_path(parent_path, outdir, strlen(outdir));
        return_code = mkdir(parent_path, S_IRWXU);
        
        if(return_code == -1){
            //printf("Could not create the parent path %s with error:\n", parent_path);
            //perror("Error: ");
        }
        /* 
            If we could create the docCollection folder,
            we try once more. 
        */
       
        else{
            return_code = mkdir(outdir, S_IRWXU);
            if(return_code == -1){
                perror("Could not create the output directory: ");
            }
        }
    }
}