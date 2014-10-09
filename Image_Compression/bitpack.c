/* bitpack.c
 * by Gabrielle Cella and Ann Bellinger, 10/15/13
 * Homework 4
 *
 * Implementation for all of the bitpacking functions. These functions check
 * to make sure a bit can be stored in a given width and then perform the
 * necessary adjustmenets to the bit word. 
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "except.h"
#include "assert.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

static inline int64_t shift_lefts(int64_t value, int shift);
static inline uint64_t shift_leftu(uint64_t value, int shift);
static inline int64_t shift_rights(int64_t value, int shift);
static inline uint64_t shift_rightu(uint64_t value, int shift);

/*Checks to see if an unsigned value, n, will fit in a width of width */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= 64);

        return (n <= (shift_leftu(1, width) - 1 ));
}

/*Checks to see if an signed value, n, will fit in a width of width */
bool Bitpack_fitss(int64_t n, unsigned width)
{
        if (width == 0) {
                if (n == 0) {
                        return true;
                } else {
                        return false;
                }
        }

        assert(width <= 64);
        int lowlimit = shift_lefts(-1, width - 1);
        int highlimit = shift_leftu(1, width - 1) - 1;

        return (n <= highlimit && n >= lowlimit);
}

/*Returns an unsigned value from a word given the least significant bit and the
  width of the value */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        if (width == 0) {
                return 0;
        }

        uint64_t temp = shift_rightu(word, lsb);
        uint64_t mask = shift_leftu(1, width) - 1;
        mask = mask & temp;

        return mask;
}

/*Returns an signed value from a word given the least significant bit and the
  width of the value */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        int64_t value = Bitpack_getu(word, width, lsb);
        int64_t mask = shift_lefts(1, width - 1);
        if (!(mask & value)) {
                return value;
        }
        int64_t mask2 = shift_lefts(-1, width);

        return mask2 | value;
}

/*Returns a word that is given as a parameter with the added unsigned value
  at its given least significant bit with a width of width */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
        if (!Bitpack_fitsu(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        uint64_t mask = shift_lefts(1, width + lsb) - 1;
        uint64_t subtract = shift_lefts(1, lsb) - 1;
        mask -= subtract;

        uint64_t temp = mask | word;

        int64_t temp2 = -1;

        value = shift_leftu(value, lsb);

        temp2 = temp2 & value;
        temp2 += subtract;

        int64_t mask2 = -1;
        mask2 = shift_lefts(mask2, width + lsb);

        temp2 = mask2 | temp2;

        return temp & temp2;
}

/*Returns a word that is given as a parameter with the added signed value
  at its given least significant bit with a width of width */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        uint64_t mask = shift_leftu(1, width) - 1;
        uint64_t testvalue = mask & value;

        return Bitpack_newu(word, width, lsb, testvalue);
}

/*Shifts a signed value left by shift.*/
static inline int64_t shift_lefts(int64_t value, int shift)
{
        if (shift == 64) {
                return 0;
        }

        return value << shift;
}

/*Shifts an unsigned value left by shift*/
static inline uint64_t shift_leftu(uint64_t value, int shift)
{
        if (shift == 64) {
                return 0;
        }

        return value << shift;
}

/*Shifts a signed value right by shift*/
static inline int64_t shift_rights(int64_t value, int shift)
{
        if (shift == 64) {
                if (value < 0) {
                        return -1;
                } else {
                        return 0;
                }
        }

        return value >> shift;
}

/*Shifts an unsigned value right by shift*/
static inline uint64_t shift_rightu(uint64_t value, int shift)
{
        if (shift == 64) {
                return 0;
        }

        return value >> shift;
}

