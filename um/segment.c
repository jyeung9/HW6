/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file contains our implementation for our segment interface 
 *
 * Segments will be pointers to sequences that will contain word elements
 *      which are uint32_t types
 * 
 * CRES are considered under the idea that the segment interface can function ad
 * a standalone interface. 
 */

#include "segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/** 
 * Seg_new
 * 
 * Parameters:  An estimated length of the new segment 
 * Return:      A new segment
 * Purpose:     Will take in the predicted length and will construct a segment
 *              of that length (basically a pointer to a Seq_T)
 * CRE:         word_len (the number of elements) cannot be less than 0
 */
Seg_T Seg_new(int word_len)
{
    assert(word_len >= 0);
    return Seq_new(word_len);
}

/** 
 * Seg_free
 * 
 * Parameters:  The segment to be freed
 * Return:      Void
 * Purpose:     Will free the segment, checks to make sure it't not already NULL
 * CRE:         if seg == NULL
 */
void Seg_free(Seg_T seg)
{
    assert(seg != NULL);
    Seq_free(&seg);
    
    return;
}

/** 
 * Seg_put
 * 
 * Parameters:  The segment we're interacting with, the index to insert at, the
 *              word to insert
 * Return:      Void
 * Purpose:     Will insert the specified word at the index listed
 * CRE:         If the index is less than 0 and greater
 *              than the length of the segment
 *              segment is not NULL
 */
void Seg_put(Seg_T seg, int i, uint32_t value)
{
    assert(seg != NULL);
    assert(i >= 0 && i < Seg_length(seg));
    Seq_put(seg, i, (void *)(uintptr_t)value);
    return;
}

/** 
 * Seg_get
 * 
 * Parameters:  The segment to be interacted with, the index to get the value 
 *              from
 * Return:      Void
 * Purpose:     Will get the value at the specified index
 * CRE:         If the segment is NULL, if the index is out of bounds
 */
uint32_t Seg_get(Seg_T seg, int i)
{
    assert(seg != NULL);
    assert(i >= 0 && i < Seg_length(seg));
    return (uint32_t)(uintptr_t)Seq_get(seg, i);
}

/** 
 * Seg_append
 * 
 * Parameters:  The segment, the word to insert
 * Return:      Void
 * Purpose:     Will insert the word at the end of the segment
 * CRE:         If the segment is NULL
 */
void Seg_append(Seg_T seg, uint32_t value)
{
    assert(seg != NULL);
    Seq_addhi(seg, (void *)(uintptr_t)value);
    return;
}

/** 
 * Seg_length
 * 
 * Parameters:  The segment
 * Return:      The length of the segment
 * Purpose:     Will find the length of the segment and return
 * CRE:         If the segment is NULL
 */
int Seg_length(Seg_T seg)
{
    assert(seg != NULL);
    return Seq_length(seg);
}