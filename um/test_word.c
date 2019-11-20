/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file tests the word interface
 *
 */

#include "word.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// array of registers is put here for now
uint32_t registers[8] = {0};

int main()
{
    uint32_t test_word = 1249035930; /* this is 0x4a72c69a */
    fprintf(stderr, "testing return_instruct %u\n", return_instruct(test_word));

    get_three_reg(test_word, &registers[2], &registers[5], &registers[7]);
    fprintf(stderr, "testing get_three_reg \n register1: %u\n register2: %u\n register3: %u\n", registers[2], registers[5], registers[7]);
    exit(0);
}