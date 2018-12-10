#include <iostream>
#include "GeneticAlgorithm.h"
#include "Objective.h"


int main() {
  int numParents = 2;
  int numGenerations = 4;
  int populationSize = 16;
  double S = 2.0;
  GeneticAlgorithm ga(numParents, numGenerations, populationSize);
  
  // Print stuff for testing
  for (int i = 0; i < ga.population.size(); i++) {
    printf("%12s%3i\n", "Individual:", i+1);
    Individual p = ga.population[i];
    for (int j = 0; j < p.genes.size(); j++) {
      printf("%4i:%10.6f\n", j+1, p.genes[j]);
    }
    printf("\n");
  }
  ga.rankSelection();
  // std::vector<float> O;
  // ga.process(Min::objective, roulette);
  // printf("\nFitness:\n");
  // for (Individual p: ga.population)
  //   printf("%f\n", p.fitness);
  
  // for ( int r: roulette.ranks)
  //   printf("%i\n", r); 
  
}