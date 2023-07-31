#include "headers/four_voice_texture.hpp"
#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"
#include "headers/MinorTonality.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    int size = 2;
    Tonality* tonality = new MajorTonality(C);

    // create a new problem
//    FourVoiceTexture* p = new FourVoiceTexture(size, tonality, {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
//                                                                THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE,
//                                                                FIRST_DEGREE},
//                                               {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
//                                                FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE});
    FourVoiceTexture *p = new FourVoiceTexture(size, tonality, {SECOND_DEGREE, FIFTH_DEGREE},
                                               {FIRST_INVERSION, FUNDAMENTAL_STATE});
    // std::cout << p->toString() << std::endl;


    // create a new search engine
    Search::Base<FourVoiceTexture>* e = make_solver(p, BAB_SOLVER);
    delete p;

    std::cout << "Solver type : DFS " << std::endl;
    int nb_sol = 0;
    std::cout << std::endl;
    while(FourVoiceTexture * sol = get_next_solution_space(e)){
        nb_sol++;
        cout << "Solution " << nb_sol << ": " << endl;
        //sol->print_solution();
        std::cout << sol->toString() <<std::endl;
        writeToLogFile(sol->toString().c_str());
        std::cout << statistics_to_string(e->statistics()) << std::endl;
        delete sol;
        if (nb_sol >= 10)
            break;
    }
    cout << "No (more) solutions or solution cap reached.\n" << endl;
    return 0;
}

