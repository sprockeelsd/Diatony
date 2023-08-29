#include "headers/FourVoiceTexture.hpp"
#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"
#include "headers/MinorTonality.hpp"
#include "headers/SolutionFinders.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    int size = 3;
    Tonality* tonality = new MajorTonality(C);

    // create a new problem
//    FourVoiceTexture* p = new FourVoiceTexture(size, tonality, {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
//                                                                THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE,
//                                                                FIRST_DEGREE},
//                                               {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
//                                                FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE});
    auto *pb = new FourVoiceTexture(size, tonality, {FIRST_DEGREE, FOURTH_DEGREE},
                                               {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE});
    // std::cout << p->toString() << std::endl;

    /// find the solution that minimizes the costs (maximize the preference satisfaction)
    find_best_solution(pb);
    find_all_solutions(pb, DFS_SOLVER,10);
    delete pb;

    return 0;
}

