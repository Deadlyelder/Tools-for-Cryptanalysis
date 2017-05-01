/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#ifndef SEARCH_H
#define SEARCH_H

template <int N, typename T = uint32_t>
struct output {
  typedef T word_t;

  output() {}

  output(const int n, const int j) : p(1.0), w(0), i(j) {
    for (int i = 0; i < N; ++i)
      s[i] = 0.0;

    s[n] = 1.0;
  }

  double s[N];
  double p;
  word_t w; 
  int i;
};

template <int N, typename T>
inline bool operator<(const output<N, T>& a, const output<N, T>& b) {
  if (a.p != b.p)
    return a.p < b.p;
  else
    return a.i < b.i;
}

template <int N, typename T>
inline output<N, T> operator*(const output<N, T>& x, const int m[][N]) {
  output<N, T> y;

  y.w = x.w;
  y.i = x.i + 1;

  for (int i = 0; i < N; ++i)
    {
      y.s[i] = 0.0;

      for (int j = 0; j < N; ++j)
        y.s[i] += x.s[j] * m[j][i];
    }

  return y;
}

template <int N, typename T>
double operator*(const output<N, T>& x, const double v[]) {
  double p = 0.0;

  for (int i = 0; i < N; ++i)
    p += x.s[i] * v[i];

  return p;
}

template <int N, typename T>
inline output<N, T> operator+(const output<N, T>& a, const output<N, T>& b) {
  output<N, T> c;

  c.w = 0;
  c.i = a.i;

  for (int i = 0; i < N; ++i)
    c.s[i] = a.s[i] + b.s[i];

  return c;
}

template <int N, typename T>
inline double sum(const output<N, T>& x) {
  double p = 0.0;

  for (int i = 0; i < N; ++i)
    p += x.s[i];

  return p;
}

// ----------------------------------------------------------------------------

template <int N, int W = 32, typename T = uint32_t>
class search {
public:
  typedef T word_t;
  typedef output<N, T> out_t;
  typedef std::priority_queue<out_t> pq_t;

  typedef int my_array[2][N][N];
  my_array* m[W];

  void start() {
    out_t x = out_t(0, 0);

    for (int i = 0; i < W; ++i)
      x = x * (*m[i])[0] + x * (*m[i])[1];

    const double r = sum(x);

    for (int n = 0; n < N; ++n) {
      v[0][W][n] = 1.0 / r;
      v[1][W][n] = 1.0 / r;
    }

    for (int j = W - 1; j > 0; --j) {
      for (int n = 0; n < N; ++n) {
        for (int out = 0; out < 2; ++out) {
          q = pq_t();

          out_t x = out_t(n, j) * (*m[j])[out];
          x.p = std::max(x * v[0][j + 1], x * v[1][j + 1]);
          if (out) x.w |= word_t(1) << j;

          if (x.p != 0.0) q.push(x);
          fill_queue();

          v[out][j][n] = q.empty() ? 0.0 : q.top().p;
        }
      }
    }

    q = pq_t();
    q.push(out_t(0, 0));
    fill_queue();
  }

  bool empty() const {
    return q.empty();
  }

  const out_t& top() const {
    return q.top();
  }

  void pop() {
    q.pop();
    fill_queue();
  }

private:
  void fill_queue() {
    while (!q.empty()) {
      const int i = q.top().i;

      if (i == W)
        break;

      out_t x = q.top() * (*m[i])[0];
      out_t y = q.top() * (*m[i])[1];

      q.pop();

      x.p = std::max(x * v[0][i + 1], x * v[1][i + 1]);
      y.p = std::max(y * v[0][i + 1], y * v[1][i + 1]);
      y.w |= word_t(1) << i;

      if (x.p != 0) q.push(x);
      if (y.p != 0) q.push(y);
    }
  }

  pq_t q;
  double v[2][W + 1][N];
};

#endif /* SEARCH_H */
