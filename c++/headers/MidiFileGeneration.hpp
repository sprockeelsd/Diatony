#ifndef MYPROJECT_MIDIFILEGENERATION_HPP
#define MYPROJECT_MIDIFILEGENERATION_HPP

#include "FourVoiceTexture.hpp"
#include "Utilities.hpp"
#include "Tonality.hpp"
#include "MajorTonality.hpp"
#include "MinorTonality.hpp"
#include "Solver.hpp"
#include "MidiFile.h"

#include <gecode/gist.hh>

using namespace Gecode;
using namespace std;
using namespace smf;

/**
 * Writes a solution to a MIDI file
 * @param size the size of the problem
 * @param sol the solution to write
 * @return the time at which the solution ends
 */
void writeSolToMIDIFile(int size, vector<const FourVoiceTexture *> sols, MidiFile outputFile);

/**
 * Writes all the solutions to a MIDI file
 * @param size the size of the problem
 * @param rhythm the rhythm vector of the problem
 * @param sols the solutions to write
 * @param file the MIDI file to write to
 */
void writeSolsToMIDIFile(int size, vector<const FourVoiceTexture *> sols);

#endif //MYPROJECT_MIDIFILEGENERATION_HPP
