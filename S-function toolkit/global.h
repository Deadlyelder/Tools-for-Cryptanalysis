/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <queue>
#include "search.h"
#include "equivalent.h"
#include "count.h"

void init_matrix_xdp(int m[2][2][2][4][4]);
void init_matrix_adp(int m[2][2][2][8][8]);
void init_matrix_mul3(int m[2][2][16][16]);
void init_matrix_xdp3(int m[2][2][2][2][9][9]);

#endif /* GLOBAL_H */
