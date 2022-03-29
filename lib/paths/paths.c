#include <stdlib.h>
#include <string.h>
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
    result_path[root_length] = '\0';
    
    strcat(result_path, to_append);
    return result_path;
}

char* insert_combined_path(char *buffer, char *root, char *to_append){

    size_t root_length =  strlen(root);
    memcpy(buffer, root, root_length);
    buffer[root_length] = '/';
    buffer[root_length] = '\0';
    
    strcat(buffer, to_append);
}