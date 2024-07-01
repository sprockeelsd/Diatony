// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A main function that solves diatonic harmonic problems of tonal music
//

#include "../headers/aux/Utilities.hpp"
#include "../headers/aux/Tonality.hpp"
#include "../headers/aux/MajorTonality.hpp"
#include "../headers/aux/MinorTonality.hpp"
#include "../headers/aux/MidiFileGeneration.hpp"

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

    Tonality* tonality = new MinorTonality(G);
    std::cout << "tonality: " << tonality->to_string() << std::endl;

    std::string search_type = argv[1];
    std::string build_midi = argv[2];

    /// vectors representing the chords and the states
    vector<int> chords = {FIRST_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    //@todo faire une fonction qui remplit automatiquement les qualités par défaut
    vector<int> chords_qualities;
    for(int i = 0; i < chords.size(); i++){
        chords_qualities.push_back(tonality->get_chord_quality(chords[i]));
    }
    chords_qualities[2] = DOMINANT_SEVENTH_CHORD;
    chords_qualities[6] = DOMINANT_SEVENTH_CHORD;
    vector<int> states = {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION,
                           SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};

    int size = chords.size();

    /// create a new problem
    auto *pb = new FourVoiceTexture(size, tonality, chords, chords_qualities, states);
    /// Search options
    Search::Options opts;
    opts.threads = 1;
    opts.stop = Search::Stop::time(600000); // stop after 120 seconds
    opts.cutoff = Search::Cutoff::merge(
            Search::Cutoff::linear(2*size),
            Search::Cutoff::geometric((4*size)^2, 2));
    opts.nogoods_limit = size * 4 * 4;


    //DFS<FourVoiceTexture> solver(pb, opts);
    /// Restart based solver
    RBS<FourVoiceTexture, BAB> solver(pb, opts);
    delete pb;

    FourVoiceTexture* currentBestSol = nullptr;
    int n_sols = 0;
    auto start = std::chrono::high_resolution_clock::now();     /// start time
    while(FourVoiceTexture* next_sol = solver.next()){
        n_sols++;
        std::cout << "temporary solution found (" << n_sols  << ")" << std::endl;
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
    std::cout << "time taken: " << duration.count() << " seconds and " << n_sols << " solutions found.\n" << std::endl;

    if(build_midi == "true" && currentBestSol != nullptr){
        writeSolToMIDIFile(size, "output", currentBestSol);
        cout << "MIDI file created" << endl;
    }
    return 0;
}