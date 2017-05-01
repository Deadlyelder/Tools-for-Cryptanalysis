/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#ifndef PRINT_H
#define PRINT_H

template <int M, int N>
void print_matrix(const int a[M][N][N]) {
  for (int m = 0; m < M; m++) {
    std::cout << "A_" << m << "=[" << std::endl;
    for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) {
        std::cout << a[m][j][i];
        if (i != N-1 || j != N-1) std::cout << ",";
      }
      std::cout << std::endl;
    }
    std::cout << "];" << std::endl << std::endl;
  }
}

#endif /* PRINT_H */
