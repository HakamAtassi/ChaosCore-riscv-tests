// See LICENSE for license details.

//**************************************************************************
// Median filter bencmark
//--------------------------------------------------------------------------
//
// This benchmark performs a 1D three element median filter. The
// input data (and reference data) should be generated using the
// median_gendata.pl perl script and dumped to a file named
// dataset1.h.

#include "util.h"

#include <stdlib.h>
#include <stdio.h>


//--------------------------------------------------------------------------
// Input/Reference Data

void thread_entry(int cid, int nc) {

    if (cid == 0) {
        while (1); 
    } else if (cid == 1) {
        while (1); 
    }

}


