/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#include "global.h"

void init_matrix_mul3(int m[2][2][16][16]) {
  memset(m, 0, 4 * 16 * 16 * sizeof(int));

  for (int delta_x = 0; delta_x < 2; ++delta_x) {

    for (int c1_in = 0; c1_in < 2; ++c1_in) {
    for (int c2_in = 0; c2_in < 2; ++c2_in) {
    for (int d1_in = 0; d1_in < 2; ++d1_in) {
    for (int d2_in = 0; d2_in < 2; ++d2_in) {

      for (int x1 = 0; x1 < 2; ++x1) {


        const int      x2 =  x1 ^ delta_x;
        const int      z1 =  x1 ^ d1_in ^ c1_in;
        const int  c1_out = (x1 + d1_in + c1_in) >> 1;
        const int  d1_out = x1;
        const int      z2 =  x2 ^ d2_in ^ c2_in;
        const int  c2_out = (x2 + d2_in + c2_in) >> 1;
        const int  d2_out = x2;
        const int delta_z =  z1 ^ z2;

        const int S_in  = 8 * d2_in  + 4 * d1_in  + 2 * c2_in  + c1_in;
        const int S_out = 8 * d2_out + 4 * d1_out + 2 * c2_out + c1_out;

        ++m[delta_x][delta_z][S_in][S_out];
      }
    }}}}
  }
}
