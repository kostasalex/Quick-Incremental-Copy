#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "globals.h"

/* Copying only modified or new files/directories from
   source to destination path.*/
int copyDir(DIR *dir_src, DIR *dir_dest,\
char *srcPath, char *destPath);


/* Copying a source file to destination and transfers all premissions */
int copyFile(char *srcFullName ,char *destFullName, int size);

/* Deleting from destination all no longer existing in source 
   files/directories */
int delete(DIR *dir_src, DIR *dir_dest,\
char *srcPath, char *destPath);


/* Deleting all files/directories from a given directory */
int deleteDir(DIR *dir, char* fullPath);