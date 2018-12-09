#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <algorithm>
#include <gsl/gsl_linalg.h>
#include "Markov.h"
#include "HH.h"

// Breakdown:
// Use LU decomposition to find steady states – 3 pts __done
// Use an ODE solver to solve for the dynamics of the Markov model – 3 pts __done
// Implement normalization and least squares to evaluate the model – 3 pts __done
// Use simulated annealing to find parameters that improve the model fit – 3 pts __done
// Achieve a good fit and insert into HH model – 3 pts

//Normalization function
std::vector<float> norm(std::vector<float> v) {
  std::vector<float> normd(v.size());
  float max = *std::max_element(v.begin(), v.end());
  float min = *std::min_element(v.begin(), v.end());
  if ( std::abs(min) > std::abs(max))
    max = min;
  for (int i = 0; i < v.size(); i++)
    normd[i] = (v[i]/ std::abs(max));
  return normd;
}

//Least Squares Error function
float leastSquare (std::vector<float> v1, std::vector<float> v2) {
    int size = v1.size();
    float error = 0.0, val = 0.0;

    for (int i = 0; i < size; i++)
      error += std::pow(std::abs(v2[i] - v1[i]), 2);

    return error;
}

// Output Functions

// Print to file function
void print(std::vector<std::vector<float> > v, std::string fname) {
  std::ofstream output;
  output.open(fname);
  for (int i = 0; i < v[0].size(); i++)
    output << v[0][i] << "," << v[1][i] << "\n";
  output.close();
}

// Print parameters function
void print_params(std::vector<float> ps) {
  std::cout << "Params:\n";
  for (int i = 0; i < 10; ++i) {
    std::cout << "\t" << i << ": " << ps[i];
    if (i == ps.size() -1) { std::cout << "\n"; }
  }
}

int main() {
  Markov markov;
  Sodium na = Sodium();

  float dt = 0.01;
  float tmax = 10.0;
  float V = -20.0;
  float error = 0.0;
  float lb = 0.0091, ub = 0.8757;

  std::vector<std::vector<float>> alphaBeta = markov.ssParameters;

  std::vector<std::vector<float> > mModel = markov.soln(dt, tmax, V, 0, alphaBeta);
  std::vector<float> target = na.ConductanceSoln(dt, tmax, V); //na.solution(dt, tmax, V);
  target = norm(target);

  std::vector<float> t = mModel[0]; // time steps
  std::vector<float> O = mModel[4]; // O values
  std::vector<float> model = norm(O);

  // Print the initial markov model values - for testing
  print({t, O}, "startO.txt");

  // Initial error
  error = leastSquare(model, target);
  
  // Simulated Annealing start

  // Print the first set of Markov parameters
  print_params(alphaBeta[0]);

  // holds the Markov model version being tested
  std::vector<float> tracker;

  // maximum evaluations, evaluation count
  int maxevals = 10000,   evs = 1; 
  // minimum allowed cost
  float mincost = -1097;
  // Anneal
  for (int i = 0; i < 10; ++i) {
    // the first 'x' is the current Markov parameter being tested
    float x = alphaBeta[0][i];
    // initial cost is the error 
    float cost = error; 
    // bests
    float bestcost = cost, bestx = x;

    // annealing constants
    float k = 10;   // "Boltzmann" constant
    float T = 5000; // Initial "temperature"

    // random numbers
    std::mt19937 mt(0); // Random number generator
    std::uniform_real_distribution<float> u(0,1); // Uniform distribution

    // do some evaluations
    while (evs < maxevals && cost > mincost) {
          //Set temperature schedule to lower every 1000 steps
          if ( ! (evs%1000)) { 
            T /= 1.1; 
            x = bestx;
          } 

          // float newx = alphaBeta[0][i] * T/5 * u(mt) - T/10;
          float newx = x * T/5 * u(mt) - T/10; // Tie steps to temperature
          if (newx > ub) { newx = ub; }
          if (newx < lb) { newx = lb; }

          // create the temporary variable for the markov parameters
          std::vector<std::vector<float>> xparams = alphaBeta;
          xparams[0][i] = newx;
          // generate a solution with the changed parameter
          tracker = markov.soln(dt, tmax, V, 0, xparams)[4];
          tracker = norm(tracker);

          // generate a new cost estimate
          float newCost = leastSquare(tracker, target);

          // Metropolis Criteria
          float p = std::exp( (- (newCost-cost) ) / ( k*T ) );
          if (p > u(mt)) { alphaBeta[0][i] = newx; cost=newCost;}
          if (cost < bestcost) {
            bestcost = cost;
            bestx = alphaBeta[0][i];
          }
        
          evs++;
      }
      alphaBeta[0][i] = bestx;
      std::cout << i << " " << "Best x: " << alphaBeta[0][i] << ", Best cost: " << bestcost << " runs: " << evs << "\n";
      evs = 1;  
  }
    print_params(alphaBeta[0]);
    model = markov.soln(dt, tmax, V, 0, alphaBeta)[4];
    
    print({t, model }, "endO.txt");

    std::vector<std::vector<float>> MandHHmodel(2, std::vector<float>(model.size()));
    MandHHmodel[0] = t;
    
    for (int i = 0; i < model.size(); i++){
      MandHHmodel[1][i] = (model[i] * (V - 55.0)) / V;
    }
    print(MandHHmodel, "end.txt");
}
