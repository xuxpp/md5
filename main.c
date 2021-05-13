#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "md5.h"

static void print_md5(const char* f_name, uint8_t *md5)
{
	printf("%s:", f_name);
    for(size_t i = 0; i < 16; ++i){
		printf("%02x", md5[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen("yuyang.pdf", "r");
    // Get file size:
    fseek(fp, 0, SEEK_END);
    size_t f_size = ftell(fp);
    rewind(fp);
    
    // Read file
    uint8_t *buf = calloc(1, f_size);
    size_t num_read = fread(buf, 1, f_size, fp);

    // Get md5
    uint32_t result[4] = {0};
    md5(buf, f_size, result);

    // Print md5
    print_md5("yuyang.pdf", (uint8_t *)result);

    free(buf);
    fclose(fp);

    return 0;
}