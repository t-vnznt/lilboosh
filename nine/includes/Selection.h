#include <vector>

class Selection {
  public:
    Selection();
    void initializeRanks(int, std::vector<float>);
    std::vector<int> ranks;

};

Selection::Selection() {
  
}
