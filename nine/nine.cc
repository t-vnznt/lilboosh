#include "Population.h"

// float minfun(std::vector<float> x)  {
// 	return std::pow(x[0]-0.11,2.0)
// 	       +std::pow(x[1]-7.0,2.0)
//                +std::pow(x[2]-89.4,2.0);
// }

int main() {
  int popnum = 5;
  int gencount = 1;
  int best;

  Population pop = Population(popnum);

  for (int g=0; g < gencount; ++g) {
    std::vector<float> O(popnum);
    for (int i=0; i<popnum; ++i) O[i]=minfun(pop.pop[i]);

    std::cout << "\nGeneration " << g << "\n";
    for (auto o : O) std::cout << o << " ";
		std::cout << "\n";

    pop.selectParents(O);
  }
}