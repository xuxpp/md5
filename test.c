#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

int main(int argc, char *argv[])
{
    bool result = false;
    FILE *fp = fopen(argv[1], "r");
    // Get file size:
    fseek(fp, 0, SEEK_END);
    size_t f_size = ftell(fp);
    rewind(fp);
    
    // Read file
    uint8_t *buf = calloc(1, f_size);
    size_t num_read = fread(buf, 1, f_size, fp);

    // Get md5
    uint32_t hash[4] = {0};
    md5(buf, f_size, hash);

    const char ref[] = {0x29, 0x42, 0xbf, 0xab, 0xb3, 0xd0, 0x53, 0x32, 0xb6, 0x6e, 0xb1, 0x28, 0xe0, 0x84, 0x2c, 0xff};

    result = (memcmp(ref, hash, sizeof(ref)) == 0);

    free(buf);
    fclose(fp);

    return result ? 0 : 1;
}