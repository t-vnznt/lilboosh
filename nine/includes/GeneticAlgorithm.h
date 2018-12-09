#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include "Individual.h"

class GeneticAlgorithm {
  private:
  public:
    int numParents;
    int numGenerations;
    int populationSize;
    std::vector<Individual> population;
    
    GeneticAlgorithm(int, int, int);
    void initializePopulation();
    void report();
    std::vector<float> process( float (*f)(std::vector<float>));
};

GeneticAlgorithm::GeneticAlgorithm(int p, int g, int s) {
  this->numParents = p;
  this->numGenerations = g;
  this->populationSize = s;
  initializePopulation();
}

void GeneticAlgorithm::initializePopulation() {
  this->population.resize(this->populationSize);
  for (int i = 0; i < this->populationSize; i++) {
    Individual ind;
    this->population[i] = ind;
  }
}

void GeneticAlgorithm::report() {
  int i = 1;
  for (Individual ind:this->population) {
    int j = 0;
    std::cout << "Individual " << i << ":\n";
    std::cout << "  Parameters:\n";
    for (float p:ind.parameters) {  
      std::cout <<"    " << j << ": " << p << "\n";
      j++;
    }
    i++;
  }
}

std::vector<float> GeneticAlgorithm::process(float (*f)(std::vector<float>)) {
  std::vector<float> o(this->populationSize);
  for (int i = 0; i < this->populationSize; i++) {
    o[i] = (*f)(this->population[i].parameters);
  }

  return o;
}