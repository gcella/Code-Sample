/* uarray2b.c
 * by Gabrielle Cella and Ann Bellinger, 10/3/13
 * Homework 3
 *
 * Implementation for a two dimensional blocked array, UArray2b_T.
 * This is a two dimensional array of unboxed elements. It is implemented
 * using Hanson's one dimensional UArray_T
 */

#include "uarray2b.h"
#include <uarray.h>
#include <stdlib.h>
#include <assert.h>

#define T UArray2b_T

static int row(T uarray2b, int i);
static int col(T uarray2b, int i);
static int convert_1d(T array2b, int i, int j);

const int K64 = 64000;

struct T{
        int width;
        int height;
        int size;
        int blocksize;
        UArray_T elem;
};

/*
 * UArray2b_new() returns an new UArray2b_T of a specified number of 
 * rows (height), columns (width), element size (size), 
 * and block size (blocksize)
 */
T UArray2b_new(int width, int height, int size, int blocksize)
{
        T array2b = malloc(sizeof(*array2b));
        assert(array2b != NULL);
        array2b->elem = UArray_new(width*height, size);
        array2b->width = width;
        array2b->height = height;
        array2b->size = size;
        array2b->blocksize = blocksize;
        assert(blocksize != 0);

        return array2b;
}

/*
 * UArray2b_new() returns an new UArray2b_T of a specified number of 
 * rows (height), columns (width), element size (size), 
 * and block size that is the maximum blocksize for 64K caches
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
        return UArray2b_new(width, height, size, K64/size);
}

void UArray2b_free(T *array2b)
{
        UArray_free(&(*array2b)->elem);
        free(*array2b);
}

int UArray2b_width(T array2b)
{
        return array2b->width;
}

int UArray2b_height(T array2b)
{
        return array2b->height;
}

int UArray2b_size(T array2b)
{
        return array2b->size;
}

int UArray2b_blocksize(T array2b)
{
        return array2b->blocksize;
}

/*
 * UArray2b_at() returns a void pointer to the element stored in the 2D array at
 * the specified row and column number (i, j)
 */
void *UArray2b_at(T array2b, int i, int j)
{
        if(j > array2b->height || i > array2b->width) {
                exit(1);
                /*cannot access an index outside the bounds of the array*/
        }
        return UArray_at(array2b->elem, convert_1d(array2b, i, j));
}

/*
 * UArray2b_map() calls user defined function apply() on every index 
 * of the 2D array, going through the blocks and the elements within each block 
 * in row-major order
 */
void UArray2b_map(T array2b, void apply(int i, int j, T array2b, 
                                        void *elem, void *cl), void *cl)
{
        int pos;
        for (pos = 0; pos < (array2b->width * array2b->height); pos++) {
                apply(col(array2b, pos), row(array2b, pos), array2b, 
                      UArray2b_at(array2b, col(array2b, pos), 
                                  row(array2b, pos)), cl);
        }

}

/*
 * row() converts i, the index in our UArray_T to the index of the row in 
 * the corresponding 2D array
 */
static int row(T array2b, int i)
{
        int blockrow = i / (array2b->blocksize * array2b->width);
        int largechunk = blockrow * array2b->blocksize * array2b->width;
        i = i - largechunk;

        int currblockh = array2b->height - (blockrow * array2b->blocksize);
        if ((currblockh / array2b->blocksize) > 0) {
                currblockh = array2b->blocksize;
        }

        int blocksb4 = i / (array2b->blocksize * currblockh);

        i = i - (blocksb4 * array2b->blocksize * currblockh);

        int currblockw = array2b->width - (blocksb4 * array2b->blocksize);
        if ((currblockw / array2b->blocksize) > 0) {
                currblockw = array2b->blocksize;
        }

        i = i / currblockw;
        i += (blockrow * array2b->blocksize);
 
        return i;
}

/*
 * col() converts i, the index in our UArray_T to the index of the column in 
 * the corresponding 2D array
 */
static int col(T array2b, int i)
{
        int blockrow = i / (array2b->blocksize * array2b->width);
        int largechunk = blockrow * array2b->blocksize * array2b->width;
        i = i - largechunk;

        int currblockh = array2b->height - (blockrow * array2b->blocksize);
        if ((currblockh / array2b->blocksize) > 0) {
                currblockh = array2b->blocksize;
        }

        int blocksb4 = i / (array2b->blocksize * currblockh);

        i = i - (blocksb4 * array2b->blocksize * currblockh);

        int currblockw = array2b->width - (blocksb4 * array2b->blocksize);
        if ((currblockw / array2b->blocksize) > 0) {
                currblockw = array2b->blocksize;
        }

        i = i % currblockw;
        i += blocksb4 * array2b->blocksize;

        return i;
}

/*
 * convert_1d() converts the coordinates i and j, to the corresponding index in 
 * the UArray_T
 */
static int convert_1d(T array2b, int i, int j)
{
        int blockrow = j / array2b->blocksize;
        int blockcol = i / array2b->blocksize;
        int rowinbox = j % array2b->blocksize;
        int colinbox = i % array2b->blocksize;

        int currblockh = array2b->height - (array2b->blocksize * blockrow);
        if ((currblockh / array2b->blocksize) > 0) {
                currblockh = array2b->blocksize;
        }

        int currblockw = array2b->width - (array2b->blocksize * blockcol);
        if ((currblockw / array2b->blocksize) > 0) {
                currblockw = array2b->blocksize;
        }

        int rowsabove = blockrow * array2b->blocksize * array2b->width;
        int colside = blockcol * array2b->blocksize * currblockh;

        int pos = (rowinbox * currblockw) + colinbox;

        return pos + colside + rowsabove;
}
