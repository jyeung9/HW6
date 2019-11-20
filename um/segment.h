/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file is our header file for our segment interface 
 *
 */

#ifndef SEGMENT_H_
#define SEGMENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <seq.h>
// #include "word.h"

typedef struct Seq_T *Seg_T;

/**
 * Will return reference to new Segment of length word_len
 */
Seg_T Seg_new(int word_len);

/**
 * Will free all elements within the segment and the segment itself
 */
void Seg_free(Seg_T seg);

/**
 * Will put the specified word in the specified index of the specified segment
 */
void Seg_put(Seg_T seg, int i, uint32_t value);

/**
 * Will get the word in the specified segment at the index (i)
 */
uint32_t Seg_get(Seg_T seg, int i);

/**
 * Will add the specified word to the end of the segment
 */
void Seg_append(Seg_T seg, uint32_t value);

/** 
 * Will return the length of the segment
 */
int Seg_length(Seg_T seg);

#endif