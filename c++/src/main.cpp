#include "../headers/FourVoiceTexture.hpp"
#include "../headers/Utilities.hpp"
#include "../headers/Tonality.hpp"
#include "../headers/MajorTonality.hpp"
#include "../headers/MinorTonality.hpp"
#include "../headers/Solver.hpp"
#include "../headers/MidiFile.h"
#include "../headers/MidiFileGeneration.hpp"

#include <gecode/gist.hh>

using namespace Gecode;
using namespace std;
using namespace smf;

/**
 * Finds solutions to a musical problem
 * Takes 2 arguments:
 * - the first one specifies whether we need to find all solutions or just the best one
 * - The second specifies whether we need to create a MIDI file or not
 */
int main(int argc, char* argv[]) {
    // if there is not exactly 2 argument, there is an error
    if(argc != 3)
        return 1;

    Tonality* tonality = new MajorTonality(A_FLAT);
    write_to_log_file(time().c_str(), LOG_FILE);

    std::string search_type = argv[1];
    std::string build_midi = argv[2];

    /// vectors representing the chords and the states
//    vector<int> chords = {FIRST_DEGREE, FIRST_DEGREE};
//    vector<int> chords_qualities = {MAJOR_CHORD, MAJOR_CHORD};
//    vector<int> states = {FUNDAMENTAL_STATE, FIRST_INVERSION};

//    vector<int> chords = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIRST_DEGREE, FIFTH_DEGREE};
//    vector<int> chords_qualities = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD};
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};

    vector<int> chords = {FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIFTH_DEGREE};
    vector<int> chords_qualities = {MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD};
    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION};

    int size = chords.size();

    /// create a new problem
    auto *pb = new FourVoiceTexture(size, tonality, chords, chords_qualities, states,
                                    RIGHT_TO_LEFT, VAL_RND);
    /// Search options
    Search::Options opts;
    opts.threads = 1;
    opts.stop = Search::Stop::time(600000); // stop after 120 seconds
    opts.cutoff = Search::Cutoff::luby(size);
    opts.nogoods_limit = size * 3;


    //BAB<FourVoiceTexture> solver(pb, opts);
    /// Restart based solver
    RBS<FourVoiceTexture, BAB> solver(pb, opts);
    delete pb;

    FourVoiceTexture* currentBestSol = nullptr;
    auto start = std::chrono::high_resolution_clock::now();     /// start time
    while(FourVoiceTexture* next_sol = solver.next()){
        std::cout << "temporary solution found" << std::endl;
        currentBestSol = (FourVoiceTexture*) next_sol->copy();
        std::cout << currentBestSol->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
        delete next_sol;
    }
    std::cout << "search over" << std::endl;
    auto currTime = std::chrono::high_resolution_clock::now();     /// current time
    std::chrono::duration<double> duration = currTime - start;
    if(solver.stopped()){
        std::cout << "Best solution not found within the time limit." << std::endl;
        std::cout << currentBestSol->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
    }
    else if(currentBestSol == nullptr){
        std::cout << "No solutions" << std::endl;
    }
    else{
        std::cout << "Best solution found" << std::endl;
        std::cout << currentBestSol->to_string() << std::endl;
        std::cout << statistics_to_string(solver.statistics()) << std::endl;
    }
    std::cout << "time taken: " << duration.count() << " seconds" << std::endl;

    if(build_midi == "true" && currentBestSol != nullptr){
        writeSolToMIDIFile(size, "output", currentBestSol);
        cout << "MIDI file created" << endl;
    }

    return 0;
}