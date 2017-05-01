/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#ifndef EQUIVALENT_H
#define EQUIVALENT_H

namespace equivalent {
  template <int M, int C>
  inline bool equal(const int q[M][C], const int c[M][C][C], const int i) {
    for (int k = 0; k < M; ++k)
      for (int j = 0; j < C; ++j)
        if (q[k][j] != c[k][i][j])
          return false;

    return true;
  }

  template <int M, int C>
  inline int find(const int q[M][C], const int c[M][C][C], const int n) {
    for (int i = 0; i < n; ++i)
      if (equivalent::equal<M>(q, c, i))
        return i;

    return n;
  }
}

template <int M, int N, int C>
int combine_equiv(const int m[M][N][N], int c[M][C][C]) {
  int r[2][N] = {{0}};
  int* s = r[0];
  int* t = r[1];
  int n = 0;

  while (true) {
    const int p = n;
    n = 0;

    for (int i = 0; i < N; ++i) {
      int q[M][C] = {{0}};

      for (int k = 0; k < M; ++k)
        for (int j = 0; j < N; ++j)
          q[k][s[j]] += m[k][i][j];

      t[i] = equivalent::find<M>(q, c, n);

      if (t[i] == n) {
        assert(n < C);

        for (int k = 0; k < M; ++k)
          for (int j = 0; j < C; ++j) 
            c[k][n][j] = q[k][j];

        ++n;
      }
    }

    if (n == p)
      return n;

    std::swap(s, t);
  }
}

#endif /* EQUIVALENT_H */
