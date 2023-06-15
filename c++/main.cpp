#include "headers/gecode_problem.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    int size = 3;
    int upper_bound_domain = 5;
    int lower_bound_domain = 1;
    // create a new problem
    Problem* p = new Problem(size, lower_bound_domain, upper_bound_domain);

    // create a new search engine
    Search::Base<Problem>* e = make_solver(p, bab_solver);
    delete p;

    int nb_sol = 0;

    while(Problem * pb = get_next_solution_space(e)){
        nb_sol++;
        cout << "Solution " << nb_sol << ": " << endl;
        pb->print_solution();
        delete pb;
    }
    cout << "No (more) solutions." << endl;
    return 0;
}

