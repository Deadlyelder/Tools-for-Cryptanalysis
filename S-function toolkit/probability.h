/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#ifndef PROBABILITY_H
#define PROBABILITY_H

/* adp, xdp */
template <int M, int N, typename T>
double compute_probability(const int m[M][M][M][N][N], const double col_sum,
                           const T x, const T y, const T z) {
  double p=0.0; // probability
  double a[2][N] = {{0}};
  double* v = a[0];
  double* r = a[1];

  v[0] = 1.0;

  for(int i=0; i< (int)sizeof(T)*8; i++) {
    const T alpha = (x >> i) & 1;
    const T beta = (y >> i) & 1;
    const T gamma = (z >> i) & 1;

    for (int j=0; j!=N; j++) {
      r[j] = 0.0;
      for (int k=0; k!=N; k++) {
        r[j] += v[k] * m[alpha][beta][gamma][k][j] / col_sum;
      }
    }
    std::swap(v,r);
  }
  // final probability
  for(int i=0; i< N; i++) {
    p += v[i];
  }
  return p;
}

/* xdp3 */
template <int M, int N, typename T>
double compute_probability(const int m[M][M][M][M][N][N], const double col_sum,
                           const T x, const T y,
                           const T z, const T t) {
  double p = 0.0;  // probability
  double a[2][N] = {{0}};
  double* v = a[0];
  double* r = a[1];

  v[0] = 1.0;

  for(int i=0; i< (int)sizeof(T)*8; i++) {
    const T alpha = (x >> i) & 1;
    const T beta = (y >> i) & 1;
    const T gamma = (z >> i) & 1;
    const T theta = (t >> i) & 1;

   for (int j=0; j!=N; j++) {
     r[j] = 0.0;
     for (int k=0; k!=N; k++) {
        r[j] += v[k] * m[alpha][beta][gamma][theta][k][j] / col_sum;
     }
    }
    std::swap(v,r);
   }
   // final probability
   for(int i=0; i< N; i++) {
    p += v[i];
   }
   return p;
}

/* mul3 */
template <int M, int N, typename T>
double compute_probability(const int m[M][M][N][N], const double col_sum, const T x, const T y) {
  double p=0.0; // probability
  double a[2][N] = {{0}};
  double* v = a[0];
  double* r = a[1];

  v[0] = 1.0;

  for(int i=0; i< (int)sizeof(T)*8; i++) {
    const T alpha = (x >> i) & 1;
    const T beta = (y >> i) & 1;

    for (int j=0; j!=N; j++) {
      r[j] = 0.0;
      for (int k=0; k!=N; k++) {
        r[j] += v[k] * m[alpha][beta][k][j] / col_sum;
      }
    }
    std::swap(v,r);
  }

  // final probability
  for(int i=0; i< N; i++) {
    p += v[i];
  }
  return p;
}
#endif /* PROBABILITY_H */
