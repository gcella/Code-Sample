/* packword.c
 * by Gabrielle Cella and Ann Bellinger, 10/15/13
 * Homework 4
 *
 * Implementation of the compression and decompression functions that pack and 
 * unpack 32-bit words from and to a, b, c, d, PRchroma, and PBchroma values
 * respectively. 
 */

#include "processes.h"
#include "bitpack.h"

typedef A2Methods_UArray2 A2;

const int WORDSIZE = sizeof(uint64_t);
const unsigned DENOM = 255;

/*Width constants*/
const int PRWIDTH = 4;
const int PBWIDTH = 4;
const int AWIDTH = 6;
const int BWIDTH = 6;
const int CWIDTH = 6;
const int DWIDTH = 6;

/*LSB constants*/
const int PRLSB = 0;
const int PBLSB = 4;
const int DLSB = 8;
const int CLSB = 14;
const int BLSB = 20;
const int ALSB = 26;

static void fill_packedwords(int i, int j, A2 pixels, void *ptr, void *cl);
static void unpack_word(int i, int j, A2 pixels, void *ptr, void *cl);

/*Creates a new UArray2_T to store the packed 32-bit words. This function 
  returns the UArray2_T to compress40.c.*/
UArray2_T create_packedwords_array(Pnm_components wordparts)
{
        UArray2_T packword = UArray2_new(wordparts->height, wordparts->width,
                                         WORDSIZE);
        assert(packword != NULL);

        wordparts->methods->map_default(wordparts->pixels, fill_packedwords, 
                                        &packword);

        return packword;
}

/*This is the apply function for a Pnm_components array. This function calls
  the appropriate Bitpack_new function to store a, b, c, d, PRchroma, and
  PBchroma in the 32-bit word. It then stores the word in the corresponding 
  location in the UArray2_T, which is the cl.*/
static void fill_packedwords(int i, int j, A2 pixels, void *ptr, void *cl)
{
        (void) pixels;

        UArray2_T packword = (*(UArray2_T*)cl);
        Components comp = ((Components)ptr);

        uint64_t *temp = UArray2_at(packword, i, j);
        *temp = Bitpack_newu(*temp, PRWIDTH, PRLSB, comp->PRchroma);
        *temp = Bitpack_newu(*temp, PBWIDTH, PBLSB, comp->PBchroma);
        *temp = Bitpack_news(*temp, DWIDTH, DLSB, comp->d);
        *temp = Bitpack_news(*temp, CWIDTH, CLSB, comp->c);
        *temp = Bitpack_news(*temp, BWIDTH, BLSB, comp->b);
        *temp = Bitpack_newu(*temp, AWIDTH, ALSB, comp->a);
}

/*Creates a new Pnm_components struct with a Pnm_components array. This function
  returns the Pnm_components struct to decompress40.c*/
Pnm_components decomp_word_array(UArray2_T packedword) 
{
        Pnm_components wordparts = malloc(sizeof(*wordparts));
        assert(wordparts != NULL);

        wordparts->width = UArray2_numcols(packedword);
        wordparts->height = UArray2_numrows(packedword);
        wordparts->denominator = DENOM;
        wordparts->methods = uarray2_methods_plain;
        assert(wordparts->methods != NULL);

        wordparts->pixels = wordparts->methods->new(wordparts->width, 
                                                    wordparts->height,
                                                    sizeof(struct Components));

        A2Methods_mapfun *map = wordparts->methods->map_row_major;
        assert(map != NULL);

        map(packedword, unpack_word, &wordparts);

        return wordparts;
}

/*This is the apply function for a UArray2_T. This function calls the 
  appropriate Bitpack_get function to retrieve the a, b, c, d, PRchroma, and 
  PBchroma values from a 32-bit word. It then stores those values in a 
  Components struct and stores that struct in UArray2_T, which is in the cl.*/
static void unpack_word(int i, int j, A2 pixels, void *ptr, void *cl)
{       
        (void) pixels;

        Pnm_components wordparts = *((Pnm_components*)cl);
        uint64_t *word = ((uint64_t*)ptr);

        Components comp = wordparts->methods->at(wordparts->pixels, i, j);

        comp->PBchroma = Bitpack_getu(*word, PBWIDTH, PBLSB);
        comp->PRchroma = Bitpack_getu(*word, PRWIDTH, PRLSB);
        comp->d = Bitpack_gets(*word, DWIDTH, DLSB);
        comp->c = Bitpack_gets(*word, CWIDTH, CLSB);
        comp->b = Bitpack_gets(*word, BWIDTH, BLSB);
        comp->a = Bitpack_getu(*word, AWIDTH, ALSB);
}
