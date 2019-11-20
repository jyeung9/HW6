/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file contains our implementation for our instructions interface
 *
 */

#include "segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <bitpack.h>

#define BYTESIZE 8

/**
 * add
 * 
 * Parameters:  pointer to register ra, and values inside registers rb, rc
 * Return:      void
 * Purpose:     adds values in registers rb and rc, stores in register ra
 * CRE:         ra cannot be NULL
 */
void add(uint32_t *ra, uint32_t rb, uint32_t rc)
{
    assert(ra != NULL);
    *ra = rb + rc;
    return;
}

/**
 * multiply
 * 
 * Parameters:  pointer to register ra, and values inside registers rb, rc
 * Return:      void
 * Purpose:     multiplies values in registers rb and rc, stores in register ra
 * CRE:         ra should not be NULL
 */
void multiply(uint32_t *ra, uint32_t rb, uint32_t rc)
{
    assert(ra != NULL);
    *ra = rb * rc;
    return;
}

/**
 * divide
 * 
 * Parameters:  pointer to register ra, and values inside registers rb, rc
 * Return:      void
 * Purpose:     multiplies values in registers rb and rc, stores in register ra
 * CRE:         ra should not be NULL
 */
void divide(uint32_t *ra, uint32_t rb, uint32_t rc)
{
    assert(ra != NULL);
    *ra = rb / rc;
    return;
}

/**
 * bitwise_nand
 * 
 * Parameters:  pointer to register ra, and values inside registers rb, rc
 * Return:      void
 * Purpose:     performs bitwise nand on rb and rc, stores result in ra
 * CRE:         ra should not be NULL
 */
void bitwise_nand(uint32_t *ra, uint32_t rb, uint32_t rc)
{
    assert(ra != NULL);
    uint32_t and_result;
    and_result = rb & rc;
    *ra = (uint32_t)(~(and_result));
    return;
}

/**
 * load_val
 * 
 * Parameters:  pointer to register ra and values to be stored
 * Return:      void
 * Purpose:     adds values in registers rb and rc, stores in register ra
 * CRE:         ra should not be NULL
 */
void load_value(uint32_t *ra, uint32_t val)
{
    assert(ra != NULL);
    *ra = val;
    return;
}

/**
 * halt
 * 
 * Parameters:  none
 * Return:      void
 * Purpose:     stops computation, exits program
 * CRE:         none
 */
void halt()
{
    exit(0);
}

/**
 * map_segment
 * 
 * Parameters:  Register to store length and new address of segment, 
 *              references to unmapped indices and mapped segments
 * Return:      void
 * Purpose:     Will create a new mapped segment of all 0s. 
 * CRE:         rb should not be NULL, sequesnces should not be NULL
 */
void map_segment(uint32_t *rb, uint32_t rc, Seq_T mapped, Seq_T unmapped)
{
    assert(rb != NULL);
    assert(mapped != NULL);
    assert(unmapped != NULL);
    
    Seg_T temp = Seg_new(rc);
    for (int i = 0; i < (int)rc; i++) {
        Seg_append(temp, 0);
    }
    if (unmapped != NULL && Seq_length(unmapped) > 0) {
        *rb = (uint32_t)(uintptr_t)Seq_remlo(unmapped);
        Seq_put(mapped, *rb, temp);
    }
    else {
        *rb = Seq_length(mapped);
        Seq_addhi(mapped, temp);
    }

    return;
}

/**
 * unmap_segment
 * 
 * Parameters:  Register to store length and new address of segment, 
 *              references to unmapped indices and mapped segments
 * Return:      void
 * Purpose:     Will unmap a segment, append that to the end of the unmapped
 *              sequence. 
 * CRE:         mapped and unmapped should not be NULL
 */
void unmap_segment(uint32_t rc, Seq_T mapped, Seq_T unmapped)
{
    assert(mapped != NULL);
    assert(unmapped != NULL);
    Seg_free(Seq_get(mapped, rc));
    Seq_put(mapped, rc, NULL);
    void *temp = Seq_addhi(unmapped, (void *)(uintptr_t)rc);
    (void)temp;
    return;
}

/**
 * seg_load
 * 
 * Parameters:  pointer to register ra, index of segment and word within 
 *              segment
 * Return:      void
 * Purpose:     loads word from memory into register ra
 * CRE:         ra and mapped should not be NULL
 */
void seg_load(uint32_t *ra, uint32_t rb, uint32_t rc, Seq_T mapped)
{
    assert(ra != NULL);
    assert(mapped != NULL);
    *ra = Seg_get(Seq_get(mapped, rb), rc);
    return;
}

/**
 * seg_store
 * 
 * Parameters:  registers that store indexing of where to store and register 
 *              that holds word to be stored, mapped sequences
 * Return:      void
 * Purpose:     stores the value in rc into segment mapped[ra], word [rb].
 *              Will fail if out of bounds in any way
 * CRE:         mapped should not be NULL,  failure is there
 */
void seg_store(uint32_t ra, uint32_t rb, uint32_t rc, Seq_T mapped)
{
    assert(mapped != NULL);
    Seg_put(Seq_get(mapped, ra), rb, rc);
    return;
}

/**
 * conditional_move
 * 
 * Parameters:  register to compare with 0, register to load data from, register
 *              to load data into
 * Return:      void
 * Purpose:     if the value at rc is not 0, the value at rb will be stored into
 *              the value pointed to by ra. 
 * CRE:         ra should not be NULL
 */
void conditional_move(uint32_t *ra, uint32_t rb, uint32_t rc)
{
    assert(ra != NULL);
    if (rc != 0) {
        *ra = rb;
    }
    return;
}

/**
 * load_program
 * 
 * Parameters:  The mapped sequence and the index to get the segment from,
 *              and the new prog counter
 * Return:      void
 * Purpose:     It will take the segment located at mapped[rb], duplicate it 
 *              and store the duplicate in mapped[0], can break when its out of
 *              bounds
 * CRE:         mapped and count should not be NULL, but can break
 */
void load_program(Seq_T mapped, uint32_t rb, uint32_t rc, int *count)
{
    assert(count != NULL);
    assert(mapped != NULL);
    
    if (rb != 0) {
        Seg_T store_here = Seg_new(1);

        /* deep copy */
        for (int i = 0; i < Seg_length(Seq_get(mapped, rb)); i++) {
            Seg_append(store_here, Seg_get(Seq_get(mapped, rb), i));
        }
        Seg_free(Seq_get(mapped, 0));
        Seq_put(mapped, 0, store_here);
    }
    *count = rc - 1;
    return;
}

/**
 * input
 * 
 * Parameters:  the register to store the value in
 * Return:      void
 * Purpose:     It will take in user input and put it into the specified reg
 * CRE:         rc should not be NULL
 */
void input(uint32_t *rc)
{
    assert(rc != NULL);
    int user_in = getchar();
    if (user_in == EOF) {
        *rc = (uint32_t)(~0);
    }
    else {
        *rc = (uint32_t)user_in;
    }
    return;
}

/**
 * output
 * 
 * Parameters:  the register to get the value from
 * Return:      void
 * Purpose:     It will take the value from reg rc and print it out
 * CRE:         rc is greater than 255
 */
void output(uint32_t rc)
{
    assert(rc < 256);
    putchar(rc);
    return;
}