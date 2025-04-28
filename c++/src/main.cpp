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
 *
 * This function finds the solution for a harmonisation problem, given a series of tonalities and chord degrees, qualities
 * and states for the chords in each tonality. Currently, modulation constraints are not available, but they will be
 * in the near future.
 */
int main(int argc, char* argv[]) {
    /// if there is not exactly 2 argument, there is an error
    if(argc != 3)
        return 1;

    /// Data for the problem
    Tonality* CMajor = new MajorTonality(C);
    Tonality* GMajor = new MajorTonality(G);

    Tonality* CMinor = new MinorTonality(C);
    Tonality* EbMajor = new MajorTonality(E_FLAT);

    vector<Tonality*> tonalities = {CMajor};


    std::string search_type = argv[1];
    std::string build_midi = argv[2];

    Tonality* t1 = CMinor;
    /// vectors representing the chords and the states
    vector<int> chords = {FIRST_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE_APPOGIATURA, FIFTH_DEGREE, FIRST_DEGREE
    };
    vector<int> chords_qualities1;
    chords_qualities1.reserve(chords.size());
    for(int chord : chords)
        chords_qualities1.push_back(t1->get_chord_quality(chord));
    chords_qualities1[4] = DOMINANT_SEVENTH_CHORD;
    vector<int> states1 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE
    };

    auto sec1params = new TonalProgressionParameters(0, static_cast<int>(chords.size()), 0, 5, t1, chords,
        chords_qualities1, states1);


    Tonality* t2 = EbMajor;
    vector<int> chords2 = {FIRST_DEGREE, SIXTH_DEGREE, FIVE_OF_FIVE, FIFTH_DEGREE, FIRST_DEGREE
    };
    vector<int> chords_qualities2;
    chords_qualities2.reserve(chords2.size());
    for(int chord : chords2)
        chords_qualities2.push_back(t2->get_chord_quality(chord));
    vector<int> states2 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE
    };
    auto sec2params = new TonalProgressionParameters(1, static_cast<int>(chords2.size()), 6, 10,  t2, chords2, chords_qualities2, states2);

    vector<TonalProgressionParameters*> sectionParams = {sec1params, sec2params};

    auto mod = new ModulationParameters(PERFECT_CADENCE_MODULATION, 4, 5, sec1params, sec2params);

    vector<ModulationParameters*> modulationParams = {mod};

    auto pieceParams = new FourVoiceTextureParameters(11, 2, sectionParams, modulationParams);

    auto space = new FourVoiceTexture(pieceParams);

    //std::cout << space->to_string() << std::endl;
    //return 0;

    BAB<FourVoiceTexture> e(space);
    delete space;

    int n_sols = 0;
    FourVoiceTexture* lastSol = nullptr;
    while (FourVoiceTexture* sol = e.next()) {
        n_sols += 1;
        lastSol = dynamic_cast<FourVoiceTexture *>(sol->copy());
        std::cout << sol->to_string() << std::endl;
        //if (n_sols >= 1) break;
    }
    std::cout << n_sols << " solutions found." << std::endl;

    if(build_midi == "true" && lastSol != nullptr){
            writeSolToMIDIFile(lastSol->getParameters()->get_totalNumberOfChords(), "../out/MidiFiles/sol", lastSol);
        cout << "MIDI file(s) created" << endl;
    }

    delete sec1params;
    delete sec2params;
    delete mod;
    delete pieceParams;

    return 0;
}