#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

class Markov {
  private:
    std::vector<float> initialValues();
    std::vector<std::vector<float> > steadyState();
  public:
    Markov();
    std::vector<std::vector<float> > ssParameters;
    std::vector<float> dYdt(float, float, float, float, float, std::vector<float>);
    std::vector<std::vector<float> > soln(float, float, float, bool, std::vector<std::vector<float>>);
    std::vector<float> Os(float, float, float, bool, std::vector<std::vector<float>>);
    std::vector<float> getVals ();
};

Markov::Markov() {
  ssParameters = steadyState();
}

std::vector<float> Markov::initialValues() {
  std::vector<float> vals(10);
  const gsl_rng_type * T;
  gsl_rng * r;
  
  int i, n = 10;
  gsl_rng_env_setup();

  T = gsl_rng_default;
  r = gsl_rng_alloc (T);

  for (i = 0; i < n; i++) {
    double u = gsl_rng_uniform (r);
    vals[i] = float(u);  
  }

  return vals;
}

std::vector<std::vector<float> > Markov::steadyState() {
  int i;
  float a32, b32, a21, b21, a1o, b1o, a1f, b1f, aof, bof;
  std::vector<float> ip = initialValues();
  std::vector<float> xp(5);
  a32 = ip[0];
  b32 = ip[1];
  a21 = ip[2];
  b21 = ip[3];
  a1o = ip[4];
  b1o = ip[5];
  a1f = ip[6];
  b1f = ip[7];
  aof = ip[8];
  bof = ip[9];
  
  double m[] = { 
    -a32, b32, 0.0, 0.0, 0.0, 
     a32, -(b32-a32), b21, 0.0, 0.0,
     0.0, a21, -(b21+a1o+a1f), b1o, b1f, 
     0.0, 0.0, a1o, -(b1o-aof), bof,
     0.0, 0.0, a1f, aof, -(b1f+bof)
  };

  double b[] = { 0.0, 0.0, 0.0, 0.0, 1.0 };

  int S;
  gsl_matrix_view M = gsl_matrix_view_array (m, 5, 5);
  gsl_vector_view B = gsl_vector_view_array (b, 5);

  gsl_vector *X = gsl_vector_alloc (5);
  gsl_permutation *P = gsl_permutation_alloc (5);
  gsl_linalg_LU_decomp (&M.matrix, P, &S);
  gsl_linalg_LU_solve (&M.matrix, P, &B.vector, X);

  for (i = 0; i < 5; i++)
    xp[i] = gsl_vector_get(X, i);
    
  gsl_permutation_free (P);
  gsl_vector_free (X);

  return {ip, xp};
}
// a32 = 0, b32 = 1
// a21 = 2, b21 = 3
// a1o = 4, b1o = 5
// a1f = 6, b1f = 7
// aof = 8, bof = 9
std::vector<float> Markov::dYdt(float c3_t, float c2_t, float c1_t, float o_t, float if_t, std::vector<float> p) {
  std::vector<float> dY (5);
  dY[0] = -p[0] * c3_t + p[1] * c2_t;
  dY[1] =  p[0] * c3_t - (p[1] + p[2]) * c2_t + p[3] * c1_t;
  dY[2] =  p[2] * c2_t - (p[3] + p[4] + p[6]) * c1_t + p[5] * o_t + p[7] * if_t;
  dY[3] =  p[4] * c1_t - (p[5] + p[8]) * o_t + p[9] * if_t;
  dY[4] =  p[6] * c1_t - (p[7] + p[9]) * if_t + p[8] * o_t;

  return dY;
}

std::vector<std::vector<float> > Markov::soln(float dt, float tFinal, float V_0, bool sa, std::vector<std::vector<float> > markov_params) {
  //Uses past ODE solver to solve for dynamic state
  int mp = 0, xp = 1;
  std::vector<std::vector<float> > p = markov_params;

  int cols = 6;
  int rows = tFinal/dt;
  
  std::vector<std::vector<float> > y(cols, std::vector<float>(rows));
  std::vector<float> t(rows);
  
  y[0][0] = p[xp][0];
  y[1][0] = p[xp][1];
  y[2][0] = p[xp][2];
  y[3][0] = p[xp][3];
  y[4][0] = p[xp][4];
  
  for (int i = 0; i < rows; i ++) {
    
    std::vector<float> K1 = dYdt(y[0][i], y[1][i], y[2][i], y[3][i], y[4][i], p[mp]);    
    std::vector<float> K2 = dYdt(y[0][i] + dt * .5 * K1[0], y[1][i] + dt * .5 * K1[1], y[2][i] + dt * .5 * K1[2], y[3][i] + dt * .5 * K1[3], y[4][i] + dt * .5 * K1[4], p[mp]);
    std::vector<float> K3 = dYdt(y[0][i] + dt * .5 * K2[0], y[1][i] + dt * .5 * K2[1], y[2][i] + dt * .5 * K2[2], y[3][i] + dt * .5 * K2[3], y[4][i] + dt * .5 * K2[4], p[mp]);
    std::vector<float> K4 = dYdt(y[0][i] + dt * .5 * K3[0], y[1][i] + dt * .5 * K3[1], y[2][i] + dt * .5 * K3[2], y[3][i] + dt * .5 * K3[3], y[4][i] + dt * .5 * K3[4], p[mp]);
    
    y[0][i+1] = y[0][i] + (dt * (K1[0] + 2.0 * K2[0] + 2.0 * K3[0] + K4[0])) / 6.0;
    y[1][i+1] = y[1][i] + (dt * (K1[1] + 2.0 * K2[1] + 2.0 * K3[1] + K4[1])) / 6.0;
    y[2][i+1] = y[2][i] + (dt * (K1[2] + 2.0 * K2[2] + 2.0 * K3[2] + K4[2])) / 6.0;
    y[3][i+1] = y[3][i] + (dt * (K1[3] + 2.0 * K2[3 ] + 2.0 * K3[3] + K4[3])) / 6.0;
    y[4][i+1] = y[4][i] + (dt * (K1[4] + 2.0 * K2[4] + 2.0 * K3[4] + K4[4])) / 6.0;
    
    t[i+1] = dt * (i + 1);
  }
  
  return {t, y[0], y[1], y[2], y[3], y[4]};
}

std::vector<float> Markov::Os(float dt, float tFinal, float V_0, bool sa, std::vector<std::vector<float> > markov_params) {
  std::vector<float> O_soln = soln(dt,tFinal,V_0,sa,markov_params)[4];
  return O_soln;
}

