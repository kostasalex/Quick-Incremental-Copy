#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "edit.h"

/* in case of Current or Parrent directory returns 0*/
int NoDotAndDotDot(struct dirent *d){
    return (strcmp(d->d_name, ".") && strcmp(d->d_name, ".."));
}


int copyDir(DIR *dir_src, DIR *dir_dest,\
char *srcPath, char *destPath){

    struct dirent * dirent_src, *dirent_dest;
    char destFullPath[300], srcFullPath[300];
    int destFound;

    /* Loop every file/directory in source path */
    while ((dirent_src = readdir(dir_src)) != NULL){

        destFound = 0;

        //If it's current/parent directory skip
        if(!NoDotAndDotDot(dirent_src))continue;
        
        stats.total++;//Total files/directories in source

        //Source full path name
        sprintf(srcFullPath,"%s/%s", srcPath,dirent_src->d_name);

        /* Search if a copy exists in destination path */
        while((dirent_dest = readdir(dir_dest)) != NULL){

            if(!NoDotAndDotDot(dirent_dest))
                continue;

            if(!strcmp(dirent_src->d_name, dirent_dest->d_name)){
                destFound = 1;
                break;
            }

        }
        rewinddir(dir_dest);
        /* Destination full path name */
        sprintf(destFullPath,"%s/%s", destPath, dirent_src->d_name);

        struct stat srcstat;
        stat(srcFullPath, &srcstat);

        /* Destination not found Create new directory or copy file */
        if(!destFound){

            //Create new directory
            if(dirent_src->d_type == DT_DIR){
                if(mkdir(destFullPath, 0700) == -1)
                    return -1;    
                stats.copyCount++;
                printf("%s\n",destFullPath);  
            }
            //Create new file
            else if(dirent_src->d_type == DT_REG){
                if(copyFile(srcFullPath, destFullPath, srcstat.st_size)==-1)
                    return -1;
                stats.bytes += srcstat.st_size;
                stats.copyCount++;
                printf("%s\n",destFullPath);
            }

        }
        /* If destination file is different from source create new */
        else{ 
            //Check if it's file
            if(dirent_src->d_type == DT_REG){
                    struct stat deststat;
                    stat(srcFullPath, &deststat);

                    //Check if the files are different
                    if(deststat.st_size != srcstat.st_size  || \
                        deststat.st_mtime < srcstat.st_mtime){
                        if(copyFile(srcFullPath, destFullPath, srcstat.st_size)==-1)
                            return -1;
                        stats.bytes += srcstat.st_size;
                        stats.copyCount++;
                        printf("%s\n",destFullPath);
                    }
            }

        }

        if(dirent_src->d_type == DT_DIR){

            /* Search recursively in sub directories */
            DIR *next_dir_src, *next_dir_dest;
            if((next_dir_src = opendir(srcFullPath)) == NULL)
                    return -1;

            if((next_dir_dest = opendir(destFullPath)) == NULL)
                    return -1;

            copyDir(next_dir_src, next_dir_dest, srcFullPath, destFullPath);
            closedir(next_dir_src);closedir(next_dir_dest);
        }

    }

    return 0;
}



int copyFile(char *srcFullName ,char *destFullName, int size){
    int infile, outfile;
    ssize_t nread;
    
    char buffer[size];

    if((infile = open(srcFullName , O_RDONLY)) == -1)
        return(-1);
    
    if((outfile = open(destFullName, O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1){
        close ( infile );
        return(-2);
    }

    while((nread = read(infile, buffer, size )) > 0 ){
        if(write(outfile, buffer ,nread) < nread){
        close(infile);close(outfile);return(-3);
        }
    }
    close(infile);close(outfile);

    /* Copy permissions */
    struct stat statbuf;
    stat(srcFullName, &statbuf);
    chmod(destFullName, statbuf.st_mode);

    if(nread == -1)return(-4);
    else return(0);
}


int delete(DIR *dir_src, DIR *dir_dest,\
char *srcPath, char *destPath){

    struct dirent * dirent_src, *dirent_dest;

    char destFullPath[300], srcFullPath[300];
    int srcFound;

    while ((dirent_dest = readdir(dir_dest)) != NULL){

        srcFound = 0;

        /* If it's current/parent directory skip */
        if(!NoDotAndDotDot(dirent_dest))continue;
        
        //dest full path name
        sprintf(destFullPath,"%s/%s", destPath, dirent_dest->d_name);

        /* Else find the file/directory in source */
        while((dirent_src = readdir(dir_src)) != NULL){

            if(!strcmp(dirent_src->d_name, dirent_dest->d_name)){
                srcFound = 1;
                break;
            }
        }
        rewinddir(dir_src);


        /* Source not found delete file/directory */
        if(!srcFound){

            //In case of file just delete it
            if(dirent_dest->d_type != DT_DIR){
                if(remove(destFullPath) == 0)
                    stats.delCount++;
                else 
                    printf("Couldn't remove :\n");
                printf("%s\n", destFullPath);
            }

            //In case of directory remove all sub directories/files recursively
            else{

                DIR *dir;
                if((dir = opendir(destFullPath)) == NULL)
                    return -1;
                deleteDir(dir, destFullPath);
                printf("%s\n", destFullPath);
                remove(destFullPath);
                stats.delCount++;
                closedir(dir);
            }
        }
        /* Source found */
        else{

            //Source full path name
            sprintf(srcFullPath,"%s/%s", srcPath,dirent_src->d_name);

            if(dirent_src->d_type == DT_DIR){

                /* Search recursively in sub directories */
                DIR *next_dir_src, *next_dir_dest;
                if((next_dir_src = opendir(srcFullPath)) == NULL)
                        return -1;

                if((next_dir_dest = opendir(destFullPath)) == NULL)
                        return -1;

                delete(next_dir_src, next_dir_dest, srcFullPath, destFullPath);
                
                closedir(next_dir_src);closedir(next_dir_dest);
            }
        }
    }
    return 0;
}


int deleteDir(DIR *dir, char* fullPath){

    struct dirent * dirent;

    char pathName[300];
    static int counter = 0;

    while ((dirent = readdir(dir)) != NULL){
        counter++;
        /* If it's current/parent directory skip */
        if(!NoDotAndDotDot(dirent))continue;

        //dest full path name
        sprintf(pathName,"%s/%s", fullPath, dirent->d_name);
        if(dirent->d_type != DT_DIR){
            if(remove(pathName) == 0)
                stats.delCount++;
            else 
                printf("Couldn't remove :\n");
            printf("%s\n", pathName);
        }

        //In case of directory remove all sub directories/files recursively
        else{

            DIR *nextdir;
            if((nextdir = opendir(pathName)) == NULL)
                return -1;

            deleteDir(nextdir, pathName);
            printf("%s\n",pathName);
            remove(pathName);
            stats.delCount++;

            closedir(nextdir);
        }
    }
    return 0;
}