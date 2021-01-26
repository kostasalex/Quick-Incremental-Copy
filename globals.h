/* Modified whenever files/folders created */
#include <time.h>
struct Stats
{
    int dirCount;
    time_t start;

}stats;

/* Modified by input handler */
struct Flags
{
    int verbose, deleted, links;

}flags;

