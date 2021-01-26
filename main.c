#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "copy.h"


/* Gets source/destination directories and flags.
Returns -1 for invalid input*/
DIR *dir_src, *dir_dest;
int inputHandler(int argc, char *argv[]);

int main(int argc, char * argv []) {

    /* Init global variables */
    flags.verbose = 0;flags.deleted = 0;flags.links = 0;
    stats.dirCount = 0;stats.start = time(0);
    printf("%ld\n", stats.start);
    //return 0;

    if(inputHandler(argc, argv) == -1){
        printf("Usage: source_path destination_path -v -d -l\n");
        return -1;
    }

    stats.dirCount = 0;
    copyDir(dir_src, dir_dest, argv[1], argv[2]);
    printf("%d directories created\n", stats.dirCount);

}



int inputHandler(int argc, char *argv[]){

    if(argc >= 3){//Valid #arguments

        /* Read source directory */
        if((dir_src = opendir(argv[1])) == NULL){
            return -1;
        }
        
        /* Read destination directory and create if not exists*/
        if((dir_dest = opendir(argv[2])) == NULL){
            if(mkdir(argv[2], 0700) == -1)
                return -1;
            else dir_dest = opendir(argv[2]);

        }

        /* Read input flags if exists */
        if(argc > 3){
            for(int i = 3; i < argc; i++){
                
                if(!strcmp(argv[i],"-v"))    
                    flags.verbose = 1;
                
                else if(!strcmp(argv[i],"-d"))
                    flags.deleted = 1;
                
                else if(!strcmp(argv[i],"-l"))
                    flags.links = 1;
            }
        }
    }
    else //Invalid #arguments
        return -1;

    return 0;

}
