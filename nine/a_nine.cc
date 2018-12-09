#include<iostream>
#include<random>
#include<vector>
#include<cmath>

float minfun(std::vector<float> x)  {
    return std::pow(x[0]-0.11,2.0)
           +std::pow(x[1]-7.0,2.0)
               +std::pow(x[2]-89.4,2.0);
}

int main()  {
    // Set bounds
    std::vector<float> ub={1,10,100}, lb={-1,-10,-100};

    // Allocate space for our population
    int popnum=50;  //  Number of members in population
    std::vector<std::vector<float> > pop(popnum);
    
    // Initialize population
    std::mt19937 mt(0); std::uniform_real_distribution<float> u(0,1);

    for (auto &p : pop)  {
        // Allocate space for parameters
        p.resize(ub.size());
        // Initialize population
        for (int i=0; i<ub.size(); ++i)
            p[i]=lb[i]+(ub[i]-lb[i])*u(mt);
    }

    // Number of generations
    int gnum=500, best;

    for (int g=0; g<gnum; ++g)  {

        // Evaluate objective function
        std::vector<float> O(popnum);  // Hold obj. fxn values

        // Populate O
        for (int i=0; i<popnum; ++i) O[i]=minfun(pop[i]);

        // Select parents for new generation
        // Rank members
    
        std::vector<int> nums(popnum);
        for (int i=0; i<popnum; ++i)  nums[i]=i;
        std::vector<int> R(popnum); // Vector that holds ranks

        // Find max
        float max=0;
        for (auto o : O) if (o>max) max=o;

        for (int i=0; i<(popnum-1); ++i)  {
            float min=max; int nmin;
            for (auto &n : nums)  {
                if (O[n] < min)  {
                min=O[n];
                nmin=n;
            }
        }

        // Delete nmin from nums
        for (int j=0; j<nums.size(); ++j)
            if (nums[j]==nmin)  {
                nums.erase(nums.begin()+j);
                break;
            }
            // Set R[i] to nmin
            R[i]=nmin;
        }

        // Set the last value
        R[R.size()-1]=nums[0];

        // Compute Fitness
        double S=2; std::vector<float> F(popnum);
        int iter=0;
        for (auto r : R) {
            F[r]=2-S+2*(S-1)*(std::pow((float)(popnum-iter-1), 4)/(float)(popnum-1));
            ++iter;
        } 

        // Roulette Wheel Parents Selection
        int numparents=10; std::vector<int> parents(numparents);

        // How long is our line
        float linelength=0;
        for (auto f : F) linelength+=f;

        for (int i=0; i<numparents; ++i)  {
            float pick=linelength*u(mt);
            float popsum=0;
    
            for (int j=0; j<popnum; ++j)  {
                popsum+=F[j];
                if (pick <popsum)  {
                    parents[i]=j;
                    break;
                }
            }
        }

        //Recombination
        std::vector<std::vector<float>> newpop(popnum);
        for (int i = 0; i < popnum; ++i){
            newpop[i].resize(ub.size());
            std::vector<float> parent1(ub.size());
            std::vector<float> parent2(ub.size());

            for (int j=0; j < ub.size(); ++j){ //One way to modify algorith: use diff way to iterate through parents
                int modnum = 2 * (i % numparents/2);
                parent1[j] = pop[parents[modnum]][j];
                parent2[j] = pop[parents[modnum+1]][j];
            }

            //Find new parameters
            float plb, pub;
            for (int j = 0; j < ub.size(); ++j){
                if (parent1[j] > parent2[j]){
                    plb = parent1[j] - 0.25 * abs(parent1[j]);
                    pub = parent2[j] + 0.25 * abs(parent2[j]);
                } 
                else {
                    plb = parent2[j] - 0.25 * abs(parent2[j]);
                    pub = parent1[j] + 0.25 * abs(parent1[j]);
                }
                // Stay in bounds
                if (plb < lb[j]) plb = lb[j];
                if (pub > ub[j]) pub = ub[j];
                newpop[i][j] = plb + (pub - plb) * u(mt);

            }

        }

        // Set mutation reate
        float mrate = 1/(float)ub.size();
        float msize = 0.025; //As a fraction of the ub and lb

        for (int i = 0; i < popnum; ++i){
            //for each parameter
            for (int j = 0; j < ub.size(); ++j){
                float asize = msize * (ub[j] - lb[j]) * (u(mt));
                if (u(mt) < mrate) {
                    newpop[i][j] += asize;
                    if (newpop[i][j] < lb[j]) newpop[i][j] = lb[j];
                    if (newpop[i][j] > ub[j]) newpop[i][j] = ub[j];
                    mrate = (pop[best][i] - R[i]) / (pop[best][i] - R[i]) * 0.5;
                }
            }
        }

        //Replace old population
        for (int i = 0; i < pop.size(); ++i){
            for (int j = 0; j < ub.size(); ++j){
                pop[i][j] = newpop[i][j];
            }
        }

        best = R[0];
    }
    std::cout << "\n";
    for (int j = 0; j < ub.size(); j++) std::cout << pop[best][j] << " ";
    std::cout << "\n";
}