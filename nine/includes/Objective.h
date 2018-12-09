#include <vector>
#include <cmath>

class Objective {
  public:
    Objective();
};

class Min: public  Objective {
  public:
    Min() : Objective() {};
    static float objective(std::vector<float>);
};

float Min::objective(std::vector<float> x) {
  return std::pow(x[0]-0.11,2.0)
            +std::pow(x[1]-7.0,2.0)
                +std::pow(x[2]-89.4,2.0);
}