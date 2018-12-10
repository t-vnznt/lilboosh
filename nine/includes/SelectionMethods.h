// #include "Selection.h"

class RankSelection: public Selection {
  public:
    RankSelection(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
};

class RouletteWheel: public Selection {
  public:
    RouletteWheel(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
    void selectParents(std::vector<Individual>&);
};



class Recombination: public Selection {
  public:
    Recombination(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
};

class Mutation: public Selection {
  public:
    Mutation(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
};

void RouletteWheel::selectParents(std::vector<Individual>& pop) {
  float line = 0.0;
  for (Individual &p : pop) 
    line += p.fitness;
  
  std::vector<int> parents(this->numParents);
  for (int i = 0; i < this->numParents; ++i) {
    float pick = line * u(mt);
    float popsum = 0.0;

    for (int j = 0; j < this->populationSize; ++j) {
      popsum += pop[j].fitness;
      if (pick < popsum) {
        parents[i] = j;
        break;
      }
    }
  }
  for (auto pr : parents)
    printf("%i\n", pr);
  // return parents;
}