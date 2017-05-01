/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#ifndef COUNT_H
#define COUNT_H

// matrices to count the number of output differences
// note: output state zero means 'no output state', don't include in summation
template <int N>
void num_diff(const int c[2][2][2][N][N], int d[2][2][1<<N][1<<N])
{
  memset(d, 0, 4 * (1<<N) * (1<<N) * sizeof(int));

  for (int in1=0; in1!=2; in1++) { // first input
    for (int in2=0; in2!=2; in2++) { // second input
      for (int out=0; out!=2; out++) { // for all output values
        int valid_outputs[N] = {0}; // mask of valid outputs for all states
        for (int state_in=0; state_in!=N; state_in++) {
          for (int state_out=0; state_out!=N; state_out++) {
            if (c[in1][in2][out][state_in][state_out]) {
              valid_outputs[state_in] |= (1<<state_out);
            }
          }
        }
        for (int mask_in=0; mask_in!=(1<<N); mask_in++) { // input state mask
           int mask_out = 0;
           for (int state=0; state!=N; state++) {
             if ((mask_in >> state) & 1) {
               mask_out |= valid_outputs[state];
             }
           }
           d[in1][in2][mask_in][mask_out]++;
        }
      }
    }
  }

}

#endif /* COUNT_H */
