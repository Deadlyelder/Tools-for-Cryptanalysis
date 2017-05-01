/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#include "global.h"

void init_matrix_xdp(int m[2][2][2][4][4]) {
  memset(m, 0, 8 * 4 * 4 * sizeof(int));

  for (int delta_x = 0; delta_x < 2; ++delta_x) {
  for (int delta_y = 0; delta_y < 2; ++delta_y) {

    for (int c1_in = 0; c1_in < 2; ++c1_in) {
    for (int c2_in = 0; c2_in < 2; ++c2_in) {

      for (int x1 = 0; x1 < 2; ++x1) {
      for (int y1 = 0; y1 < 2; ++y1) {

        const int      x2 =  x1 ^ delta_x;
        const int      y2 =  y1 ^ delta_y;
        const int      z1 =  x1 ^ y1 ^ c1_in;
        const int  c1_out = (x1 + y1 + c1_in) >> 1;
        const int      z2 =  x2 ^ y2 ^ c2_in;
        const int  c2_out = (x2 + y2 + c2_in) >> 1;
        const int delta_z =  z1 ^ z2;

        const int S_in  = 2 * c2_in  + c1_in;
        const int S_out = 2 * c2_out + c1_out;

        ++m[delta_x][delta_y][delta_z][S_in][S_out];
      }}
    }}
  }}
}
