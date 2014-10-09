/* decompress40.c
 * by Gabrielle Cella and Ann Bellinger, 10/15/13
 * Homework 4
 *
 * This is the implementation for decompress40 which calls all of the functions
 * necessary for decompressing an image. 
 */

#include <stdio.h>
#include "compress40.h"
#include "processes.h"
#include "bitpack.h"

static void store_compressed(int i, int j, UArray2_T uarray2, 
                             void *elem, void *cl);

/*Wrapper function for all of the intermediary image decompression functions.
  This function stores the intermediary components between the compress format
  and the RGB format. It also calls the free-ing functions, which are universal
  to compress and decompress.*/
void decompress40(FILE *input)
{
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                          &width, &height);
        assert(read == 2);

        int c = getc(input);
        assert(c == '\n');

        UArray2_T words = UArray2_new(height/2, width/2, sizeof(uint64_t));
        UArray2_map_row_major(words, store_compressed, &input);

        Pnm_components wordparts = decomp_word_array(words);
        UArray2_free(&words);

        Pnm_cv compvid = decomp_components_array(wordparts);
        wordparts_free(wordparts);

        Pnm_ppm decompressed = decomp_cv_array(compvid);
        compvid_free(compvid);

        Pnm_ppmwrite(stdout, decompressed);
        Pnm_ppmfree(&decompressed);
}

/*This is the apply function for reading from the input file. For each index in 
  the UArray2_T, this function reads the next word from input and stores the 
  word*/
static void store_compressed(int i, int j, UArray2_T uarray2, 
                             void *elem, void *cl)
{
        (void) i;
        (void) j;
        (void) uarray2;

        FILE *input = *((FILE**)cl);
        uint64_t word = 0;

        int lsb = INIT_LSB;
        int value = 0;

        while (lsb >= 0) {
                value = fgetc(input);
                if (value == -1) {
                        return;
                }
                word = Bitpack_newu(word, BITWIDTH, lsb, value%256);
                lsb -= BITWIDTH;
        }
        *(uint64_t *)elem = word;
}
