#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include "Individual.h"
#include "Selection.h"
// #include "SelectionMethods.h"

class GeneticAlgorithm {
  public:
    int numberOfParents;
    int numberOfGenerations;
    
    std::vector<Individual> population;
    std::vector<int> parents;

    void rankSelection();
    void rouletteSelection();

    GeneticAlgorithm(int, int, int);
    
};

GeneticAlgorithm::GeneticAlgorithm(int numParents, int numGenerations, int populationSize) {
  this->numberOfParents = numParents;
  this->numberOfGenerations = numGenerations;
  this->population.resize(populationSize);
  for (int i=0; i<populationSize; i++) {
    Individual individual;
    this->population[i] = individual;
  }
}

void GeneticAlgorithm::rankSelection() {

  Selection* rank = new Rank();
  
}
// void GeneticAlgorithm::process(float (*f)(std::vector<float>), RouletteWheel& roulette) {
//   int populationSize = this->population.size();
//   std::vector<float> o(populationSize);
//   for (int i=0; i<populationSize; i++) {
//     o[i]=(*f)(this->population[i].genes);
//   }
  
//   roulette.rankPopulation(o, this->population);

//   int iter = 0;
//   for (auto r : roulette.ranks) {
//     this->population[r].fitness = roulette.calculateFitness(iter, r, this->population);
//     ++iter;
//   }

//   roulette.selectParents(this->population);
  

  // for (int i = 0; i < roulette.ranks.size(); i++)
  //   printf("%i\n", roulette.ranks[i]); 
  // for ( int r: roulette.ranks)
  //   printf("%i\n", r); 
// }