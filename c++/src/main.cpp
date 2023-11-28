#include "../headers/FourVoiceTexture.hpp"
#include "../headers/Utilities.hpp"
#include "../headers/Tonality.hpp"
#include "../headers/MajorTonality.hpp"
#include "../headers/MinorTonality.hpp"
#include "../headers/Solver.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    Tonality* tonality = new MinorTonality(C);

    write_to_log_file(time().c_str());
//    vector<int> chords = {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
//                          THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIRST_DEGREE,
//                          FIFTH_DEGREE, FIRST_DEGREE};
//
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
//                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
//                          FUNDAMENTAL_STATE};
//    vector<int> chords = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE,
//                          SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIFTH_DEGREE,
//                          FIRST_DEGREE};
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
//                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
//                          FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    vector<int> chords = {FIRST_DEGREE, SIXTH_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE,
                          SECOND_INVERSION, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
                          FIRST_INVERSION, FIRST_INVERSION, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    int size = chords.size();

    /// create a new problem
  auto *pb = new FourVoiceTexture(size, tonality, chords, states);

    /// find the solution that minimizes the costs (maximize the preference satisfaction)
    // const FourVoiceTexture *bestSol = find_best_solution(pb);
    /// find all solutions to the problem
    auto start = std::chrono::high_resolution_clock::now();
    find_all_solutions(pb, BAB_SOLVER);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    delete pb;

    return 0;
}