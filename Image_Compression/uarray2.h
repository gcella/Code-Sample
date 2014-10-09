/* uarray2.h
 * by Gabrielle Cella and Matthew Cardarelli, 9/23/13
 * Homework 2
 *
 * UArray2_T is an abstract data type representing a two dimensional array
 * of unboxed elements.
 */

#ifndef UArray2_INCLUDED
#define UArray2_INCLUDED

#define T UArray2_T
typedef struct T *T;

extern T UArray2_new(int nrows, int ncols, int size);
extern void UArray2_free(T *uarray2);
extern int UArray2_numrows(T uarray2);
extern int UArray2_numcols(T uarray2);
extern int UArray2_size(T uarray2);
extern void *UArray2_at(T uarray2, unsigned c, unsigned r);
extern void UArray2_map_row_major(T uarray2, 
                                  void apply(int c, int r, T uarray2, 
                                             void *elem, void *cl), void *cl);
extern void UArray2_map_col_major(T uarray2,
                                  void apply(int c, int r, T uarray2, 
                                             void *elem, void *cl), void *cl);

#undef T
#endif
