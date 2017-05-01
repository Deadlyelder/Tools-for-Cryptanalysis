/*
 * Toolkit for the Differential Cryptanalysis
 * of ARX-based Cryptographic Constructions.
 *
 * (c) 2010 Nicky Mouha, Vesselin Velichkov,
 *          Christophe De Canni`{e}re and Bart Preneel
 */
#include "global.h"
#include "print.h"
#include "probability.h"
#include "search.h"

/* Enable to show the matrices */
// #define PRINT_MATRICES

void test_xdp() {
  int m[2][2][2][4][4];
  int c[2][2][2][2][2];

  const uint32_t x = 0x12492489;
  const uint32_t y = x << 1;
  const uint32_t z = 0x3AEBAEAB;

  std::cout << "*** xdp" << std::endl;
  init_matrix_xdp(m);
#ifdef PRINT_MATRICES
  std::cout << "*** print xdp matrices" << std::endl;
  print_matrix<8>(&m[0][0][0]);
#endif
  combine_equiv<8>(&m[0][0][0], &c[0][0][0]);
#ifdef PRINT_MATRICES
  std::cout << "*** print minimized xdp matrices" << std::endl;
  print_matrix<8>(&c[0][0][0]);
#endif
  const double col_sum = 4.0;
  const double p = compute_probability<2,2,uint32_t>(c,col_sum,x,y,z);
  printf("xdp(0x%08x,0x%08x->0x%08x)=2^%f\n",x,y,z,log(p)/log(2.0));
}

void test_adp() {
  int m[2][2][2][8][8];

  const uint32_t x = 0x12492489;
  const uint32_t y = x << 1;
  const uint32_t z = 0x3AEBAEAB;

  std::cout << "*** adp" << std::endl;
  init_matrix_adp(m);
#ifdef PRINT_MATRICES
  std::cout << "*** print adp matrices" << std::endl;
  print_matrix<8>(&m[0][0][0]);
#endif
  const double col_sum = 4.0;
  const double p = compute_probability<2,8,uint32_t>(m,col_sum,x,y,z);
  printf("adp(0x%08x,0x%08x->0x%08x)=2^%f\n",x,y,z,log(p)/log(2.0));
}

void test_xdp3() {
  int m[2][2][2][2][9][9];
  int c[2][2][2][2][4][4];

  // random test values
  const uint32_t x = 0xF44ED7C4;
  const uint32_t y = 0x0DEEDE14;
  const uint32_t z = 0x0186EECD;
  const uint32_t t = 0xF846E72D;

  std::cout << "*** xdp3" << std::endl;
  init_matrix_xdp3(m);
#ifdef PRINT_MATRICES
  std::cout << "*** print xdp3 matrices" << std::endl;
  print_matrix<16>(&m[0][0][0][0]);
#endif
  combine_equiv<16>(&m[0][0][0][0], &c[0][0][0][0]);
#ifdef PRINT_MATRICES
  std::cout << "*** print minimized xdp3 matrices" << std::endl;
  print_matrix<16>(&c[0][0][0][0]);
#endif
  double col_sum = 8.0;
  const double p = compute_probability<2,4,uint32_t>(c,col_sum,x,y,z,t);
  printf("xdp3(0x%08x,0x%08x,0x%08x->0x%08x)=2^%f\n",x,y,z,t,log(p)/log(2.0));
}

void test_mul3() {
  int m[2][2][16][16];
  int c[2][2][4][4];

  const uint32_t x = 0x12492489;
  const uint32_t y = 0x3AEBAEAB;

  std::cout << "*** mul3" << std::endl;
  std::cout << "*** Compare this probability to the xdp calculation" << std::endl;
  init_matrix_mul3(m);
#ifdef PRINT_MATRICES
  std::cout << "*** print mul3 matrices" << std::endl;
  print_matrix<4>(&m[0][0]);
#endif
  combine_equiv<4>(&m[0][0], &c[0][0]);
#ifdef PRINT_MATRICES
  std::cout << "*** print minimized mul3 matrices" << std::endl;
  print_matrix<4>(&c[0][0]);
#endif
  double col_sum = 2.0;
  const double p = compute_probability<2,4,uint32_t>(c,col_sum,x,y);
  printf("xdp-mul3(0x%08x->0x%08x)=2^%f\n",x,y,log(p)/log(2.0));
}

void test_xdc() {
  std::cout << "*** xdc" << std::endl;
  int m[2][2][2][4][4];
  int c[2][2][2][2][2];
  int d[2][2][4][4];

  init_matrix_xdp(m);
  combine_equiv<8>(&m[0][0][0], &c[0][0][0]);
  num_diff(c, d);
#ifdef PRINT_MATRICES
  std::cout << "*** print xdc matrices" << std::endl;
  print_matrix<4>(&d[0][0]);
#endif

  std::cout << "*** Counting the number of output differences, in the example" << std::endl;
  std::cout << "    in Theorem 1 of \"Improved Cryptanalysis of Skein\"," << std::endl;
  std::cout << "    Aumasson et al., ASIACRYPT 2009" << std::endl;
  const uint64_t delta = 0x1000010402000000uLL;
  int num_diffs = 0;
  {
    int v[2][4] = {{0}};
    int* x = v[0];
    int* y = v[1];

    x[1] = 1;
    for (int i=0; i!=64; i++) {
      for (int j=0; j!=4; j++) {
        y[j] = 0;
        for (int k=0; k!=4; k++) {
          y[j] += x[k] * d[(delta>>i)&1][0][k][j];
        }
      }
      std::swap(x,y);
    }

    for (int k=1; k!=4; k++) {
      num_diffs += x[k];
    }
  }
  std::cout << "xdc(0x" << std::setw(16) << std::setfill('0')
	  << std::hex << delta << ",0x0000000000000000)=" << std::dec
	  << num_diffs << std::endl;
}

void test_maxdiff() {
  const int N = 8;
  const int W = 32;

  typedef uint32_t word_t;

  int m[2][2][2][8][8];
  init_matrix_adp(m);

  search<N, W, word_t> s;

  const uint32_t a = 0x54000000;
  const uint32_t b = 0xA4000000;

  std::cout << "*** adp: list of output differences, sorted by probability, highest first" << std::endl;
  printf("adp(0x%08x,0x%08x->c)\n",a,b);

  for (int i = 0; i < W; ++i)
    s.m[i] = &m[(a >> i) & 1][(b >> i) & 1];

  s.start();

  while (!s.empty()) {
    printf("%08x 2^%.2f\n", s.top().w, log(s.top().p) / log(2.0));
    s.pop();
  }
}

int main() {
  test_xdp();
  test_xdc();
  test_adp();
  test_xdp3();
  test_mul3();
  test_maxdiff();

  return 0;
}
