#include <iostream>
#include <vector>
#include<random>

std::mt19937 mt(0); 
std::uniform_real_distribution<float> u(0,1);

class Individual {
  private:
    std::vector<float> ubounds = {1, 10, 100};
    std::vector<float> lbounds = {-1, -10, -100};    
  public:
    std::vector<float> parameters;
    float fitness;

    Individual();
};

Individual::Individual() {
  // Allocate space for parameters
  this->parameters.resize(this->ubounds.size());
  // Initialize individual
  for (int i=0; i < this->ubounds.size(); ++i) 
    this->parameters[i] = this->lbounds[i] + (this->ubounds[i] - this->lbounds[i]) * u(mt);

}