#include <iostream>
#include "GeneticAlgorithm.h"
#include "Objective.h"

int main() {
  GeneticAlgorithm ga(2,4,16);
  
  std::vector<float> O;
  
  O = ga.process(Min::objective);
  for (int i = 0; i < O.size(); i++) {
    std::cout << O[i] << "\n";
  }

}