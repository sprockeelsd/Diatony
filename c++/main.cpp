#include "headers/FourVoiceTexture.hpp"
#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"
#include "headers/MinorTonality.hpp"
#include "headers/Solver.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    Tonality* tonality = new MajorTonality(C);

    write_to_log_file(time().c_str());
    vector<int> chords = {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
                          THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIRST_DEGREE,
                          FIFTH_DEGREE, FIRST_DEGREE};

    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
                          FUNDAMENTAL_STATE};
    int size = chords.size();

    /// create a new problem
  auto *pb = new FourVoiceTexture(size, tonality, chords, states);

    /// find the solution that minimizes the costs (maximize the preference satisfaction)
    const FourVoiceTexture *bestSol = find_best_solution(pb);
    /// find all solutions to the problem
    //find_all_solutions(pb, DFS_SOLVER);
    delete pb;

    return 0;
}

