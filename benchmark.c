#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "md5.h"


int main(int argc, char *argv[])
{
    const char *msg = "The quick brown fox jumps over the lazy dog";

    size_t len = strlen(msg);
    uint32_t result[4] = {0};

    struct timeval t1, t2, t_result;

    gettimeofday(&t1, NULL);
    for (size_t i = 0; i < 100000; i++)
        md5((const uint8_t *)msg, len, result);
    gettimeofday(&t2, NULL);

    timersub(&t2, &t1, &t_result);
    double hash_per_sec = 100000.0 / ((double)t_result.tv_sec + ((double)t_result.tv_usec / 1000000));
    printf("Time spend on 100000 hashes: %ld.%06ld s (%ld hashes per sec)\n", t_result.tv_sec, t_result.tv_usec, (size_t)hash_per_sec);

    return 0;
}