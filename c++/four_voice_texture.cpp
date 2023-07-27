#include <utility>

#include "headers/four_voice_texture.hpp"

/***********************************************************************************************************************
 *                                          FourVoiceTexture class methods                                             *
 ***********************************************************************************************************************/

/**
 * Constructor
 * @param s the size of the array of variables
 * @param *t a pointer to a Tonality object
 * @param chordDegs the degrees of the chord of the chord progression
 * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
 */
FourVoiceTexture::FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordStas){
    size = s;
    tonality = t;
    chordDegrees = std::move(chordDegs);
    chordStates = chordStas;

    /** variable initialization */
    FullChordsVoicing = IntVarArray(*this, 4*size, 0,127); // tonality->get_tonality_notes()
    nDifferentValuesInDiminishedChord = IntVarArray(*this, size, 0, 4);

    // variable arrays for melodic intervals for each voice
    bassMelodicIntervals = IntVarArray(*this, size-1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    tenorMelodicIntervals = IntVarArray(*this, size-1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    altoMelodicIntervals = IntVarArray(*this, size-1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    sopranoMelodicIntervals = IntVarArray(*this, size-1, -PERFECT_OCTAVE, PERFECT_OCTAVE);

    // variable arrays for absolute melodic intervals for each voice
    absoluteBassMelodicIntervals = IntVarArray(*this, size-1, 0, PERFECT_OCTAVE);
    absoluteTenorMelodicIntervals = IntVarArray(*this, size-1, 0, PERFECT_OCTAVE);
    absoluteAltoMelodicIntervals = IntVarArray(*this, size-1, 0, PERFECT_OCTAVE);
    absoluteSopranoMelodicIntervals = IntVarArray(*this, size-1, 0, PERFECT_OCTAVE);

    // variable arrays for harmonic intervals between adjacent voices (only positive because there is no direction
    bassTenorHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    bassAltoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    bassSopranoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    tenorAltoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE);
    tenorSopranoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE);
    altoSopranoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE);

    sumOfMelodicIntervals = IntVar(*this, 0, PERFECT_OCTAVE * 4 * size + 1);
    nOfDiminishedChordsWith4notes = IntVar(*this, 0, size);
    nOfFundamentalStateChordsWithoutDoubledBass = IntVar(*this, 0, size);

    /** constraints */

    /// link the arrays together
    link_melodic_arrays(*this, size, FullChordsVoicing, bassMelodicIntervals, tenorMelodicIntervals,
                        altoMelodicIntervals, sopranoMelodicIntervals);

    link_absolute_melodic_arrays(*this, bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals,
                                 sopranoMelodicIntervals, absoluteBassMelodicIntervals, absoluteTenorMelodicIntervals,
                                 absoluteAltoMelodicIntervals, absoluteSopranoMelodicIntervals);

    link_harmonic_arrays(*this, size, FullChordsVoicing, bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                         bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals, tenorSopranoHarmonicIntervals,
                         altoSopranoHarmonicIntervals);

    /// restrain the domain of the voices to their range + state that bass <= tenor <= alto <= soprano
    restrain_voices_domains(*this, size, FullChordsVoicing);

    /// set the cost variables
    linear(*this, IntVarArgs() << absoluteTenorMelodicIntervals << absoluteAltoMelodicIntervals <<
                               absoluteSopranoMelodicIntervals << absoluteBassMelodicIntervals,
           IRT_EQ, sumOfMelodicIntervals); // sumOfMelodicIntervals is the sum of the absolute melodic intervals

    link_diminished_chords_cost(*this, size, *tonality, chordDegrees, FullChordsVoicing,
                                nDifferentValuesInDiminishedChord, nOfDiminishedChordsWith4notes);


    /**-------------------------------------------- generic constraints -----------------------------------------------*/

    for(int i = 0; i < size-1; i++){
        for(int interval : {PERFECT_FIFTH, PERFECT_OCTAVE, UNISSON}){
            /// from bass
            forbid_parallel_intervals(*this, interval, i, BASS,
                                      TENOR,bassTenorHarmonicIntervals, FullChordsVoicing); // between bass and tenor
            forbid_parallel_intervals(*this, interval, i, BASS,
                                      ALTO,bassAltoHarmonicIntervals, FullChordsVoicing); // between tenor and alto
            forbid_parallel_intervals(*this, interval, i, BASS,
                                      SOPRANO,bassSopranoHarmonicIntervals, FullChordsVoicing); // between alto and soprano
            /// from tenor
            forbid_parallel_intervals(*this, interval, i, TENOR,
                                      ALTO, tenorAltoHarmonicIntervals, FullChordsVoicing); // between tenor and alto
            forbid_parallel_intervals(*this, interval, i, TENOR,
                                      SOPRANO, tenorSopranoHarmonicIntervals, FullChordsVoicing); // between tenor and soprano
            /// from alto
            forbid_parallel_intervals(*this, interval, i, ALTO,
                                      SOPRANO, altoSopranoHarmonicIntervals, FullChordsVoicing); // between alto and soprano
        }
    }

    /**------------------------------------------chord related constraints --------------------------------------------*/
    for(int i = 0; i < size; i++){
        IntVarArgs currentChord(FullChordsVoicing.slice(4 * i, 1, 4));

        setToChord(*this, tonality, chordDegrees[i], currentChord);
        setBass(*this, tonality, chordDegrees[i], chordStates[i], currentChord);

        if(chordStas[i] == FUNDAMENTAL_STATE){
            // @todo change this to take into account c/i chords (see cst def)
            if(i == 0)// handle the case where there is no previous chord
                chordNoteOccurrenceFundamentalState(*this, tonality, chordDegrees[i],
                                                    chordDegrees[i], currentChord);
            chordNoteOccurrenceFundamentalState(*this, tonality, chordDegrees[i],
                                                chordDegrees[i-1], currentChord);
        }
        else if(chordStas[i] == FIRST_INVERSION){
            chordNoteOccurrenceFirstInversion(*this, tonality, chordDegrees[i], currentChord);
        }
        else{ // first or second inversion

        }
    }

    /**--------------------------------------------melodic constraints ------------------------------------------------*/
    for(int i = 0; i < size-1; i++){
        if (chordStas[i] == FUNDAMENTAL_STATE){
            std::cout << "fundamental state 1" << std::endl;
            if (chordStas[i+1] == FUNDAMENTAL_STATE){
                std::cout << "fundamental state 2" << std::endl;
                fundamentalStateChordToFundamentalStateChord(*this, i, chordDegrees,
                                                             *tonality,
                                                             bassMelodicIntervals,
                                                             tenorMelodicIntervals,
                                                              altoMelodicIntervals,
                                                              sopranoMelodicIntervals,
                                                              FullChordsVoicing);
            }
        }
    }

    /**------------------------------------------------branching-------------------------------------------------------*/
    branch(*this, FullChordsVoicing, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
}

/**
 * Copy constructor
 * @param s an instance of the FourVoiceTexture class
 */
FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s): IntLexMinimizeSpace(s){
    //IntVars update
    size = s.size;
    tonality = s.tonality;
    chordDegrees = s.chordDegrees;
    chordStates = s.chordStates;

    bassMelodicIntervals.update(*this, s.bassMelodicIntervals);
    tenorMelodicIntervals.update(*this, s.tenorMelodicIntervals);
    altoMelodicIntervals.update(*this, s.altoMelodicIntervals);
    sopranoMelodicIntervals.update(*this, s.sopranoMelodicIntervals);

    absoluteBassMelodicIntervals.update(*this, s.absoluteBassMelodicIntervals);
    absoluteTenorMelodicIntervals.update(*this, s.absoluteTenorMelodicIntervals);
    absoluteAltoMelodicIntervals.update(*this, s.absoluteAltoMelodicIntervals);
    absoluteSopranoMelodicIntervals.update(*this, s.absoluteSopranoMelodicIntervals);

    bassTenorHarmonicIntervals.update(*this, s.bassTenorHarmonicIntervals);
    tenorAltoHarmonicIntervals.update(*this, s.tenorAltoHarmonicIntervals);
    altoSopranoHarmonicIntervals.update(*this, s.altoSopranoHarmonicIntervals);

    FullChordsVoicing.update(*this, s.FullChordsVoicing);
    nDifferentValuesInDiminishedChord.update(*this, s.nDifferentValuesInDiminishedChord);

    sumOfMelodicIntervals.update(*this, s.sumOfMelodicIntervals);
    nOfDiminishedChordsWith4notes.update(*this, s.nOfDiminishedChordsWith4notes);
    nOfFundamentalStateChordsWithoutDoubledBass.update(*this, s.nOfFundamentalStateChordsWithoutDoubledBass);
}

/**
 * Returns the size of the problem
 * @return an integer representing the size of the vars array
 */
int FourVoiceTexture::getSize() const{
    return size;
}

/**
 * Returns the values taken by the variables vars in a solution
 * @return an array of integers representing the values of the variables in a solution
 */
int* FourVoiceTexture::return_solution(){
    int* solution = new int[size*4];
    for(int i = 0; i < 4*size; i++){
        solution[i] = FullChordsVoicing[i].val();
    }
    return solution;
}

/**
 * Copy method
 * @return a copy of the current instance of the FourVoiceTexture class. Calls the copy constructor
 */
Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
}

/**
 * Constrain method for bab search
 * Ensures that exactly 2*b.size notes are identical to the ones in the previous solution
 * @param _b a solution to the problem from which we wish to add a constraint for the next solutions
 */
//void FourVoiceTexture::constrain(const Space& _b) {
//    const auto &b = dynamic_cast<const FourVoiceTexture &>(_b);
//}
/**
 * Cost function for lexical minimization
 * @return the cost variables in order of importance
 */
IntVarArgs FourVoiceTexture::cost() const {
    return {sumOfMelodicIntervals, nOfDiminishedChordsWith4notes}; //@todo add nOfFundamentalStateChordsWithoutDoubledBass
}

/**
 * Prints the solution in the console
 */
void FourVoiceTexture::print_solution(){
    for(int i = 0; i < 4*size; i++){
        cout << FullChordsVoicing[i].val() << " ";
    }
    cout << endl;
}

/**
 * toString method
 * @todo make a toString method for IntVarArrays so the code here is cleaner
 * @return a string representation of the current instance of the FourVoiceTexture class.
 * Right now, it returns a string "FourVoiceTexture object. size = <size>"
 * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
 */
string FourVoiceTexture::toString(){
    string message = "";
    message += "******************************************\n";
    message += "*        FourVoiceTexture object.        *\n";
    message += "******************************************\n";
    message += "----------------parameters----------------\n";
    message += "size = " + to_string(size) + "\n.";
    message += "chord degrees = " + int_vector_to_string(chordDegrees) + "\n";
    message += "chord states = " + int_vector_to_string(chordStates) + "\n";

    message += "----------------variables-----------------\n";

    message += "BassTenorHarmonicIntervals = " + intVarArrayToString(bassTenorHarmonicIntervals) + "\n";
    message += "TenorAltoHarmonicIntervals = " + intVarArrayToString(tenorAltoHarmonicIntervals) + "\n";
    message += "AltoSopranoHarmonicIntervals = " + intVarArrayToString(altoSopranoHarmonicIntervals) + "\n\n";

    message += "BassMelodicIntervals = " + intVarArrayToString(bassMelodicIntervals) + "\n";
    message += "TenorMelodicIntervals = " + intVarArrayToString(tenorMelodicIntervals) + "\n";
    message += "AltoMelodicIntervals = " + intVarArrayToString(altoMelodicIntervals) + "\n";
    message += "SopranoMelodicIntervals = " + intVarArrayToString(sopranoMelodicIntervals) + "\n\n";

    message += "absoluteBassMelodicIntervals = " + intVarArrayToString(absoluteBassMelodicIntervals) + "\n";
    message += "absoluteTenorMelodicIntervals = " + intVarArrayToString(absoluteTenorMelodicIntervals) + "\n";
    message += "absoluteAltoMelodicIntervals = " + intVarArrayToString(absoluteAltoMelodicIntervals) + "\n";
    message += "absoluteSopranoMelodicIntervals = " + intVarArrayToString(absoluteSopranoMelodicIntervals) + "\n\n";

    message += "----------------solution-----------------\n\n";

    message += "FullChordsVoicing = " + intVarArrayToString(FullChordsVoicing) + "\n\n";
    message += "-----------------------------------------\n\n";

    message += "-------------cost variables--------------\n";

    message += "sumOfMelodicIntervals = " + intVarToString(sumOfMelodicIntervals) + "\n\n";
    message += "nDifferentValuesInChord" + intVarArrayToString(nDifferentValuesInDiminishedChord) + "\n";
    message += "nOfDiminishedChordsWith4notes = " + intVarToString(nOfDiminishedChordsWith4notes) + "\n";

    message += "\n";
    return message;
}

/***********************************************************************************************************************
 *                                                Search engine methods                                                *
 ***********************************************************************************************************************/

/**
 * Creates a search engine for the given problem
 * @param pb an instance of the FourVoiceTexture class representing a given problem
 * @param type the type of search engine to create (see enumeration in headers/gecode_problem.hpp)
 * @return a search engine for the given problem
 */
Search::Base<FourVoiceTexture>* make_solver(FourVoiceTexture* pb, int type){

    Gecode::Search::Options opts;

    if (type == BAB_SOLVER)
        return new BAB<FourVoiceTexture>(pb, opts);
    else // default case
        return new DFS<FourVoiceTexture>(pb, opts);
}

/**
 * Returns the next solution space for the problem
 * @param solver a solver for the problem
 * @return an instance of the FourVoiceTexture class representing the next solution to the problem
 */
FourVoiceTexture* get_next_solution_space(Search::Base<FourVoiceTexture>* solver){
    FourVoiceTexture* sol_space = solver->next();
    if (sol_space == nullptr) // handle the case of no solution or time out, necessary when sending the data to OM
        return nullptr;
    return sol_space;
}

/***********************************************************************************************************************
 *                                                 Auxiliary functions                                                 *
 ***********************************************************************************************************************/

/**
 * Write a text into a log file
 * Useful for debugging in the OM environment
 * @param message the text to write
 */
void writeToLogFile(const char* message){
    std::time_t currentTime = std::time(nullptr); // Get the current time
    std::string timeString = std::asctime(std::localtime(&currentTime)); // Convert to string

    const char* homeDir = std::getenv("HOME"); // Get the user's home directory
    if (homeDir) {
        std::string filePath(homeDir);
        filePath += "/log.txt"; // Specify the desired file path, such as $HOME/log.txt

        std::ofstream myfile(filePath, std::ios::app); // append mode
        if (myfile.is_open()) {
            myfile <<timeString<< endl << message << endl;
            myfile.close();
        }
    }
}