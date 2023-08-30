#include "headers/FourVoiceTexture.hpp"
#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"
#include "headers/MinorTonality.hpp"
#include "headers/Solver.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    int size = 3;
    Tonality* tonality = new MajorTonality(C);

    /// date and time for logs
    std::time_t currentTime = std::time(nullptr); // Get the current time
    std::string timeString = std::asctime(std::localtime(&currentTime)); // Convert to string
    write_to_log_file(timeString.c_str());

    /// create a new problem
//  auto *p = new FourVoiceTexture(size, tonality, {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
//                                                                THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE,
//                                                                FIRST_DEGREE},
//                                               {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
//                                                FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE});
    auto *pb = new FourVoiceTexture(size, tonality, {FIRST_DEGREE},
                                               {FUNDAMENTAL_STATE});

    /// find the solution that minimizes the costs (maximize the preference satisfaction)
    find_best_solution(pb);
    /// find all solutions to the problem
    //find_all_solutions(pb, DFS_SOLVER);
    delete pb;

    return 0;
}

