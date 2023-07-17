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
    tenorAltoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE);
    altoSopranoHarmonicIntervals = IntVarArray(*this, size, 0, PERFECT_OCTAVE);

    /** constraints */

    // link the arrays together
    link_melodic_arrays(*this, size, FullChordsVoicing, bassMelodicIntervals, tenorMelodicIntervals,
                        altoMelodicIntervals, sopranoMelodicIntervals);

    link_absolute_melodic_arrays(*this, bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals,
                                 sopranoMelodicIntervals, absoluteBassMelodicIntervals, absoluteTenorMelodicIntervals,
                                 absoluteAltoMelodicIntervals, absoluteSopranoMelodicIntervals);

    link_harmonic_arrays(*this, size, FullChordsVoicing, bassTenorHarmonicIntervals,
                         tenorAltoHarmonicIntervals, altoSopranoHarmonicIntervals);

    // restrain the domain of the voices to their range + state that bass <= tenor <= alto <= soprano
    restrain_voices_domains(*this, size, FullChordsVoicing);


    /**-------------------------------------------- generic constraints -----------------------------------------------*/

    for(int i = 0; i < size-1; i++){
        // fifths
        forbid_parallel_intervals(*this, PERFECT_FIFTH, i, BASS,
                                  bassTenorHarmonicIntervals, FullChordsVoicing); // between bass and tenor
        forbid_parallel_intervals(*this, PERFECT_FIFTH, i, TENOR,
                                  tenorAltoHarmonicIntervals, FullChordsVoicing); // between tenor and alto
        forbid_parallel_intervals(*this, PERFECT_FIFTH, i, ALTO,
                                  altoSopranoHarmonicIntervals, FullChordsVoicing); // between alto and soprano

        //octaves
        forbid_parallel_intervals(*this, PERFECT_OCTAVE, i, BASS,
                                  bassTenorHarmonicIntervals, FullChordsVoicing); // between bass and tenor
        forbid_parallel_intervals(*this, PERFECT_OCTAVE, i, TENOR,
                                  tenorAltoHarmonicIntervals, FullChordsVoicing); // between tenor and alto
        forbid_parallel_intervals(*this, PERFECT_OCTAVE, i, ALTO,
                                  altoSopranoHarmonicIntervals, FullChordsVoicing); // between alto and soprano

        //unissons
        forbid_parallel_intervals(*this, UNISSON, i, BASS,
                                  bassTenorHarmonicIntervals, FullChordsVoicing); // between bass and tenor
        forbid_parallel_intervals(*this, UNISSON, i, TENOR,
                                  tenorAltoHarmonicIntervals, FullChordsVoicing); // between tenor and alto
        forbid_parallel_intervals(*this, UNISSON, i, ALTO,
                                  altoSopranoHarmonicIntervals, FullChordsVoicing); // between alto and soprano
    }

    /**------------------------------------------chord related constraints --------------------------------------------*/
    for(int i = 0; i < size; i++){
        IntVarArgs currentChord(FullChordsVoicing.slice(4 * i, 1, 4));

        setToChord(*this, tonality, chordDegrees[i], currentChord);
        setBass(*this, tonality, chordDegrees[i], chordStates[i], currentChord);

        if(chordStas[i] == FUNDAMENTAL_STATE){
            // @todo change this to take into account c/i chords (see cst def)
            chordNoteOccurrenceFundamentalState(*this, tonality, chordDegrees[i], currentChord);
        }
        else{ // first or second inversion

        }
    }

    /**--------------------------------------------melodic constraints ------------------------------------------------*/
    for(int i = 0; i < size-1; i++){
        if (chordStas[i] == FUNDAMENTAL_STATE){
            if (chordStas[i+1] == FUNDAMENTAL_STATE){
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
    // this order because the inner voices are supposed to move the least, bass and soprano is less important
    // @todo figure out the branching, the variables below make search slower and might not be necessary
//    branch(*this, absoluteTenorMelodicIntervals, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
//    branch(*this, absoluteAltoMelodicIntervals, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
//    branch(*this, absoluteSopranoMelodicIntervals, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
//    branch(*this, absoluteBassMelodicIntervals, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
    branch(*this, FullChordsVoicing, INT_VAR_DEGREE_MAX(), INT_VAL_MIN());
}

/**
 * Copy constructor
 * @param s an instance of the FourVoiceTexture class
 */
FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s): Space(s){
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
void FourVoiceTexture::constrain(const Space& _b) {
    const auto &b = dynamic_cast<const FourVoiceTexture &>(_b);

    IntVar nOfIdenticalNotes(*this, 0, 4*b.size);

    vector<int> prevSol;
    prevSol.reserve(4*b.size); // preallocate memory

    for(int i = 0; i < 4*b.size; i++){// copy the previous solution in an int vector
        prevSol.push_back(b.FullChordsVoicing[i].val());
    }

    IntArgs previousSolution(prevSol); // cast vector to IntArgs

    // number of identical notes <= half of the notes @todo change that probably
    count(*this, FullChordsVoicing, previousSolution, IRT_EQ, nOfIdenticalNotes);
    rel(*this, nOfIdenticalNotes, IRT_LE, 2*b.size);
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
    string message = "FourVoiceTexture object. \n";
    message += "size = " + to_string(size) + "\n.";
    message += "chord degrees = " + int_vector_to_string(chordDegrees) + "\n";
    message += "chord states = " + int_vector_to_string(chordStates) + "\n";
    message += "FullChordsVoicing = {";
    for(int i = 0; i < 4*size; i++){
        if(FullChordsVoicing[i].assigned())
            message += to_string(FullChordsVoicing[i].val());
        else
            message += "<not assigned>";
        if(i != 4*size - 1)
            message += ", ";
    }
    message += "}\n";
    message += "BassTenorHarmonicIntervals = {";
    for(int i = 0; i < size; i++){
        if(bassTenorHarmonicIntervals[i].assigned()){
            message += to_string(bassTenorHarmonicIntervals[i].val());
        }
        else
            message += "<not assigned>";
        if(i != size - 1)
            message += ", ";
    }
    message += "}\n";
    message += "TenorAltoHarmonicIntervals = {";
    for(int i = 0; i < size; i++){
        if(tenorAltoHarmonicIntervals[i].assigned())
            message += to_string(tenorAltoHarmonicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != size - 1)
            message += ", ";
    }
    message += "}\n";
    message += "AltoSopranoHarmonicIntervals = {";
    for(int i = 0; i < size; i++){
        if(altoSopranoHarmonicIntervals[i].assigned())
            message += to_string(altoSopranoHarmonicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != size - 1)
            message += ", ";
    }
    message += "}\n";
    message += "BassMelodicIntervals = {";
    for(int i = 0; i < size-1; i++){
        if(bassMelodicIntervals[i].assigned())
            message += to_string(bassMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != size - 2)
            message += ", ";
    }
    message += "}\n";
    message += "TenorMelodicIntervals = {";
    for(int i = 0; i < size-1; i++){
        if(tenorMelodicIntervals[i].assigned())
            message += to_string(tenorMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != size - 2)
            message += ", ";
    }
    message += "}\n";
    message += "AltoMelodicIntervals = {";
    for(int i = 0; i < size-1; i++){
        if(altoMelodicIntervals[i].assigned())
            message += to_string(altoMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != size - 2)
            message += ", ";
    }
    message += "}\n";
    message += "SopranoMelodicIntervals = {";
    for(int i = 0; i < size-1; i++){
        if(sopranoMelodicIntervals[i].assigned())
            message += to_string(sopranoMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != size - 2)
            message += ", ";
    }
    message += "}\n";
    message += "absoluteBassMelodicIntervals = {";
    for(int i = 0; i < absoluteBassMelodicIntervals.size(); i++){
        if(absoluteBassMelodicIntervals[i].assigned())
            message += to_string(absoluteBassMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != absoluteBassMelodicIntervals.size() - 1)
            message += ", ";
    }
    message += "}\n";
    message += "absoluteTenorMelodicIntervals = {";
    for(int i = 0; i < absoluteTenorMelodicIntervals.size(); i++){
        if(absoluteTenorMelodicIntervals[i].assigned())
            message += to_string(absoluteTenorMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != absoluteTenorMelodicIntervals.size() - 1)
            message += ", ";
    }
    message += "}\n";
    message += "absoluteAltoMelodicIntervals = {";
    for(int i = 0; i < absoluteAltoMelodicIntervals.size(); i++){
        if(absoluteAltoMelodicIntervals[i].assigned())
            message += to_string(absoluteAltoMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != absoluteAltoMelodicIntervals.size() - 1)
            message += ", ";
    }
    message += "}\n";
    message += "absoluteSopranoMelodicIntervals = {";
    for(int i = 0; i < absoluteSopranoMelodicIntervals.size(); i++){
        if(absoluteSopranoMelodicIntervals[i].assigned())
            message += to_string(absoluteSopranoMelodicIntervals[i].val());
        else
            message += "<not assigned>";
        if(i != absoluteSopranoMelodicIntervals.size() - 1)
            message += ", ";
    }
    message += "}\n";
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