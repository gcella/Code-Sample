/* compvid.c
 * by Gabrielle Cella and Ann Bellinger, 10/15/13
 * Homework 4
 * 
 * Implementation of the compression and decompression functions that convert
 * to and from RGB values and Y, PB, and PR values. 
 */

#include <math.h>
#include "processes.h"
#include "arith40.h"

typedef A2Methods_UArray2 A2;

/*Compression constants*/
const float RY = 0.299;
const float GY = 0.587;
const float BY = 0.114;
const float RPB = -0.168736;
const float GPB = -0.331264;
const float BPB = 0.5;
const float RPR = 0.5;
const float GPR = -0.418688;
const float BPR = -0.081312;

/*Decompression constants*/
const float RYd = 1.0;
const float GYd = 1.0;
const float BYd = 1.0;
const float RPBd = 0;
const float GPBd = -0.344136;
const float BPBd = 1.772;
const float RPRd = 1.402;
const float GPRd = -0.714136;
const float BPRd = 0;

static void fill_cv_array(int i, int j, A2 pixels, void *ptr, void *cl);
static float convert_rgb(unsigned value, unsigned denominator);
static float convert_cv(Pnm_rgb pixel, unsigned denominator, float v1, 
                        float v2, float v3);
static void fill_pnm_decomp(int i, int j, A2 pixels, void *ptr, void *cl);
static unsigned quantize_rgb(float value, unsigned denom);
static float decomp_rgb(cv_info cv, float v1, float v2, float v3);

/*Creates a new Pnm_cv, using the information in a Pnm_ppm. It contains an array
  of cv_info structs, which hold Y, PB, and PR values. This function returns 
  the Pnm_cv to compress40.c*/
Pnm_cv create_cv_array(Pnm_ppm input_array)
{
        Pnm_cv compvid = malloc(sizeof(*compvid));
        assert(compvid != NULL);

        if(input_array->width % 2 != 0) {
                compvid->width = input_array->width - 1;
        } else {
                compvid->width = input_array->width;
        }
        if(input_array->height % 2 != 0) {
                compvid->height = input_array->height - 1;
        } else {
                compvid->height = input_array->height;
        }

        compvid->denominator = input_array->denominator;
        compvid->methods = uarray2_methods_blocked;
        assert(compvid->methods != NULL);

        compvid->pixels = 
                compvid->methods->new_with_blocksize(compvid->width,
                                                     compvid->height, 
                                                     sizeof(struct cv_info), 
                                                     BLOCKSIZE);

        A2Methods_mapfun *map = input_array->methods->map_row_major;
        assert(map != NULL);
        map(input_array->pixels, fill_cv_array, &compvid);

        return compvid;
}

/*This is an apply function used with the array of Pnm_rgb in Pnm_ppm. This 
  function calls functions to convert the R, G, and B values to Y, Pb, and Pr  
  values and stores these values in a cv_info struct. That struct is then stored
  in the Pnm_cv that is passed as the cl.*/
static void fill_cv_array(int i, int j, A2 pixels, void *ptr, void *cl)
{
        (void) pixels;

        Pnm_cv compvid = *((Pnm_cv*)cl);

        if ((unsigned)i + 1 > compvid->width || 
            (unsigned)j + 1 > compvid->height) {
                return;
        }

        Pnm_rgb rgb = ((Pnm_rgb)ptr);

        cv_info temp = compvid->methods->at(compvid->pixels, i, j);

        temp->Y = convert_cv(rgb, compvid->denominator, RY, GY, BY);
        temp->Pb = convert_cv(rgb, compvid->denominator, RPB, GPB, BPB);
        temp->Pr = convert_cv(rgb, compvid->denominator, RPR, GPR, BPR); 
}

/*This function returns Y, Pb, or Pr depending on the values passed in for v1, 
  v2, or v3, which are constants declared above. */
static float convert_cv(Pnm_rgb pixel, unsigned denominator, float v1, 
                        float v2, float v3)
{
        return ((v1 * convert_rgb(pixel->red, denominator)) + 
                (v2 * convert_rgb(pixel->green, denominator)) + 
                (v3 * convert_rgb(pixel->blue, denominator)));
}

/*This function converts an R, G, or B value to a float by dividing by the 
  denominator*/
static float convert_rgb(unsigned value, unsigned denominator)
{
        return (((float)value) / ((float)denominator));
}

/*Creates a new Pnm_ppm that will hold the Pnm_rgb struct for a decompressed
  image. This function returns the Pnm_ppm to decompress40.c*/
Pnm_ppm decomp_cv_array(Pnm_cv compvid)
{
        Pnm_ppm output = malloc(sizeof(*output));
        assert(output != NULL);
        output->width = compvid->width;
        output->height = compvid->height;
        output->denominator = compvid->denominator;
        output->methods = uarray2_methods_plain;
        assert(output->methods != NULL);

        output->pixels = output->methods->new(output->width, output->height, 
                                              sizeof(struct Pnm_rgb));

        A2Methods_mapfun *map = compvid->methods->map_block_major;
        assert(map != NULL);
        map(compvid->pixels, fill_pnm_decomp, &output);

        return output;
}

/*This is the apply function for the cv_info array. This function calls 
  functions to convert Y, PR, and PB values to R, G, and B values and store
  these values in a Pnm_rgb struct. That struct is then stored in the UArray2
  that is in the closure.*/
static void fill_pnm_decomp(int i, int j, A2 pixels, void *ptr, void *cl)
{
        (void) pixels;
        Pnm_ppm output = *((Pnm_ppm*)cl);

        cv_info cv = ((cv_info)ptr);


        Pnm_rgb temp = output->methods->at(output->pixels, i, j);


        temp->red = quantize_rgb(decomp_rgb(cv, RYd, RPBd, RPRd), 
                                 output->denominator);

        temp->blue = quantize_rgb(decomp_rgb(cv, BYd, BPBd, BPRd), 
                                  output->denominator);
       
        temp->green = quantize_rgb(decomp_rgb(cv, GYd, GPBd, GPRd), 
                                   output->denominator);

}

/*This function returns an R, G, or B depending on the values passed in for v1, 
  v2, or v3, which are constants declared above.*/
static float decomp_rgb(cv_info cv, float v1, float v2, float v3)
{
        return ((v1 * cv->Y) + (v2 * cv->Pb) + (v3 * cv->Pr)); 
}

/*This function takes in an R, G, or B float and returns the cooresponding 
  quantized value.*/
static unsigned quantize_rgb(float value, unsigned denom)
{
        int rvalue = floor(value * denom);

        if (rvalue < 0) {
                rvalue = 0;
        } else if ((unsigned)rvalue > denom) {
                rvalue = denom;
        }
        return (unsigned)rvalue;
}

/*Function to free a Pnm_cv*/
void compvid_free(Pnm_cv compvid)
{
        compvid->methods->free(&(compvid->pixels)); 
        free(compvid);
}
