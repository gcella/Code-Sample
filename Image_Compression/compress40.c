/* compress40.c
 * by Gabrielle Cella and Ann Bellinger, 10/15/13
 * Homework 4
 *
 * Implementation for the image compression wrapper function, which calls all of
 * the intermediary functions, and the function that writes the packed words 
 * to standard out.
 */

#include <stdio.h>
#include <inttypes.h>
#include "compress40.h"
#include "processes.h"
#include "bitpack.h"

static void write_compressed(int i, int j, UArray2_T uarray2, 
                             void *elem, void *cl);

/*Wrapper function for all of the intermediary image compression functions. This
  function stores the intermediary components between the original and
  compressed image. It also calls the free-ing functions, which are universal to
  compress and decompress.*/
void compress40(FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods != NULL);

        Pnm_ppm original = Pnm_ppmread(input, methods);
        assert(original != NULL);

        Pnm_cv compvid = create_cv_array(original);
        Pnm_ppmfree(&original);

        Pnm_components wordparts = create_components_array(compvid);
        compvid_free(compvid);

        UArray2_T packword = create_packedwords_array(wordparts);
        wordparts_free(wordparts);

        printf("COMP40 Compressed image format 2\n%u %u\n", 
               UArray2_numcols(packword) * 2, UArray2_numrows(packword) * 2);
        UArray2_map_row_major(packword, write_compressed, NULL);
        UArray2_free(&packword);
}

/*Writes the compressed image (the packed words) to standard out*/
static void write_compressed(int i, int j, UArray2_T uarray2, 
                             void *elem, void *cl)
{
        (void) i;
        (void) j;
        (void) uarray2;
        (void) cl;

        uint64_t word = *(uint64_t*)elem;

        int lsb = INIT_LSB; /* Reads out in Big-Endian format */
        uint64_t bit = 0;
        while(lsb >= 0)
        {
                bit = Bitpack_getu(word, BITWIDTH, lsb);
                putchar((int)bit);
                lsb -= BITWIDTH;
        }
}

