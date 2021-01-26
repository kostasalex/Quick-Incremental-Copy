#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "copy.h"

int NoDotAndDotDot(struct dirent *d){
    return (strcmp(d->d_name, ".") && strcmp(d->d_name, ".."));
}


int copyDir(DIR *dir_src, DIR *dir_dest,\
char *srcPath, char *destPath){

    static int firstcall = 0;
    firstcall++;

    if(firstcall == 1){
        //now = time(0);
    }

    struct stat statbuf;

    //if not first call call close dir..
    int counter;

    struct dirent * dirent_src, *dirent_dest;

    DIR *next_dir_src, *next_dir_dest;

    char buffer[300], destFullPath[300], srcFullPath[300];
    int destFound;

    while ((dirent_src = readdir(dir_src)) != NULL){
    printf("%ld\n", stats.start);
        destFound = 0;
        /* If it's not directory or it's current/parent directory skip */
        if(dirent_src->d_type != DT_DIR || !NoDotAndDotDot(dirent_src))
            continue;
        
        //Source full path name
        sprintf(srcFullPath,"%s/%s", srcPath,dirent_src->d_name);

        /* Check if folder is created during the copy proccess */
        stat(srcFullPath, &statbuf);
        long long int  test;
        //if((test = statbuf.st_mtime - stats.start) < 0){
        //    continue;
        //}
        //printf("%lld\n%ld\n%ld\n ", test, statbuf.st_atime, stats.start);

        /* Else find the directory in destination */
        while((dirent_dest = readdir(dir_dest)) != NULL){

            if(dirent_dest->d_type != DT_DIR || !NoDotAndDotDot(dirent_dest))
                continue;

            if(!strcmp(dirent_src->d_name, dirent_dest->d_name)){
                destFound = 1;
                break;
            }

        }

        sprintf(destFullPath,"%s/%s", destPath, dirent_src->d_name);
        //Folder in destination directory not found, create new.
        if(!destFound){
            if(mkdir(destFullPath, 0700) == -1)
                return -1;
            //printf("%s\n", destFullPath);
            stats.dirCount++;
        }

        /* Search recursively in sub folders */
        if((next_dir_src = opendir(srcFullPath)) == NULL)
                return -1;

        if((next_dir_dest = opendir(destFullPath)) == NULL)
                return -1;

        copyDir(next_dir_src, next_dir_dest, srcFullPath, destFullPath);
    }

    return 0;
}