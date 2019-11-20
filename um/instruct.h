/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file is our header file for our instructions interface
 *
 */

#ifndef INSTRUCT_H_
#define INSTRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "segment.h"

/**
 * this takes the values from registers rb and rc and puts the sum in ra
 */
void add(uint32_t *ra, uint32_t rb, uint32_t rc);

/**
 * this takes the values from registers rb and rc and puts the product % 2^32
 * in ra
 */
void multiply(uint32_t *ra, uint32_t rb, uint32_t rc);

/**
 * this takes the values from registers rb and rc and puts the quotient in ra
 */
void divide(uint32_t *ra, uint32_t rb, uint32_t rc);

/**
 * this performs a bitwise AND on registers rb and rc, then 
 * derives the complement of that result, and stores that complement
 * into register ra
*/
void bitwise_nand(uint32_t *ra, uint32_t rb, uint32_t rc);

/** 
 * this will take the value specified and input it into register ra
 */
void load_value(uint32_t *ra, uint32_t val);

/**
 * this takes the word at word offset rc, and segment at segment rb
 * and puts it into register ra
 */
void seg_load(uint32_t *ra, uint32_t rb, uint32_t rc, Seq_T mapped);

/**
 * Will create a new segment with the specified length and store the address
 * in register rb
 */
void map_segment(uint32_t *rb, uint32_t rc, Seq_T mapped, Seq_T unmapped);

/**
 * Will unmap the specified segment and add th index to unmapped
 */
void unmap_segment(uint32_t rc, Seq_T mapped, Seq_T unmapped);

/**
 * Will take the word located in register rc and store in mapped[ra][rb]
 */
void seg_store(uint32_t ra, uint32_t rb, uint32_t rc, Seq_T mapped);

/**
 * If rc != 0, the value in ra = rb
 */
void conditional_move(uint32_t *ra, uint32_t rb, uint32_t rc);

/**
 * The segment at rb in mapped is duplicated and set into mapped[0]
 */
void load_program(Seq_T mapped, uint32_t rb, uint32_t rc, int *count);

/**
 * reads in the user input and stores in register rc
 */
void input(uint32_t *rc);

/**
 * takes the value in register rc and prints out
 */
void output(uint32_t rc);

/**
 * ends the program
 */
void halt();

#endif