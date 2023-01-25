/* Modified whenever files/folders created */
#include <time.h>
struct Stats
{
    int copyCount;
    int delCount;
    int total;
    time_t start;
    int bytes;
    

}stats;

/* Modified by input handler */
struct Flags
{
    int verbose, deleted, links;

}flags;

