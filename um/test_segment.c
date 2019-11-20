/**
 * 
 * COMP40 
 * Fall 2019
 * HW: 6 -- The UM
 * Julie Yeung (jyeung03) and Zetty Cho (echo02)
 * 
 * This file tests the segment interface
 *
 */

#include "segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
    
    Seg_T test_segment = Seg_new(3);
    word steve = 5;
    word steve_friend = 3;

    // Seg_put(test_segment, 0, steve); // should be CRE, it IS

    Seg_append(test_segment, steve);

    fprintf(stderr, "First element : %u\n", Seg_get(test_segment, 0));

    Seg_put(test_segment, 0, steve_friend);

    fprintf(stderr, "First element is now : %u\n", Seg_get(test_segment, 0));

    Seg_append(test_segment, steve);

    fprintf(stderr, "First element is : %u\n", Seg_get(test_segment, 0));

    fprintf(stderr, "Second element is  : %u\n", Seg_get(test_segment, 1));

    fprintf(stderr, "Seg length is  : %d\n", Seg_length(test_segment));

    Seg_free(test_segment);

    exit(0);
}