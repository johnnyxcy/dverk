#include <chrono>
#include <iostream>

#include <Eigen/Dense>
#include <functional>

#include "dverk/dverk.h"

using namespace std;

struct dverk_wrapped_eigen_data_t {
  std::function<void(double t, const Eigen::Map<Eigen::VectorXd> &y,
                     Eigen::Map<Eigen::VectorXd> &dydt)>
      f;

  const int neq;
};

int raw_dverk_wrapped_f(int *neq, double *t, double *y, double *ydot,
                        void *dat) {
  auto data_ = (dverk_wrapped_eigen_data_t *)(dat);

  Eigen::Map<Eigen::VectorXd> y_(y, data_->neq);

  Eigen::Map<Eigen::VectorXd> dydt_(ydot, data_->neq);

  data_->f(*t, y_, dydt_);

  return (0);
}

void test_case(double t, const Eigen::Map<Eigen::VectorXd> &y,
               Eigen::Map<Eigen::VectorXd> &ydot) {
  double cl, v, ka;
  cl = 0.134;
  v = 7.81;
  ka = 0.571;
  ydot[0] = -ka * y[0];
  ydot[1] = ka * y[0] - cl / v * y[1];
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

  struct dverk_wrapped_eigen_data_t dat {
    .f = test_case, .neq = neq
  };

  for (int i = 0; i < 12; i++) {
    dverk(&neq, raw_dverk_wrapped_f, &t, y, &tout, &tol, &ind, c, &nw, w, &dat);
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
