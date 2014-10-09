/* uarray2.c
 * by Gabrielle Cella and Matthew Cardarelli, 9/23/13
 * last updated by Gabrielle Cella and Ann Bellinger, 10/3/13
 * Homework 2
 *
 * Implementation for a two dimensional array abstract data type, UArray2_T.
 * This is a two dimensional array of unboxed elements, implemented using
 * Hanson's one dimensional UArray_T
 */

#include "uarray2.h"
#include <uarray.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define T UArray2_T

static int convert_index(T uarray2, unsigned c, unsigned r);

struct T{
        unsigned numrows;
        unsigned numcols;
        UArray_T elem;
};

/*
 * UArray2_new() returns a new UArray2_T of a specified number of rows (nrows),
 * columns (ncols), and element size (size)
 */
T UArray2_new(int nrows, int ncols, int size)
{
        T array2 = malloc(sizeof(*array2));
        assert(array2 != NULL);
        array2->elem = UArray_new(nrows*ncols, size);
        array2->numrows = nrows;
        array2->numcols = ncols;

        return array2;
}

/*
 * UArray2_free() frees the elements stored in UArray2_T, as well as the 
 * UArray2_T itself
 */
void UArray2_free(T *uarray2)
{
        UArray_free(&(*uarray2)->elem);
        free(*uarray2);
}

/* UArray2_numrows() returns the number of rows in the two dimensional array */
int UArray2_numrows(T uarray2)
{
        return uarray2->numrows;
}

/*UArray2_numcols() returns the number of columns in the two dimensional array*/
int UArray2_numcols(T uarray2)
{
        return uarray2->numcols;
}

/*UArray2_size() returns the size of each element stored in the 2D array*/
int UArray2_size(T uarray2)
{
        return UArray_size(uarray2->elem);
}

/*
 * UArray2_at() returns a void pointer to the element stored in the 2D array at
 * the specified row and column number (r, c)
 */
void *UArray2_at(T uarray2, unsigned c, unsigned r)
{
        if (r > uarray2->numrows || c > uarray2->numcols) {
                exit(1);
                /*cannot access an index outside the bounds of the array*/
        }
        return UArray_at(uarray2->elem, convert_index(uarray2, c, r));
}

/*
 * convert_index() returns an integer representing the index of the 1D 
 * array corresponding to a given row and column number (r, c) in the 2D array 
 * This function is private to the UArray2_T implementation and cannot be
 * accessed by clients
 */
static int convert_index(T uarray2, unsigned c, unsigned r)
{
        return (r * uarray2->numcols) + c;
}

/*
 * UArray2_map_row_major() calls user defined function apply() on every index 
 * of the 2D array, varying column indicies more rapidly than row indicies
 */
void UArray2_map_row_major(T uarray2, void apply(int c, int r, T uarray2,
                                                 void *elem, void *cl),
                           void *cl)
{
        for (unsigned j = 0; j < uarray2->numrows; j++) {
                for (unsigned i = 0; i < uarray2->numcols; i++) {
                        apply(i, j, uarray2, 
                              UArray_at(uarray2->elem, 
                                        convert_index(uarray2, i, j)), cl);
                }
        }
}

/*
 * UArray2_map_col_major() calls user defined function apply() on every index
 * of the 2D array, varying row indicies more rapidly than column indicies
 */
void UArray2_map_col_major(T uarray2, void apply(int c, int r, T uarray2, 
                                                 void *elem, void *cl), 
                           void *cl)
{
        for (unsigned i = 0; i < uarray2->numcols; i++) {
                for (unsigned j = 0; j < uarray2->numrows; j++) {
                        apply(i, j, uarray2, 
                              UArray_at(uarray2->elem, 
                                        convert_index(uarray2, i, j)), cl);
                }
        }
}
