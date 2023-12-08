#include "../headers/MidiFileGeneration.hpp"

/**
 * Writes a solution to a MIDI file
 * @param size the size of the problem
 * @param sol the solution to write
 * @return the time at which the solution ends
 */
void writeSolToMIDIFile(int size, int actionTime, int tpq, vector<const FourVoiceTexture *> sols, MidiFile outputFile) {
    /// array of integers representing the rhythm
    int rhythm[size];
    for(int i = 0; i < size; i++)
        rhythm[i] = 4;
    /// if there are no solutions, exit
    if(sols.empty()){
        return;
    }
    vector<uchar> midiEvent;        // temporary storage of MIDI events
    midiEvent.resize(3);        //set the size of the array to 3 bites (first bite = Start or end of a note, second bite = note value, third bite = velocity

    /// get the best solution
    auto bestSolution = sols[sols.size() - 1]; // the last one is the best
    int* sol_notes = bestSolution->return_solution();
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
    outputFile.write("../out/output.mid");
    std::cout << "MIDIfile created." << std::endl;
}

/**
 * Writes all the solutions to a MIDI file
 * @param size the size of the problem
 * @param rhythm the rhythm vector of the problem
 * @param sols the solutions to write
 * @param file the MIDI file to write to
 */
void writeSolsToMIDIFile(int size, int*rhythm, vector<const FourVoiceTexture*> sols, MidiFile file){
    file.absoluteTicks();       // time information stored as absolute time, will be converted to delta time when written
    file.addTrack(1);     // Add a track to the file (track 0 must be left empty, so add as many as we use
    int tpq = 120;              // default value in MIDI file is 48 (tempo)
    file.setTicksPerQuarterNote(tpq);

    int actionTime = 0;
    for(int i = 0; i < sols.size(); i++){
        //actionTime = writeSolToMIDIFile(size, sols);
    }
    file.sortTracks(); // make sure data is in correct order
    file.write("../out/output.mid");
    std::cout << "MIDI file created" << std::endl;
    write_to_log_file("MIDI file created\n");
}

