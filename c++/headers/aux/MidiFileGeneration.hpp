// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A class allowing to write MIDI files
//
#ifndef MYPROJECT_MIDIFILEGENERATION_HPP
#define MYPROJECT_MIDIFILEGENERATION_HPP

#include "Utilities.hpp"
#include "Tonality.hpp"
#include "MajorTonality.hpp"
#include "MinorTonality.hpp"

#include "../diatony/TonalProgression.hpp"

#include "../midifile/MidiFile.h"

using namespace Gecode;
using namespace std;
using namespace smf;

/**
 * Writes a solution to a MIDI file
 * @param size the size of the problem
 * @param sol the solution to write
 * @return the time at which the solution ends
 */
void writeSolToMIDIFile(int size, const string& fileName, const TonalProgression* sol);

/**
 * Writes all the solutions to a MIDI file
 * @param size the size of the problem
 * @param rhythm the rhythm vector of the problem
 * @param sols the solutions to write
 * @param file the MIDI file to write to
 */
void writeSolsToMIDIFile(int size, vector<const TonalProgression*> sols);

#endif //MYPROJECT_MIDIFILEGENERATION_HPP
