#include<vector>
#include<stdio.h>

class Mutation {
  public:

};

class Recombine {
  public:

};

class Selection {
  public:
    Selection() {};
};

class Rank: public Selection {
  public:
    Rank() : Selection() {};
    
};

class Roulette: public Selection {
  public:
    Roulette() : Selection() {};
};


// #include <vector>

// class Selection;
// class RankSelection; class RouletteWheel;

// class Selection {
//   public:
//     Selection(int, int, double);
//     int populationSize;
//     int numParents;
//     double S;

//     float calculateFitness(int, int, std::vector<Individual>&);
//     void rankPopulation(std::vector<float>, std::vector<Individual>&);
// };
// class RankSelection: public Selection {
//   public:
//     RankSelection(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
// };

// class RouletteWheel: public Selection {
//   public:
//     RouletteWheel(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
//     void selectParents(std::vector<Individual>&);
// };

// class Recombination: public Selection {
//   public:
//     Recombination(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
// };

// class Mutation: public Selection {
//   public:
//     Mutation(int popnum, int numparents, int s): Selection(popnum, numparents, s) {};
// };

// void RouletteWheel::selectParents(std::vector<Individual>& pop) {
//   float line = 0.0;
//   for (Individual &p : pop) 
//     line += p.fitness;
  
//   std::vector<int> parents(this->numParents);
//   for (int i = 0; i < this->numParents; ++i) {
//     float pick = line * u(mt);
//     float popsum = 0.0;

//     for (int j = 0; j < this->populationSize; ++j) {
//       popsum += pop[j].fitness;
//       if (pick < popsum) {
//         parents[i] = j;
//         break;
//       }
//     }
//   }
//   for (auto pr : parents)
//     printf("%i\n", pr);
//   // return parents;
// }

// Selection::Selection(int popnum, int numparents, double s) {
//   this->populationSize = popnum;
//   this->ranks.resize(popnum);
//   // this->parents.resize(numparents);
//   this->S = s;
// }

// void Selection::rankPopulation(std::vector<float> obj, std::vector<Individual>& pop) {
//   int popnum = this->populationSize;
//   std::vector<int> nums(popnum);

  
//   for (int i = 0; i < popnum; ++i) nums[i] = i;
  
//   // Find max
//   float max = 0;
//   for (auto o : obj) if (o > max) max = o;
  
//   for (int i = 0; i < (popnum - 1); ++i) {
//     float min = max; int nmin;
//     for ( int &n : nums) {
//       if (obj[n] < min) {
//         min = obj[n];
//         nmin = n;
//       }
//     }
    
//     // Delete nmin from nums
//     for (int j = 0; j < nums.size(); ++j) 
//       if (nums[j] == nmin) {
//         nums.erase(nums.begin() + j);
//         break;
//       }
    
//     // Set rank[i] to min
//     this->ranks[i] = nmin;

//     std::cout << nmin << "\n";
//   }

//   // Set the las value
//   this->ranks[this->ranks.size() - 1] = nums[0];
// }

// float Selection::calculateFitness(int iter, int r, std::vector<Individual>& pop) {
//   return 2.0 - this->S + 2.0 * (this->S - 1.0) * (std::pow((float)(this->populationSize - iter - 1), 4)/(float)(this->populationSize - 1));
// }