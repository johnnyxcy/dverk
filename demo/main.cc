#include <chrono>
#include <iostream>

#include "dverk/dverk.h"

using namespace std;

struct PkData {
  double cl;
  double v;
  double ka;
};

int pk(int *neq, double *t, double *y, double *ydot, void *dat) {
  auto pk_data = *static_cast<PkData *>(dat);

  ydot[0] = -pk_data.ka * y[0];
  ydot[1] = pk_data.ka * y[0] - pk_data.cl / pk_data.v * y[1];
  return (0);
}

int test_dverk() {
  int neq, itol, ind, nw;
  neq = 2;

  double tol, t, tout, *y, *c, *w;

  nw = 10;
  y = new double[neq];
  c = new double[40];
  w = new double[nw * 9];

  t = 0.0E0;
  tout = 0.1;
  tol = 1.0E-6;
  ind = 1;

  y[0] = 100.0;
  y[1] = 0.0;

  c[1] = 1;

  auto pk_data = PkData{.cl = 0.134, .v = 7.81, .ka = 0.571};

  for (int i = 0; i < 12; i++) {
    dverk(&neq, pk, &t, y, &tout, &tol, &ind, c, &nw, w, &pk_data);
    if (ind == 3) {
      printf(" at t= %12.4e y= %14.6e %14.6e\n", t, y[0], y[1]);
      ind = 1;
    } else {
      break;
    }
    tout = tout + i;
  }

  delete[] y;
  delete[] c;
  delete[] w;

  return (0);
}

int main(void) {
  chrono::steady_clock::time_point begin, end;
  begin = chrono::steady_clock::now();
  double dt;

  int N = 1;

  for (int i = 0; i < N; i++) {
    test_dverk();
  }
  end = chrono::steady_clock::now();
  dt = chrono::duration_cast<chrono::microseconds>(end - begin).count();
  cout << "Time taken (us/per loop) = " << dt / N << endl;

  return (0);
}
