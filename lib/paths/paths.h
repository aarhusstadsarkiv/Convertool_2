#ifndef PATHS_H
#define PATHS_H

/*
    The returned string from this function
    is dynamically allocated and must
    be deallocated with free.
*/
char* get_combined_path(char *root, char *to_append);

char* insert_combined_path(char *buffer, char *root, char *to_append);

#endif