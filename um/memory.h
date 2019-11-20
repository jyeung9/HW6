/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file is our header file for the memory interface
 *
 */

#include "instruct.h"
#include "word.h"

/* our memory struct */
typedef struct
{
    word registers[8];
    Seq_T mapped;
    Seq_T unmapped;
    int prog_counter;
} * Memory;

/**
 * This funciton will return a pointer to all of our necessary data for 
 * initializing the UM
 */
Memory init_um(FILE *input);

/**
 * This funciton will run the program and incremene the prog_counter
 */
void run_program(Memory mem);