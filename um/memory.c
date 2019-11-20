/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file contains our implementation for the memory interface
 *
 */

#include "memory.h"
#include "instruct.h"
#include <assert.h>
#include <bitpack.h>
#include <um-dis.h>
#define BYTESIZE 8

// WHAT IS INACTIVATE/ACTIVATE OPCODE FOR??
typedef enum Um_opcode
{
    CMOV = 0,
    SLOAD,
    SSTORE,
    ADD,
    MUL,
    DIV,
    NAND,
    HALT,
    MAP,
    UNMAP,
    OUT,
    IN,
    LOADP,
    LV
} Um_opcode;

/**
 * This function will ensure all the data from the memory struct is properly
 * freed
 */
static void free_all(Memory mem);

/**
 * This function will contain all switch statements for the 
 * instructions
 */
static void execute_instruct(Memory mem, word code);

/** 
 * init_um
 * 
 * Parameters: Takes in a pointer to a memory struct that contains our memory,
 *              registers and prog_counter. Also takes in file to read in 
 *              instructions from
 * Returns:     Nothing
 * Purpose:     This will initialize all of the memory-storing data types for 
 *              running our UM
 * CRE:         The input file cannot be NULL, the formed segment cant be NULL
 */
Memory init_um(FILE *input)
{
    assert(input != NULL);
    Memory mem;
    mem = malloc(sizeof(*mem));
    assert(mem != NULL);
    /* will initialize registers and prog counter to 0 */
    mem->prog_counter = 0;
    for (int i = 0; i < BYTESIZE; i++) {
        mem->registers[i] = 0;
    }
    mem->unmapped = Seq_new(3);
    Seg_T seg = Seg_new(3); /* initializing the segment to be inserted */

    int c = getc(input);
    word value = 0; /* this will be word to be appended */
    int safe = 0;   /* this will act as a boolean */
    while (c != EOF) {
        for (int i = 24; i >= 0; i -= BYTESIZE) {
            if (c == EOF) {
                safe++; /* this will tell the prog not to add the word */
                break;
            }
            value = Bitpack_newu(value, BYTESIZE, i, (word)c);
            c = getc(input);
        }
        if (safe != 1) { /* the word is safe to add */
            Seg_append(seg, value);
            value = 0; /* just in case reset */
        }
    }

    assert(seg != NULL);

    /* this adds the first element to the mapped sequence */
    mem->mapped = Seq_new(3);
    Seg_T temp = (Seg_T)Seq_addhi(mem->mapped, seg);
    (void)temp;
    return mem;
}

/**
 * free_all
 * 
 * Parameters: Reference to the memory that needs to be freed.
 * Returns: Nothing
 * Purpose: Will free all memory associated with the struct used
 * CRE: Mem cannot be NULL
 */
static void free_all(Memory mem)
{
    assert(mem != NULL);
    if (mem->unmapped != NULL) {
        Seq_free(&(mem->unmapped));
    }

    if (mem->mapped != NULL) {
        for (int i = 0; i < Seq_length(mem->mapped); i++) {
            Seg_T temp = Seq_get(mem->mapped, i);
            if (temp != NULL) {
                Seg_free(temp);
            }
        }
        Seq_free(&(mem->mapped));
    }

    free(mem);
}

/** 
 * run_program
 * 
 * Parameters: Reference to the memory that contains the instructions
 * Returns: Nothing
 * Purpose: This will go through each word and execute the necessary
 *          instruction. Will increment the prog counter here
 * CRE: mem cannot be NULL
 */
void run_program(Memory mem)
{
    assert(mem != NULL);

    while (true) {
        word instruct = Seg_get(Seq_get(mem->mapped, 0), mem->prog_counter);
        execute_instruct(mem, instruct);
        mem->prog_counter++;
    }
}

/**
 * execute_instruct
 * 
 * Parameters: Reference to the memory that contains the instructions and the 
 *              current instruction
 * Returns: Nothing
 * Purpose: Will call the corresponding switch statement depending on the
 *          instruction. Will only change the prog_counter if the instruction
 *          specifies it
 * CRE: mem cannot be NULL
 */
static void execute_instruct(Memory mem, word instruction)
{
    assert(mem != NULL);
    word op_code = return_instruct(instruction);
    word ra = 0;
    if (op_code == LV) {
        word value = 0;
        get_load_val(instruction, &ra, &value);
        load_value(&(mem->registers[ra]), value);
        return;
    }
    word rb = 0;
    word rc = 0;
    get_three_reg(instruction, &ra, &rb, &rc);
    switch (op_code) {
    case CMOV:
        conditional_move(&(mem->registers[ra]), mem->registers[rb],
                         mem->registers[rc]);
        break;
    case SLOAD:
        seg_load(&(mem->registers[ra]), mem->registers[rb],
                 mem->registers[rc], mem->mapped);
        break;
    case SSTORE:
        seg_store(mem->registers[ra], mem->registers[rb],
                  mem->registers[rc], mem->mapped);
        break;
    case ADD:
        add(&(mem->registers[ra]), mem->registers[rb],
            mem->registers[rc]);
        break;
    case MUL:
        multiply(&(mem->registers[ra]), mem->registers[rb],
                 mem->registers[rc]);
        break;
    case DIV:
        divide(&(mem->registers[ra]), mem->registers[rb],
               mem->registers[rc]);
        break;
    case NAND:
        bitwise_nand(&(mem->registers[ra]), mem->registers[rb],
                     mem->registers[rc]);
        break;
    case HALT:
        free_all(mem);
        halt();
        break;
    case OUT:
        output(mem->registers[rc]);
        break;
    case IN:
        input(&(mem->registers[rc]));
        break;
    case LOADP:
        load_program(mem->mapped, mem->registers[rb], mem->registers[rc],
                     &(mem->prog_counter));
        break;
    case MAP:
        map_segment(&(mem->registers[rb]), mem->registers[rc], mem->mapped,
                    mem->unmapped);
        break;
    case UNMAP:
        unmap_segment(mem->registers[rc], mem->mapped, mem->unmapped);
        break;
    }
}