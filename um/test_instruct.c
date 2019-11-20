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

#include "instruct.h"
#include "memory.h"

// #include <bitpack.h>
#include <seq.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// array of registers is put here for now
uint32_t registers[8] = {0};

int main()
{
        //uint32_t test_word = 1249035930; /* this is 0x4a72c69a */
        registers[2] = 5;
        registers[5] = 8;

        // ADDITION
        add(&registers[1], registers[2], registers[5]);
        fprintf(stderr, "*testing add*: \n sum: %u\n regb: %u\n regc: %u\n",
                registers[1], registers[2], registers[5]);

        // MULTIPLICATION + we see that information can be overwritten here
        multiply(&registers[1], registers[2], registers[5]);
        fprintf(stderr, "Overwriting registers[1] with product\n");
        fprintf(stderr, "*testing multiply*: \n prod: %u\n regb: %u\n regc: %u\n",
                registers[1], registers[2], registers[5]);

        // DIVISION:
        divide(&registers[2], registers[1], registers[5]);
        fprintf(stderr, "*testing divide*: \n quotient: %u\n regb: %u\n regc: %u\n",
                registers[2], registers[1], registers[5]);
        // DIVISION OF VALUES THAT DON'T FIT INTO EACH OTHER NICELY
        registers[2] = 5;
        registers[3] = 2;
        divide(&registers[1], registers[2], registers[3]);
        fprintf(stderr, "*testing divide*: \n quotient: %u\n regb: %u\n regc: %u\n",
                registers[1], registers[2], registers[3]);
        // DIVISION OF SMALLER NUMERATOR
        divide(&registers[1], registers[3], registers[2]);
        fprintf(stderr, "*testing divide*: \n quotient: %u\n regb: %u\n regc: %u\n",
                registers[1], registers[2], registers[3]);

        // BITWISE NAND
        registers[3] = 0;
        registers[4] = 3;

        fprintf(stderr, "this is reg[3]: %u\n", registers[3]);
        registers[5] = 3;
        bitwise_nand(&(registers[3]), registers[4], registers[5]);
        fprintf(stderr, "*testing bitwise_nand*: \n result: %u\n regb: %u\n regc: %u\n",
                registers[3], registers[4], registers[5]);

        // MAPPED AND UNMAPPED
        // need to test for elements and length of sequences after movement
        FILE *fp = fopen("add.um", "r");
        Memory mem = init_um(fp);
        registers[6] = 2;
        fprintf(stderr, "length of unmapped seq %d\n", Seq_length(mem->unmapped));
        map_segment(&registers[5], registers[6], mem->mapped, mem->unmapped);
        if (Seq_get(mem->mapped, 1) == NULL)
        {
                fprintf(stderr, "BAAD\n");
        }
        fprintf(stderr, "length of mapped seq: %d\n", Seq_length(mem->mapped));
        unmap_segment(registers[5], mem->mapped, mem->unmapped);
        fprintf(stderr, "length of mapped seq: %d\n", Seq_length(mem->mapped));
        if (Seq_get(mem->mapped, 1) == NULL)
        {
                fprintf(stderr, "YEEEEt\n");
        }
        fprintf(stderr, "length of unmapped seq %d\n", Seq_length(mem->unmapped));

        // SEGLOAD AND SEGSTORE
        // mapped has length = 2 but only 1 segment
        registers[6] = 0;
        fprintf(stderr, "CHECK IT: %u\n", (word)(uintptr_t)Seg_get(Seq_get(mem->mapped, registers[6]), registers[5]));
        seg_load(&registers[1], registers[6], registers[5], mem->mapped);
        fprintf(stderr, "*testing seg_load* \n value loaded into reg: %u\n", registers[1]);

        //map_segment(&registers[3], registers[4], mem->mapped, mem->unmapped);
        fprintf(stderr, "*testing seg_store pt1*\n value stored into mem = %u\n",
                (word)(uintptr_t)Seg_get(Seq_get(mem->mapped, registers[6]), registers[5]));
        registers[2] = 1073741907;
        seg_store(registers[6], registers[5], registers[2], mem->mapped);
        fprintf(stderr, "*testing seg_store*\n value stored into mem = %u\n",
                (word)(uintptr_t)Seg_get(Seq_get(mem->mapped, registers[6]), registers[5]));

        // LOAD PROGRAM
        word instruct_1 = 1342177363; // division in decimal
        word instruct_2 = 3556769794; // load value
        // word instruct_3 = 2348295329;
        // Seg_T test_seg = Seg_new(3);
        // Seg_append(test_seg, instruct_1);
        // Seg_append(test_seg, instruct_2);
        map_segment(&registers[7], 2, mem->mapped, mem->unmapped);
        Seg_append(Seq_get(mem->mapped, registers[7]), instruct_2);
        Seg_append(Seq_get(mem->mapped, registers[7]), instruct_1);
        load_program(mem->mapped, 1); // should get load value instruction
        fprintf(stderr, "At segment 0: %u\n", (word)(uintptr_t)Seg_get(Seq_get(mem->mapped, 0), 0));

        // LOAD VALUE
        load_value(&registers[6], 45);
        fprintf(stderr, "*testing load value*\n register value: %u\n", registers[3]);

        // CONDITIONAL MOVE
        // ra = 0, rb = 1, rc = 2
        fprintf(stderr, "*testing conditional move*\n");
        registers[0] = 4;
        registers[1] = 3;
        registers[2] = 2;
        fprintf(stderr, "ra is %u, rb is %u, rc is %u\n", registers[0], registers[1], registers[2]);
        conditional_move(&registers[0], registers[1], registers[2]);
        fprintf(stderr, "\nra should be 3\n");
        fprintf(stderr, "ra is %u, rb is %u, rc is %u\n", registers[0], registers[1], registers[2]);
        registers[2] = 0;
        fprintf(stderr, "\nra should be 3\n");
        fprintf(stderr, "ra is %u, rb is %u, rc is %u\n", registers[0], registers[1], registers[2]);

                // INPUT
        // account for '\n' when "return"
        // problem with casting "user_in" as "int" -> unsigned32
        // reads in values as hex, prints out as decimal
        // find out how to deal with newline chars
        word user_input = 0;

        fprintf(stderr, "testing EOF\n");
        input(&user_input);
        fprintf(stderr, "user input is %u\n", user_input);
        fprintf(stderr, "*testing input*\n");
        input(&user_input);
        fprintf(stderr, "user input is %u\n", user_input);

        // OUTPUT
        fprintf(stderr, "********%d\n", user_input);
        output(user_input);

        // freeing
        fprintf(stdout, "before free_all\n");
        free_all(mem);
        fprintf(stdout, "after free_all\n");
        fclose(fp);

        exit(0);
}