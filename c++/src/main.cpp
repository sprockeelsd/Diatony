// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A main function that solves diatonic harmonic problems of tonal music
//

#include "../headers/aux/Utilities.hpp"
#include "../headers/aux/Tonality.hpp"
#include "../headers/aux/MajorTonality.hpp"
#include "../headers/aux/MinorTonality.hpp"
#include "../headers/aux/MidiFileGeneration.hpp"
#include "../headers/diatony/SolveDiatony.hpp"

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
    /// if there is not exactly 2 argument, there is an error
    if(argc != 3)
        return 1;

    /// Data for the problem
    Tonality* tonality = new MinorTonality(G);
    std::cout << "tonality: " << tonality->to_string() << std::endl;

    std::string search_type = argv[1];
    std::string build_midi = argv[2];

    /// vectors representing the chords and the states
    vector<int> chords = {FIRST_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
    vector<int> chords_qualities;
    for(int i = 0; i < chords.size(); i++){
        chords_qualities.push_back(tonality->get_chord_quality(chords[i]));
    }
    chords_qualities[2] = DOMINANT_SEVENTH_CHORD;
    chords_qualities[6] = DOMINANT_SEVENTH_CHORD;
    vector<int> states = {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION,
                           SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};

    int size = chords.size();
    /// Solve the problem
    FourVoiceTexture* currentBestSol = solve_diatony_problem_optimal(size, tonality, chords, chords_qualities, states);

    if(build_midi == "true" && currentBestSol != nullptr){
        writeSolToMIDIFile(size, "../out/Midifiles/output", currentBestSol);
        cout << "MIDI file created" << endl;
    }
    return 0;
}