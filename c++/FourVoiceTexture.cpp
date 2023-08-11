#include <utility>

#include "headers/FourVoiceTexture.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                             FourVoiceTexture class methods                                          *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Constructor
 * @param s the number of chords in the progression
 * @param *t a pointer to a Tonality object
 * @param chordDegs the degrees of the chord of the chord progression
 * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
 */
FourVoiceTexture::FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordStas){
    /// basic data
    size = s;
    tonality = t;
    chordDegrees = chordDegs;
    chordStates = chordStas;

    /// solution array
    FullChordsVoicing = IntVarArray(*this, nOfVoices * size, 0, 127); // tonality->get_tonality_notes()

    /// variable arrays for melodic intervals for each voice
    bassMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    tenorMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    altoMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    sopranoMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);

    /// variable arrays for absolute melodic intervals for each voice
    absoluteBassMelodicIntervals = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE);
    absoluteTenorMelodicIntervals = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE);
    absoluteAltoMelodicIntervals = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE);
    absoluteSopranoMelodicIntervals = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE);

    /// variable arrays for harmonic intervals between adjacent voices (only positive because there is no direction)
    bassTenorHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    bassAltoHarmonicIntervals = IntVarArray(*this, size, 0, 2 * PERFECT_OCTAVE + PERFECT_FIFTH);
    bassSopranoHarmonicIntervals = IntVarArray(*this, size, 0, 3 * PERFECT_OCTAVE + PERFECT_FIFTH);
    tenorAltoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE);
    tenorSopranoHarmonicIntervals = IntVarArray(*this, size, 0, 2 * PERFECT_OCTAVE);
    altoSopranoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE);

    /// cost variables auxiliary arrays
    nDifferentValuesInDiminishedChord = IntVarArray(*this, size, 0, nOfVoices);
    nDifferentValuesAllChords = IntVarArray(*this, size, 0, nOfVoices);
    nOccurrencesBassInFundamentalState = IntVarArray(*this, size, 0, nOfVoices);

    /// cost variables
    sumOfMelodicIntervals = IntVar(*this, 0, PERFECT_OCTAVE * nOfVoices * (size - 1));
    nOfDiminishedChordsWith4notes = IntVar(*this, 0, size);
    nOfChordsWithLessThan4notes = IntVar(*this, 0, size);
    nOfFundamentalStateChordsWithoutDoubledBass = IntVar(*this, 0, size);

    /*******************************************************************************************************************
    *                                                                                                                  *
    *                                                   Constraints                                                    *
    *                                                                                                                  *
    ********************************************************************************************************************/

    /// link the arrays together
    link_melodic_arrays(*this, nOfVoices, size, bassMelodicIntervals, FullChordsVoicing,
                        altoMelodicIntervals, tenorMelodicIntervals, sopranoMelodicIntervals);

    link_absolute_melodic_arrays(*this, bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals,
                                 sopranoMelodicIntervals, absoluteBassMelodicIntervals, absoluteTenorMelodicIntervals,
                                 absoluteAltoMelodicIntervals, absoluteSopranoMelodicIntervals);

    link_harmonic_arrays(*this, size, nOfVoices, FullChordsVoicing, bassTenorHarmonicIntervals,
                         bassAltoHarmonicIntervals,bassSopranoHarmonicIntervals, tenorSopranoHarmonicIntervals,
                         altoSopranoHarmonicIntervals, tenorAltoHarmonicIntervals);

    /// restrain the domain of the voices to their range + state that bass <= tenor <= alto <= soprano
    restrain_voices_domains(*this, size, nOfVoices, FullChordsVoicing);

    /// forbid parallel octaves and fifths
    forbid_parallel_intervals(*this, size, nOfVoices, {PERFECT_FIFTH, PERFECT_OCTAVE},
                              bassTenorHarmonicIntervals, bassAltoHarmonicIntervals, bassSopranoHarmonicIntervals,
                              tenorAltoHarmonicIntervals, tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals,
                              FullChordsVoicing);

    /// number of diminished chords with more than 3 notes (cost to minimize)
    compute_diminished_chords_cost(*this, size, nOfVoices, tonality, chordDegrees,
                                   FullChordsVoicing,nDifferentValuesInDiminishedChord,
                                   nOfDiminishedChordsWith4notes);

    /// number of chords with less than 4 note values (cost to minimize)
    compute_n_of_notes_in_chord_cost(*this, size, nOfVoices, FullChordsVoicing,
                                     nDifferentValuesAllChords,nOfChordsWithLessThan4notes);

    /// number of fundamental state chords without doubled bass
    /// @todo maybe add suggestion to which note to double next (tonal notes)
    compute_fundamental_state_doubling_cost(*this, size, nOfVoices, tonality, chordStas, chordDegs,
                                            FullChordsVoicing,nOccurrencesBassInFundamentalState,
                                            nOfFundamentalStateChordsWithoutDoubledBass);

    /// sum of melodic intervals
    linear(*this, IntVarArgs() << absoluteTenorMelodicIntervals << absoluteAltoMelodicIntervals <<
                               absoluteSopranoMelodicIntervals << absoluteBassMelodicIntervals,
                               IRT_EQ, sumOfMelodicIntervals);

    /**-----------------------------------------------------------------------------------------------------------------
    |     Harmonic constraints: loop over each chord and post the constraints depending on the degree and state        |
    -------------------------------------------------------------------------------------------------------------------*/

    for(int i = 0; i < size; i++){
        IntVarArgs currentChord(FullChordsVoicing.slice(nOfVoices * i, 1, nOfVoices));

        /// set the chord's domain to the notes of the degree chordDegrees[i]'s chord
        set_to_chord(*this, tonality, chordDegrees[i], currentChord);

        /// set the bass based on the chord's state
        set_bass(*this, tonality, chordDegrees[i], chordStates[i], currentChord);

        /// post the constraints depending on the chord's state
        if(chordStas[i] == FUNDAMENTAL_STATE){
            /// each note should be present at least once, doubling is determined with costs
            chord_note_occurrence_fundamental_state(*this, chordDegrees[i], nOfVoices, tonality,
                                                    nDifferentValuesInDiminishedChord[i],
                                                    currentChord);
        }
        /// post the constraints specific to first inversion chords
        else if(chordStas[i] == FIRST_INVERSION){
            chord_note_occurrence_first_inversion(*this, tonality, chordDegrees[i], currentChord);
        }
        /// post the constraints specific to second inversion chords
        else if(chordStas[i] == SECOND_INVERSION){

        }
        else{
            //@todo add the other cases here (4 note chords, etc)
        }
    }

    /// @todo continue refactoring from here

    /**-----------------------------------------------------------------------------------------------------------------
    | Melodic constraints: loop over each space between chords and post the constraints depending on the state and     |`
    | quality of the chords                                                                                            |
    -------------------------------------------------------------------------------------------------------------------*/

    /// between each chord
    for(int i = 0; i < size-1; i++){

        if (chordStas[i] == FUNDAMENTAL_STATE && chordStas[i+1] == FUNDAMENTAL_STATE){
            /// keep the common note(s) in the same voice(s) if there is one. If there is not, then other voices move
            /// in contrary motion to the bass
            fundamental_state_chord_to_fundamental_state_chord(*this, i, chordDegrees,
                                                               nOfVoices, tonality,
                                                               bassMelodicIntervals,
                                                               tenorMelodicIntervals,
                                                               altoMelodicIntervals,
                                                               sopranoMelodicIntervals,
                                                               FullChordsVoicing);
        }
        else if(chordStas[i] == FIRST_INVERSION){
            /// see function def for the details, it's too long to write here
            //@todo finish this constraint
            from_first_inversion_chord(*this, i, nOfVoices, chordDegrees, tonality,
                                       bassMelodicIntervals, tenorMelodicIntervals,
                                       altoMelodicIntervals, sopranoMelodicIntervals,
                                       FullChordsVoicing);
        }
        else{ /// general guidelines (not explicit from Duha)
            /// keep the common note(s) in the same voice(s) if there is one, then move other voices to the closest note
            /// (done with preferences)
        }
    }

    /**------------------------------------------------branching-------------------------------------------------------*/
    // @todo make it smart when it becomes necessary
    branch(*this, FullChordsVoicing, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
}

/**
 * Cost function for lexicographical minimization. The order is as follows:
 * 1. number of diminished chords with more than 3 notes
 * 2. number of chords with less than 4 note values
 * 3. number of fundamental state chords without doubled bass // @todo check how we can remove diminished chords from this as they are preferred to only have 3 voices
 * 4. sum of melodic intervals minimizes the melodic movement between chords
 * @return the cost variables in order of importance
 */
IntVarArgs FourVoiceTexture::cost() const {
    return {nOfDiminishedChordsWith4notes, nOfChordsWithLessThan4notes, nOfFundamentalStateChordsWithoutDoubledBass,
            sumOfMelodicIntervals};
}

/**
 * Copy constructor
 * updates every variable and copies other arguments
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
    nDifferentValuesAllChords.update(*this, s.nDifferentValuesAllChords);
    nOccurrencesBassInFundamentalState.update(*this, s.nOccurrencesBassInFundamentalState);

    sumOfMelodicIntervals.update(*this, s.sumOfMelodicIntervals);
    nOfDiminishedChordsWith4notes.update(*this, s.nOfDiminishedChordsWith4notes);
    nOfChordsWithLessThan4notes.update(*this, s.nOfChordsWithLessThan4notes);
    nOfFundamentalStateChordsWithoutDoubledBass.update(*this, s.nOfFundamentalStateChordsWithoutDoubledBass);
}

/**
 * Returns the size of the problem
 * @return an integer representing the size of the vars array
 */
int FourVoiceTexture::get_size() const{
    return size;
}

/**
 * Copy method
 * @return a copy of the current instance of the FourVoiceTexture class. Calls the copy constructor
 */
Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
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
 * Constrain method for bab search not needed for optimization problems as it is already implemented
 * Ensures that exactly 2*b.size notes are identical to the ones in the previous solution
 * @param _b a solution to the problem from which we wish to add a constraint for the next solutions
 */
//void FourVoiceTexture::constrain(const Space& _b) {
//    const auto &b = dynamic_cast<const FourVoiceTexture &>(_b);
//}

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
string FourVoiceTexture::to_string(){
    string message;
    message += "**************************************************************\n";
    message += "*                                                            *\n";
    message += "*                           Solution                         *\n";
    message += "*                                                            *\n";
    message += "**************************************************************\n\n";
    message += "--------------------parameters--------------------\n";
    message += "size = " + std::to_string(size) + "\n";
    message += "chord degrees = " + int_vector_to_string(chordDegrees) + "\n";
    message += "chord states = " + int_vector_to_string(chordStates) + "\n";

    message += "--------------------variables---------------------\n";

    message += "BassTenorHarmonicIntervals = " + intVarArray_to_string(bassTenorHarmonicIntervals) + "\n";
    message += "TenorAltoHarmonicIntervals = " + intVarArray_to_string(tenorAltoHarmonicIntervals) + "\n";
    message += "AltoSopranoHarmonicIntervals = " + intVarArray_to_string(altoSopranoHarmonicIntervals) + "\n\n";

    message += "BassMelodicIntervals = " + intVarArray_to_string(bassMelodicIntervals) + "\n";
    message += "TenorMelodicIntervals = " + intVarArray_to_string(tenorMelodicIntervals) + "\n";
    message += "AltoMelodicIntervals = " + intVarArray_to_string(altoMelodicIntervals) + "\n";
    message += "SopranoMelodicIntervals = " + intVarArray_to_string(sopranoMelodicIntervals) + "\n\n";

    message += "absoluteBassMelodicIntervals = " + intVarArray_to_string(absoluteBassMelodicIntervals) + "\n";
    message += "absoluteTenorMelodicIntervals = " + intVarArray_to_string(absoluteTenorMelodicIntervals) + "\n";
    message += "absoluteAltoMelodicIntervals = " + intVarArray_to_string(absoluteAltoMelodicIntervals) + "\n";
    message += "absoluteSopranoMelodicIntervals = " + intVarArray_to_string(absoluteSopranoMelodicIntervals) + "\n\n";

    message += "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵\n\n";
    message += "FullChordsVoicing = " + intVarArray_to_string(FullChordsVoicing) + "\n\n";
    message += "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵\n\n";

    message += "----------cost-related auxiliary arrays----------\n";

    message += "nDifferentValuesInDiminishedChord = " + intVarArray_to_string(nDifferentValuesInDiminishedChord) + "\n";
    message += "nDifferentValuesInAllChords = " + intVarArray_to_string(nDifferentValuesAllChords) + "\n";
    message += "nOccurrencesBassInFundamentalState = " + intVarArray_to_string(nOccurrencesBassInFundamentalState) + "\n\n";

    message += "-----------------cost variables------------------\n";

    message += "nOfDiminishedChordsWith4notes = " + intVar_to_string(nOfDiminishedChordsWith4notes) + "\n";
    message += "nOfChordsWithLessThan4notes = " + intVar_to_string(nOfChordsWithLessThan4notes) + "\n";
    message += "nOfFundamentalStateChordsWithoutDoubledBass = " +
            intVar_to_string(nOfFundamentalStateChordsWithoutDoubledBass) + "\n";
    message += "sumOfMelodicIntervals = " + intVar_to_string(sumOfMelodicIntervals) + "\n\n";
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
void write_to_log_file(const char* message){
    std::time_t currentTime = std::time(nullptr); // Get the current time
    std::string timeString = std::asctime(std::localtime(&currentTime)); // Convert to string

    const char* homeDir = std::getenv("HOME"); // Get the user's home directory
    if (homeDir) {
        std::string filePath(homeDir);
        filePath += "/Users/sprockeelsd/Documents/Libraries/log.txt"; // Specify the desired file path, such as $HOME/log.txt

        std::ofstream myfile(filePath, std::ios::app); // append mode
        if (myfile.is_open()) {
            myfile <<timeString<< endl << message << endl;
            myfile.close();
        }
    }
}