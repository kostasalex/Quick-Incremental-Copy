#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "copy.h"

/* Gets source/destination directories and flags.
Returns -1 for invalid input*/
DIR *dir_src, *dir_dest;
int inputHandler(int argc, char *argv[]);



int main(int argc, char * argv []) {

    struct timeval end, start;
    gettimeofday(&start, NULL); 

    /* Init global variables */
    flags.verbose = 0;flags.deleted = 0;flags.links = 0;
    stats.copyCount = 0;stats.start = time(0);stats.bytes = 0;
    stats.total = 0;stats.delCount = 0;


    if(inputHandler(argc, argv) == -1){
        printf("Usage: source_path destination_path -v -d -l\n");
        return -1;
    }

    printf("created directory %s\n", argv[2]);
    printf("Copying directories/files..\n");

    /* Copy all new/modified files from source to destination */
    copyDir(dir_src, dir_dest, argv[1], argv[2]);


    /* Flag '-d' , remove all deleted files from source */
    if(flags.deleted){
        printf("Remove unused directories/files in destination path..\n");
        rewinddir(dir_dest);rewinddir(dir_src);
        delete(dir_src, dir_dest, argv[1], argv[2]);
        printf("number of entities deleted from destination path is %d\n", stats.delCount);
    }
    gettimeofday(&end, NULL);

    /* Flag '-v' , print all stats needed*/
    if(flags.verbose){
        printf("there are %d files/directories in the hierarchy\n", stats.total);
        printf("number of entities copied is %d\n", stats.copyCount);
        if(stats.copyCount){
            printf("copied %d bytes in", stats.bytes);
            double secs = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
            printf(" %.3f at %.2f bytes/sec\n",secs, stats.bytes / secs);
        }


    }


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
