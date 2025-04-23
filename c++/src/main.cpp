// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A main function that solves diatonic harmonic problems of tonal music
//

#include "../headers/aux/Utilities.hpp"
#include "../headers/aux/Tonality.hpp"
#include "../headers/aux/MajorTonality.hpp"
#include "../headers/aux/MinorTonality.hpp"
#include "../headers/aux/MidiFileGeneration.hpp"
#include "../headers/diatony/FourVoiceTexture.hpp"
#include "../headers/diatony/SolveDiatony.hpp"

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
    Tonality* tonality = new MajorTonality(C);

    vector<Tonality*> tonalities = {tonality};

    std::string search_type = argv[1];
    std::string build_midi = argv[2];

    /// vectors representing the chords and the states
    vector<int> chords = {FIRST_DEGREE, SIXTH_DEGREE, FIVE_OF_FIVE, FIFTH_DEGREE_APPOGIATURA, FIFTH_DEGREE, FIRST_DEGREE
    };

    vector<int> chords_qualities;
    chords_qualities.reserve(chords.size());
    for(int chord : chords)
        chords_qualities.push_back(tonality->get_chord_quality(chord));

    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE
    };

    // TonalProgressionParameters(const int s, Tonality *t,
    //                             vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas)
    auto sec1params = new TonalProgressionParameters(chords.size(), tonality, chords, chords_qualities, states);
    vector<TonalProgressionParameters*> sectionParams = {sec1params};

    auto pieceParams = new FourVoiceTextureParameters(chords.size(), 1, {0}, {9}, tonalities, sectionParams);

    auto space = new FourVoiceTexture(pieceParams);
    //return 0;

    DFS<FourVoiceTexture> e(space);
    delete space;

    int n_sols = 0;
    while (FourVoiceTexture* sol = e.next()) {
        n_sols += 1;
        std::cout << sol->to_string() << std::endl;
        if (n_sols > 10) break;
    }
    return 0;
}