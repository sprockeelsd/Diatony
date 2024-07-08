// Author: Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description: A class allowing to write MIDI files
//
#include "../../headers/aux/MidiFileGeneration.hpp"

/**
 * Writes a solution to a MIDI file
 * @param size the size of the problem
 * @param sol the solution to write
 * @return the time at which the solution ends
 */
void writeSolToMIDIFile(int size, string fileName, const FourVoiceTexture *sol) {
    MidiFile outputFile;
    outputFile.absoluteTicks();     // time information stored as absolute time, will be converted to delta time when written
    outputFile.addTrack(1);   // Add a track to the file (track 0 must be left empty, so add as many as we use

    int tpq = 120;                  // default value in MIDI file is 48 (tempo)
    outputFile.setTicksPerQuarterNote(tpq);

    int actionTime = 0;
    /// array of integers representing the rhythm
    int rhythm[size];
    for(int i = 0; i < size; i++){
        rhythm[i] = 4;
    }

    vector<uchar> midiEvent;        // temporary storage of MIDI events
    midiEvent.resize(3);        //set the size of the array to 3 bites (first bite = Start or end of a note, second bite = note value, third bite = velocity

    int* sol_notes = sol->return_solution();
    /// Fill the MidiFile object
    midiEvent[2] = 64; // store attack/release velocity for note command
    for(int i = 0; i < size; i++){
        midiEvent[0] = 0x90; /// add the start of the note
        for(int j = 0; j < 4; j++){
            midiEvent[1] = sol_notes[4*i+j];
            outputFile.addEvent(1, actionTime, midiEvent);
        }
        actionTime += tpq*rhythm[i]; // increase relative time to the end of these events
        midiEvent[0] = 0x80; /// add the end of the note
        for(int j = 0; j < 4; j++){
            midiEvent[1] = sol_notes[4*i+j];
            outputFile.addEvent(1, actionTime, midiEvent);
        }
    }
    outputFile.sortTracks(); // make sure data is in correct order
    outputFile.write(fileName + ".mid");
}

/**
 * Writes all the solutions to a MIDI file
 * @param size the size of the problem
 * @param rhythm the rhythm vector of the problem
 * @param sols the solutions to write
 * @param file the MIDI file to write to
 */
void writeSolsToMIDIFile(int size, vector<const FourVoiceTexture *> sols) {
    for (int i = 0; i < sols.size(); i++){
        writeSolToMIDIFile(size, "output" + to_string(i), sols[i]);
    }
    std::cout << to_string(sols.size()) + " MIDI files created." << std::endl;
}

