/* wordelements.c
 * by Gabrielle Cella and Ann Bellinger, 10/15/13
 * Homework 4
 * 
 * Implementation of the compression and decompression functions that convert
 * to and from Y, PB, and PR values and a, b, c, d, PRchroma, and PBchroma 
 * values. 
 */

#include <math.h>
#include "processes.h"
#include "arith40.h"

const float BCD_UPPER = 0.3;
const float BCD_LOWER = -0.3;

const float QUANT_A = 63.0;
const float QUANT_BCD = 103.0;

typedef A2Methods_UArray2 A2;

static void fill_components_array(int i, int j, A2 pixels, void *ptr, void *cl);
static float avg4(float v1, float v2, float v3, float v4);
static float compute_a(float y1, float y2, float y3, float y4);
static float compute_b(float y1, float y2, float y3, float y4);
static float compute_c(float y1, float y2, float y3, float y4);
static float compute_d(float y1, float y2, float y3, float y4);
static unsigned quant_a(float a);
static int quant_bcd(float n);
static void fill_cv_decomp(int i, int j, A2 pixels, void *ptr, void *cl);
static float compute_Y1(unsigned a, int b, int c, int d);
static float compute_Y2(unsigned a, int b, int c, int d);
static float compute_Y3(unsigned a, int b, int c, int d);
static float compute_Y4(unsigned a, int b, int c, int d);

/*This function takes in a Pnm_cv struct with a UArray2b_T of cv_info structs
  and returns a Pnm_components struct with a UArray2_T of Components structs.
  This function returns the Pnm_components struct to compress40.c*/
Pnm_components create_components_array(Pnm_cv compvid)
{
        Pnm_components wordparts = malloc(sizeof(*wordparts));
        assert(wordparts != NULL);

        wordparts->width = compvid->width / BLOCKSIZE;
        wordparts->height = compvid->height / BLOCKSIZE;
        wordparts->denominator = compvid->denominator;
        wordparts->methods = uarray2_methods_plain;
        assert(wordparts->methods != NULL);

        wordparts->pixels = wordparts->methods->new(wordparts->width,
                                                    wordparts->height, 
                                                    sizeof(struct Components));

        A2Methods_mapfun *map = wordparts->methods->map_row_major; 
        assert(map != NULL);

        map(wordparts->pixels, fill_components_array, &compvid);

        return wordparts;
}

/*This is the apply function used with the array of Components in 
  Pnm_components. This function calls functions to convert the Y, PR, and PB 
  values to a, b, c, d, PBchroma, and PRchroma values and store these values in
  a Components struct. The ptr is then updated with the newly created Components
  struct.*/ 
static void fill_components_array(int i, int j, A2 pixels, void *ptr, void *cl)
{
        (void) pixels;

        Pnm_cv compvid = *((Pnm_cv*)cl);

        cv_info v1 = compvid->methods->at(compvid->pixels, 
                                          (i * BLOCKSIZE), (j * BLOCKSIZE));
        cv_info v2 = compvid->methods->at(compvid->pixels, 
                                          (i * BLOCKSIZE) + 1, (j * BLOCKSIZE));
        cv_info v3 = compvid->methods->at(compvid->pixels, 
                                          (i * BLOCKSIZE), (j * BLOCKSIZE) + 1);
        cv_info v4 = compvid->methods->at(compvid->pixels, 
                                          (i * BLOCKSIZE) + 1, 
                                          (j * BLOCKSIZE) + 1);

        Components comp = ((Components)ptr);
        comp->PBchroma = Arith40_index_of_chroma(avg4(v1->Pb, v2->Pb, 
                                                      v3->Pb, v4->Pb));
        comp->PRchroma = Arith40_index_of_chroma(avg4(v1->Pr, v2->Pr, 
                                                      v3->Pr, v4->Pr));
        comp->a = quant_a(compute_a(v1->Y, v2->Y, v3->Y, v4->Y));
        comp->b = quant_bcd(compute_b(v1->Y, v2->Y, v3->Y, v4->Y));
        comp->c = quant_bcd(compute_c(v1->Y, v2->Y, v3->Y, v4->Y));
        comp->d = quant_bcd(compute_d(v1->Y, v2->Y, v3->Y, v4->Y));
}

/* This function roduces the average of four floats, it is used to average all 
   of the Y values in a 2x2 block*/
static float avg4(float v1, float v2, float v3, float v4)
{
        return (v1 + v2 + v3 + v4) / (float)(BLOCKSIZE * BLOCKSIZE);
}

/*Uses discrete cosine transformation to compute a*/
static float compute_a(float y1, float y2, float y3, float y4)
{
        float rvalue = (y4 + y3 + y2 + y1) / (float)(BLOCKSIZE * BLOCKSIZE);
        if (rvalue > 1) {
                rvalue = 1;     /* cap values at 0 and 1 */
        }
        if (rvalue < 0) {
                rvalue = 0;
        }

        return rvalue;
}

/*Uses discrete cosine transformation to compute b*/
static float compute_b(float y1, float y2, float y3, float y4)
{
        float rvalue = (y4 + y3 - y2 - y1) / (float)(BLOCKSIZE * BLOCKSIZE);
        if (rvalue > BCD_UPPER) {
                rvalue = BCD_UPPER;
        }
        if (rvalue < BCD_LOWER) {
                rvalue = BCD_LOWER;
        }

        return rvalue;
}

/*Uses discrete cosine transformation to compute c*/
static float compute_c(float y1, float y2, float y3, float y4)
{       
        float rvalue = (y4 - y3 + y2 - y1) / (float)(BLOCKSIZE * BLOCKSIZE);
        if (rvalue > BCD_UPPER) {
                rvalue = BCD_UPPER;
        }
        if (rvalue < BCD_LOWER) {        /* cap value so it lies within */
                rvalue = BCD_LOWER;      /* the set boundaries for b, c, and d*/
        }

        return rvalue;
}

/*Uses discrete cosine transformation to compute d*/
static float compute_d(float y1, float y2, float y3, float y4)
{

        float rvalue = (y4 - y3 - y2 + y1) / (float)(BLOCKSIZE * BLOCKSIZE);
        if (rvalue > BCD_UPPER) {
                rvalue = BCD_UPPER;
        }
        if (rvalue < BCD_LOWER) {
                rvalue = BCD_LOWER;
        }

        return rvalue;
}

/*Quantize a by turning the small float value of a into an integer that
  fits in/occupies all of a's width in the codeword by multiplying by the
  constant QUANT_A */
static unsigned quant_a(float a)
{
        return (unsigned)roundf(QUANT_A * a);
}

static int quant_bcd(float n)
{
        int rvalue = n * QUANT_BCD;
        if (rvalue > (int)(QUANT_BCD * BCD_UPPER)) {
                rvalue = (int)(QUANT_BCD * BCD_UPPER);
        }
        if (rvalue < (int)(QUANT_BCD * BCD_LOWER)) {
                rvalue = (int)(QUANT_BCD * BCD_LOWER);
        }

        return rvalue;
}

/*This function takes in a Pnm_components struct with a UArray2_T of Components
  structs and returns a Pnm_cv struct with a UArray2b_T of cv_info structs. This
  function returns the Pnm_cv struct to decompress40.c*/
Pnm_cv decomp_components_array(Pnm_components wordparts)
{
        Pnm_cv compvid = malloc(sizeof(*compvid));
        assert(compvid != NULL);

        compvid->width = wordparts->width * BLOCKSIZE; /*width = decompressed 
                                                         image width*/
        compvid->height = wordparts->height * BLOCKSIZE; /*height = decompressed
                                                           image height */
        compvid->denominator = wordparts->denominator;
        compvid->methods = uarray2_methods_blocked;
        assert(compvid->methods != NULL);

        compvid->pixels = 
                compvid->methods->new_with_blocksize(compvid->width,
                                                     compvid->height,
                                                     sizeof(struct cv_info),
                                                     BLOCKSIZE);

        A2Methods_mapfun *map = wordparts->methods->map_row_major;
        assert(map != NULL);

        map(wordparts->pixels, fill_cv_decomp, &compvid);

        return compvid;
}

/*This is the apply function for the Pnm_components array. This function calls 
  functions to convert the a, b, c, d, PBchroma, and PRchroma values to Y, PR, 
  and PB values and stores these values in a cv_info struct. That struct is then
  stored in the UArray2b that is in the cl*/
static void fill_cv_decomp(int i, int j, A2 pixels, void *ptr, void *cl)
{
        (void) pixels;

        Pnm_cv compvid = *((Pnm_cv*)cl);
        Components temp = ((Components)ptr);

        cv_info v1 = compvid->methods->at(compvid->pixels, 
                                          (i * BLOCKSIZE), (j * BLOCKSIZE));
        cv_info v2 = compvid->methods->at(compvid->pixels,
                                          (i * BLOCKSIZE) + 1, (j * BLOCKSIZE));
        cv_info v3 = compvid->methods->at(compvid->pixels, 
                                          (i * BLOCKSIZE), (j * BLOCKSIZE) + 1);
        cv_info v4 = compvid->methods->at(compvid->pixels, 
                                          (i * BLOCKSIZE) + 1, 
                                          (j * BLOCKSIZE) + 1);

        v1->Y = compute_Y1(temp->a, temp->b, temp->c, temp->d);
        v1->Pb = Arith40_chroma_of_index(temp->PBchroma);       
        v1->Pr = Arith40_chroma_of_index(temp->PRchroma);

        v2->Y = compute_Y2(temp->a, temp->b, temp->c, temp->d);
        v2->Pb = Arith40_chroma_of_index(temp->PBchroma);
        v2->Pr = Arith40_chroma_of_index(temp->PRchroma);

        v3->Y = compute_Y3(temp->a, temp->b, temp->c, temp->d);
        v3->Pb = Arith40_chroma_of_index(temp->PBchroma);
        v3->Pr = Arith40_chroma_of_index(temp->PRchroma);

        v4->Y = compute_Y4(temp->a, temp->b, temp->c, temp->d);
        v4->Pb = Arith40_chroma_of_index(temp->PBchroma);
        v4->Pr = Arith40_chroma_of_index(temp->PRchroma);
}

static float compute_Y1(unsigned a, int b, int c, int d)
{
        float rvalue = a/QUANT_A - b/QUANT_BCD - c/QUANT_BCD + d/QUANT_BCD;
        if (rvalue > 1) {
                rvalue = 1;
        }
        if (rvalue < 0) {
                rvalue = 0;
        }

        return rvalue;
}
static float compute_Y2(unsigned a, int b, int c, int d)
{
        float rvalue = a/QUANT_A - b/QUANT_BCD + c/QUANT_BCD - d/QUANT_BCD;
        if (rvalue > 1) {
                rvalue = 1;
        }
        if (rvalue < 0) {
                rvalue = 0;
        }

        return rvalue;
}

static float compute_Y3(unsigned a, int b, int c, int d)
{
        float rvalue = a/QUANT_A + b/QUANT_BCD - c/QUANT_BCD - d/QUANT_BCD;
        if (rvalue > 1) {
                rvalue = 1;
        }
        if (rvalue < 0) {
                rvalue = 0;
        }

        return rvalue;
}

static float compute_Y4(unsigned a, int b, int c, int d)
{
        float rvalue = a/QUANT_A + b/QUANT_BCD + c/QUANT_BCD + d/QUANT_BCD;
        if (rvalue > 1) {
                rvalue = 1;
        }
        if (rvalue < 0) {
                rvalue = 0;
        }

        return rvalue;
}

/*Function to free a Pnm_components*/
void wordparts_free(Pnm_components wordparts)
{
        wordparts->methods->free(&(wordparts->pixels));
        free(wordparts);
}
