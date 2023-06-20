#include "headers/four_voice_texture.hpp"
#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    int size = 3;
    int upper_bound_domain = 5;
    int lower_bound_domain = 1;

    // create a new problem
    FourVoiceTexture* p = new FourVoiceTexture(size, lower_bound_domain, upper_bound_domain);

    Tonality* tonality = new MajorTonality(C);

    // create a new search engine
    Search::Base<FourVoiceTexture>* e = make_solver(p, bab_solver);
    delete p;

    int nb_sol = 0;

    while(FourVoiceTexture * sol = get_next_solution_space(e)){
        nb_sol++;
        cout << "Solution " << nb_sol << ": " << endl;
        sol->print_solution();
        delete sol;
    }
    cout << "No (more) solutions." << endl;
    return 0;
}

