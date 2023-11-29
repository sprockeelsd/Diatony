#include "../headers/FourVoiceTexture.hpp"
#include "../headers/Utilities.hpp"
#include "../headers/Tonality.hpp"
#include "../headers/MajorTonality.hpp"
#include "../headers/MinorTonality.hpp"
#include "../headers/Solver.hpp"
#include "../headers/MidiFile.h"

using namespace Gecode;
using namespace std;
using namespace smf;

int main(int argc, char* argv[]) {
    Tonality* tonality = new MajorTonality(A);

    write_to_log_file(time().c_str());
//    vector<int> chords = {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
//                          THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIRST_DEGREE,
//                          FIFTH_DEGREE, FIRST_DEGREE};
//
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
//                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
//                          FUNDAMENTAL_STATE};
    vector<int> chords = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE,
                          SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIFTH_DEGREE,
                          FIRST_DEGREE};
    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
                          FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
                          FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    /// vectors representing the chords and the states
//    vector<int> chords = {FIRST_DEGREE, SIXTH_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE,
//                          SECOND_INVERSION, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
//    vector<int> states = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE,
//                          FIRST_INVERSION, FIRST_INVERSION, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
    int size = chords.size();
    /// array of integers representing the rhythm
    int rhythm[size];
    for(int i = 0; i < size; i++)
        rhythm[i] = 4;

    /// create a new problem
    auto *pb = new FourVoiceTexture(size, tonality, chords, states);

    /// find the solution that minimizes the costs (maximize the preference satisfaction)
    // const FourVoiceTexture *bestSol = find_best_solution(pb);
    /// find all solutions to the problem
    auto start = std::chrono::high_resolution_clock::now();/// start time
    auto sols = find_all_solutions(pb, BAB_SOLVER);
    auto end = std::chrono::high_resolution_clock::now();/// end time
    delete pb;

    /// total time spent searching
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    ///-------------------------------------------create the MIDI file-----------------------------------------------///
    MidiFile outputFile;
    outputFile.absoluteTicks(); // time information stored as absolute time, will be converted to delta time when written

    outputFile.addTrack(1);   // Add a track to the file (track 0 must be left empty, so add as many as we use
    vector<uchar> midiEvent;   // temporary storage of MIDI events
    midiEvent.resize(3);   //set the size of the array to 3 bites (first bite = Start or end of a note, second bite = note value, third bite = velocity
    int tpq = 120;            // default value in MIDI file is 48 (tempo)
    outputFile.setTicksPerQuarterNote(tpq);

    /// get the best solution
    auto bestSolution = sols[sols.size() - 1]; // the last one is the best
    int* sol_notes = bestSolution->return_solution();

    /// Fill the MidiFile object
    int actionTime = 0; // relative time for MIDI events (equivalent to rhythm)
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

    return 0;
}