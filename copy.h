#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "globals.h"

int copyDir(DIR *dir_src, DIR *dir_dest,\
char *srcPath, char *destPath);
int NoDotAndDotDot(struct dirent *d);