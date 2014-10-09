#define BLOCKSIZE 2

#define BITWIDTH 8
#define INIT_LSB 24

#ifndef PROCESSES_INCLUDED
#define PROCESSES_INCLUDED

#include <stdint.h>
#include <stdlib.h>
#include "pnm.h"
#include "a2plain.h"
#include "uarray2.h"
#include "a2blocked.h"
#include "uarray2b.h"
#include "a2methods.h"
#include "assert.h"

typedef Pnm_ppm Pnm_cv;
typedef Pnm_ppm Pnm_components;

typedef struct cv_info {
        float Y;
        float Pb;
        float Pr;
} *cv_info;

typedef struct Components {
        unsigned PBchroma;
        unsigned PRchroma;
        unsigned a;
        int b, c, d;
} *Components;

extern Pnm_cv create_cv_array(Pnm_ppm input_aray);

extern Pnm_cv decomp_cv_array(Pnm_components something);

extern Pnm_components create_components_array(Pnm_cv compvid);

extern void compvid_free(Pnm_cv compvid);

extern Pnm_cv decomp_components_array(Pnm_components wordparts);

extern Pnm_ppm decomp_cv_array(Pnm_cv compvid);

extern void wordparts_free(Pnm_components wordparts);

extern UArray2_T create_packedwords_array(Pnm_components wordparts);

extern Pnm_components decomp_word_array(UArray2_T packedword);

#endif
