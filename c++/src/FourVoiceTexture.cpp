#include "../headers/FourVoiceTexture.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                                                                                     *
 *                                             FourVoiceTexture class methods                                          *
 *                                                                                                                     *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Constructor
 * @param s the number of chords in the progression
 * @param *t a pointer to a Tonality object
 * @param chordDegs the degrees of the chord of the chord progression
 * @param chordQuals the qualities of the chord of the chord progression
 * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
 * Returns a FourVoiceTexture object
 */
FourVoiceTexture::FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordQuals,
                                   vector<int> chordStas){
    /// basic data
    size = s;
    tonality = t;
    chordDegrees = chordDegs;
    chordQualities = chordQuals;
    chordStates = chordStas;
    nOfNotesInChord = IntArgs(size);
    /// keep track of the number of notes that should be in each chord ideally
    for(int i = 0; i < size; i++)
        nOfNotesInChord[i] = chordQualitiesIntervals[chordQualities[i]].size();

    /// solution array
    FullChordsVoicing = IntVarArray(*this, nOfVoices * size, 0, 127); // tonality->get_tonality_notes()

    /// variable arrays for melodic intervals for each voice
    bassMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    tenorMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    altoMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    sopranoMelodicIntervals = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    allMelodicIntervals = IntVarArray(*this, nOfVoices* (size - 1), -PERFECT_OCTAVE, PERFECT_OCTAVE);

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
    nOFDifferentNotesInChords = IntVarArray(*this, size, 0, 5);
    commonNotesInSameVoice = IntVarArray(*this, nOfVoices, 0, size - 1);

    /// cost variables
    sumOfMelodicIntervals = IntVar(*this, 0, PERFECT_OCTAVE * nOfVoices * (size - 1));
    nOfDiminishedChordsWith4notes = IntVar(*this, 0, size);
    nOfChordsWithLessThan4notes = IntVar(*this, 0, size);
    nOfFundamentalStateChordsWithoutDoubledBass = IntVar(*this, 0, size);
    nOfCommonNotesInSoprano = IntVar(*this, 0, size);
    nOfIncompleteChords = IntVar(*this, 0, size);
    nOfCommonNotesInSameVoice = IntVar(*this, - nOfVoices * (size - 1), 0);


    /// print parameters to log file
    write_to_log_file(parameters().c_str());

    /// Test constraints

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                              link the arrays together                                            |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    link_melodic_arrays(*this, size, nOfVoices, FullChordsVoicing, bassMelodicIntervals,
                        altoMelodicIntervals, tenorMelodicIntervals, sopranoMelodicIntervals);

//    /// global array for branching (not used for now)
//    for(int i = 0; i < size-1; i++){
//        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + BASS] == bassMelodicIntervals[i]));
//        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + TENOR] == tenorMelodicIntervals[i]));
//        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + ALTO] == altoMelodicIntervals[i]));
//        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + SOPRANO] == sopranoMelodicIntervals[i]));
//    }

    link_absolute_melodic_arrays(*this, nOfVoices, bassMelodicIntervals, tenorMelodicIntervals,
                                 altoMelodicIntervals, sopranoMelodicIntervals, absoluteBassMelodicIntervals,
                                 absoluteTenorMelodicIntervals, absoluteAltoMelodicIntervals,
                                 absoluteSopranoMelodicIntervals);

    link_harmonic_arrays(*this, size, nOfVoices, FullChordsVoicing, bassTenorHarmonicIntervals,
                         bassAltoHarmonicIntervals, bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                         tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);


    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                             set up costs computation                                             |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    // @todo add a cost for doubled notes that are not tonal notes

    /// number of diminished chords in fundamental state with more than 3 notes (cost to minimize)
    compute_diminished_chords_cost(*this, size, nOfVoices, tonality, chordDegrees,
                                   chordStates, FullChordsVoicing,
                                   nDifferentValuesInDiminishedChord,
                                   nOfDiminishedChordsWith4notes);

    /// number of chords with less than 4 note values (cost to minimize)
    compute_n_of_notes_in_chord_cost(*this, size, nOfVoices, FullChordsVoicing,
                                     nDifferentValuesAllChords,nOfChordsWithLessThan4notes);

    /// number of fundamental state chords without doubled bass (cost to minimize)
    compute_fundamental_state_doubling_cost(*this, size, nOfVoices, tonality,
                                            chordDegrees, chordStates, FullChordsVoicing,
                                            nOccurrencesBassInFundamentalState,
                                            nOfFundamentalStateChordsWithoutDoubledBass);

    /// number of common notes in soprano with first inversion chord going to another chord (cost to minimize)
    compute_cost_for_common_note_in_soprano(*this, commonNotesInSameVoice, nOfCommonNotesInSoprano);

    /// sum of melodic intervals (cost to minimize)
    linear(*this, IntVarArgs() << absoluteTenorMelodicIntervals << absoluteAltoMelodicIntervals <<
                               absoluteSopranoMelodicIntervals << absoluteBassMelodicIntervals,
                               IRT_EQ, sumOfMelodicIntervals);

    /// number of chords that don't have all their possible note values (cost to minimize)
    compute_cost_for_incomplete_chords(*this, size, nOfVoices, nOfNotesInChord,
                                       FullChordsVoicing,nOFDifferentNotesInChords, nOfIncompleteChords);

    /// count the number of common notes in the same voice between consecutive chords (cost to MAXIMIZE)
    /// /!\ The variable nOfCommonNotesInSameVoice has a NEGATIVE value so the minimization will maximize its absolute value
    compute_cost_for_common_notes_not_in_same_voice(*this, absoluteBassMelodicIntervals,
                                                    absoluteTenorMelodicIntervals, absoluteAltoMelodicIntervals,
                                                    absoluteSopranoMelodicIntervals, commonNotesInSameVoice,
                                                    nOfCommonNotesInSameVoice);

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                              generic constraints                                                 |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    /// restrain the domain of the voices to their range + state that bass <= tenor <= alto <= soprano
    restrain_voices_domains(*this, size, nOfVoices, FullChordsVoicing);

    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(FullChordsVoicing.slice(nOfVoices * i, 1, nOfVoices));

        /// set the chord's domain to the notes of the degree chordDegrees[i]'s chord
        set_to_chord(*this, tonality, chordDegrees[i], chordQualities[i], currentChord);

        /// set the bass based on the chord's state
        set_bass(*this, tonality, chordDegrees[i], chordStates[i], currentChord);
    }

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |     Harmonic constraints: loop over each chord and post the constraints depending on the degree and state        |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    for(int i = 0; i < size; i++){
        IntVarArgs currentChord(FullChordsVoicing.slice(nOfVoices * i, 1, nOfVoices));

        /// post the constraints depending on the chord's state
        if(chordStas[i] == FUNDAMENTAL_STATE){
            /// each note should be present at least once, doubling is determined with costs
            chord_note_occurrence_fundamental_state(*this, nOfVoices, chordDegrees[i],
                                                    chordQualities[i],tonality, currentChord,
                                                    nDifferentValuesInDiminishedChord[i]);
        }
        /// post the constraints specific to first inversion chords
        else if(chordStas[i] == FIRST_INVERSION){
            chord_note_occurrence_first_inversion(*this, size, nOfVoices, i, tonality,
                                                  chordDegrees, chordQualities, currentChord,
                                                  bassMelodicIntervals, sopranoMelodicIntervals);
        }
        /// post the constraints specific to second inversion chords
        else if(chordStas[i] == SECOND_INVERSION){
            chord_note_occurrence_second_inversion(*this, size, nOfVoices, i, tonality,
                                                   chordDegrees, chordQualities, currentChord);
        }
        else{
        }
    }

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    | Melodic constraints: loop over each space between chords and post the constraints depending on the state and     |`
    | quality of the chords                                                                                            |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    /// between each chord
    for(int i = 0; i < size-1; i++) {
        /// parallel unissons, fifths and octaves are forbidden unless we have the same chord twice in a row
        if(chordDegrees[i] != chordDegrees[i + 1]){
            forbid_parallel_intervals(*this, size, nOfVoices, {PERFECT_FIFTH, PERFECT_OCTAVE, UNISSON},
                                      FullChordsVoicing, bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                                      bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                                      tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);
        }

        /// resolve the tritone if there is one and it needs to be resolved
        if (chordDegrees[i] == SEVENTH_DEGREE && chordQualities[i] == DIMINISHED_CHORD && chordDegrees[i + 1] == FIRST_DEGREE ||
            chordDegrees[i] == FIFTH_DEGREE && chordDegrees[i+1] == FIRST_DEGREE) {
            //@todo add other chords that have the tritone
            tritone_resolution(*this, nOfVoices, i, tonality, chordDegrees,
                               chordStates,bassMelodicIntervals,
                               tenorMelodicIntervals,altoMelodicIntervals,
                               sopranoMelodicIntervals, FullChordsVoicing);
        }

        /// Exceptions to the general voice leading rules

        /// special rule for interrupted cadence
        if (chordDegs[i] == FIFTH_DEGREE && chordStas[i] == FUNDAMENTAL_STATE &&
        chordDegs[i + 1] == SIXTH_DEGREE && chordStas[i + 1] == FUNDAMENTAL_STATE) {
            interrupted_cadence(*this, i, tonality,
                                FullChordsVoicing, tenorMelodicIntervals,
                                altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// if we have an appogiatura for the V degree chord, the voice with the fundamental must move in contrary
        /// motion to the bass
        else if(chordDegs[i] == FIRST_DEGREE && chordStates[i] == SECOND_INVERSION &&
                chordDegs[i+1] == FIFTH_DEGREE){
            fifth_degree_appogiatura(*this, nOfVoices, i, tonality, FullChordsVoicing,
                                     bassMelodicIntervals,tenorMelodicIntervals,
                                     altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// general voice leading rules
        else {
            /// If the bass moves by a step, other voices should move in contrary motion
            int bassFirstChord = (tonality->get_degree_note(chordDegrees[i] + 2 * chordStates[i])
                    % PERFECT_OCTAVE);
            int bassSecondChord = (tonality->get_degree_note(chordDegrees[i + 1] + 2 * chordStates[i + 1])
                    % PERFECT_OCTAVE);
            int bassMelodicMotion = abs(bassSecondChord - bassFirstChord);
            /// if the bass moves by a step between fund. state chords @todo check if this needs to apply in other cases
            if ((bassMelodicMotion == MINOR_SECOND || bassMelodicMotion == MAJOR_SECOND ||
                bassMelodicMotion == MINOR_SEVENTH || bassMelodicMotion == MAJOR_SEVENTH) &&
                (chordStas[i] == FUNDAMENTAL_STATE && chordStas[i+1] == FUNDAMENTAL_STATE)){
                /// move other voices in contrary motion
                contrary_motion_to_bass(*this, i,bassMelodicIntervals,
                                        tenorMelodicIntervals,altoMelodicIntervals,
                                        sopranoMelodicIntervals);
            }
            /// if II -> V, move voices in contrary motion to bass
            else if(chordDegs[i] == SECOND_DEGREE && chordDegs[i+1] == FIFTH_DEGREE){
                contrary_motion_to_bass(*this, i, bassMelodicIntervals,
                                        tenorMelodicIntervals, altoMelodicIntervals,
                                        sopranoMelodicIntervals);
            }
            else if(chordDegrees[i] != chordDegrees[i + 1]){
                /// Otherwise, keep common notes in the same voice whenever possible (cost to minimize)
            }
        }
    }

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                                       Branching                                                  |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    /// variable selection heuristic
    /// go <--
    auto meritFunction = [](const Space& home, IntVar x, int i) {
        return i;
    };

    /// value selection heuristic
    /// choose a note that is not already present in the chord
    auto branchVal = [](const Space& home, IntVar x, int i) {
        auto space = (FourVoiceTexture&) home;
        int chordPos = i / space.nOfVoices; //division entière
        return x.min();
    };
    auto branchCommit = [](Space& home, unsigned int a, IntVar x, int i, int n){
        if (a == 0U){
            rel(home, x, IRT_EQ, n);
        } else {
            rel(home, x, IRT_NQ, n);
        }
    };

    //branch(*this, FullChordsVoicing, INT_VAR_MERIT_MAX(meritFunction), INT_VAL(branchVal, branchCommit));

    /// choose the variable with the highest AFC, in case of a tie choose the one with the highest degree
    branch(*this, FullChordsVoicing, INT_VAR_DEGREE_MAX(), INT_VAL_MIN()); // choose the smallest melodic intervals possible
}

/**
 * Cost function for lexicographical minimization. The order is as follows:
 * 1. number of diminished chords with more than 3 notes
 * 2. number of chords with less than 4 note values
 * 3. number of fundamental state chords without doubled bass
 * 4. sum of melodic intervals minimizes the melodic movement between chords
 * @return the cost variables in order of importance
 */
IntVarArgs FourVoiceTexture::cost() const {
    // @todo maybe give the voices a priority + check the order depending on what is more important
    //@todo change the sum of melodic intervals to minimize the number of skips and prefer the steps
    return {nOfDiminishedChordsWith4notes, nOfChordsWithLessThan4notes, nOfFundamentalStateChordsWithoutDoubledBass,
            nOfIncompleteChords, nOfCommonNotesInSameVoice, sumOfMelodicIntervals};//nOfCommonNotesInSoprano,
}

/**
 * Copy constructor
 * @param s an instance of the FourVoiceTexture class
 * @return a copy of the given instance of the FourVoiceTexture class
 */
FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s): IntLexMinimizeSpace(s){
    size = s.size;
    tonality = s.tonality;
    chordDegrees = s.chordDegrees;
    chordQualities = s.chordQualities;
    nOfNotesInChord = s.nOfNotesInChord;
    chordStates = s.chordStates;

    bassMelodicIntervals.update(*this, s.bassMelodicIntervals);
    tenorMelodicIntervals.update(*this, s.tenorMelodicIntervals);
    altoMelodicIntervals.update(*this, s.altoMelodicIntervals);
    sopranoMelodicIntervals.update(*this, s.sopranoMelodicIntervals);

    absoluteBassMelodicIntervals.update(*this, s.absoluteBassMelodicIntervals);
    absoluteTenorMelodicIntervals.update(*this, s.absoluteTenorMelodicIntervals);
    absoluteAltoMelodicIntervals.update(*this, s.absoluteAltoMelodicIntervals);
    absoluteSopranoMelodicIntervals.update(*this, s.absoluteSopranoMelodicIntervals);
    allMelodicIntervals.update(*this, s.allMelodicIntervals);

    bassTenorHarmonicIntervals.update(*this, s.bassTenorHarmonicIntervals);
    tenorAltoHarmonicIntervals.update(*this, s.tenorAltoHarmonicIntervals);
    altoSopranoHarmonicIntervals.update(*this, s.altoSopranoHarmonicIntervals);

    FullChordsVoicing.update(*this, s.FullChordsVoicing);

    nDifferentValuesInDiminishedChord.update(*this, s.nDifferentValuesInDiminishedChord);
    nDifferentValuesAllChords.update(*this, s.nDifferentValuesAllChords);
    nOccurrencesBassInFundamentalState.update(*this, s.nOccurrencesBassInFundamentalState);
    nOFDifferentNotesInChords.update(*this, s.nOFDifferentNotesInChords);
    commonNotesInSameVoice.update(*this, s.commonNotesInSameVoice);

    sumOfMelodicIntervals.update(*this, s.sumOfMelodicIntervals);
    nOfDiminishedChordsWith4notes.update(*this, s.nOfDiminishedChordsWith4notes);
    nOfChordsWithLessThan4notes.update(*this, s.nOfChordsWithLessThan4notes);
    nOfFundamentalStateChordsWithoutDoubledBass.update(*this, s.nOfFundamentalStateChordsWithoutDoubledBass);
    nOfCommonNotesInSoprano.update(*this, s.nOfCommonNotesInSoprano);
    nOfIncompleteChords.update(*this, s.nOfIncompleteChords);
    nOfCommonNotesInSameVoice.update(*this, s.nOfCommonNotesInSameVoice);
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
int* FourVoiceTexture::return_solution() const{
    int* solution = new int[size*4];
    for(int i = 0; i < 4*size; i++){
        solution[i] = FullChordsVoicing[i].val();
    }
    return solution;
}

/**
 * Constrain method for bab search
 * not needed for optimization problems as it is already implemented
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
     * returns the parameters in a string
     * @return a string containing the parameters of the problem
     */
string FourVoiceTexture::parameters(){
    string message = "-----------------------------------------parameters-----------------------------------------\n";
    message += "Number of chords: " + std::to_string(size) + "\n";
    message += "Tonality: " + midi_to_letter(tonality->get_tonic()) + " " + mode_int_to_name(tonality->get_mode()) + "\n";
    message += "Chords: \n";
    for(int i = 0; i < size; i++){
        message += degreeNames[chordDegrees[i]] + " (" + chordQualityNames[chordQualities[i]] + ") in " +
                   stateNames[chordStates[i]];
        if(i != size - 1)
            message += ",\n";
    }
    message += ".\n Maximum number of notes in each chord: { ";
    for(int i = 0; i < size; i++){
        message += std::to_string(nOfNotesInChord[i]) + " ";
    }
    return message + "}\n";
}

/**
 * toString method
 * @return a string representation of the current instance of the FourVoiceTexture class.
 * Right now, it returns a string "FourVoiceTexture object. size = <size>"
 * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
 */
string FourVoiceTexture::to_string(){
    string message;
    message += "********************************************************************************************\n";
    message += "*                                                                                          *\n";
    message += "*                                          Solution                                        *\n";
    message += "*                                                                                          *\n";
    message += "********************************************************************************************\n\n";
    message += parameters();

    message += "\n-----------------------------------------variables------------------------------------------\n";

    message += "BassTenorHarmonicIntervals = \t" + intVarArray_to_string(bassTenorHarmonicIntervals) + "\n";
    message += "TenorAltoHarmonicIntervals = \t" + intVarArray_to_string(tenorAltoHarmonicIntervals) + "\n";
    message += "AltoSopranoHarmonicIntervals = \t" + intVarArray_to_string(altoSopranoHarmonicIntervals) + "\n\n";

    message += "BassMelodicIntervals = \t\t" + intVarArray_to_string(bassMelodicIntervals) + "\n";
    message += "TenorMelodicIntervals = \t" + intVarArray_to_string(tenorMelodicIntervals) + "\n";
    message += "AltoMelodicIntervals = \t\t" + intVarArray_to_string(altoMelodicIntervals) + "\n";
    message += "SopranoMelodicIntervals = \t" + intVarArray_to_string(sopranoMelodicIntervals) + "\n\n";

    message += "absoluteBassMelodicIntervals = \t\t" + intVarArray_to_string(absoluteBassMelodicIntervals) + "\n";
    message += "absoluteTenorMelodicIntervals = \t" + intVarArray_to_string(absoluteTenorMelodicIntervals) + "\n";
    message += "absoluteAltoMelodicIntervals = \t\t" + intVarArray_to_string(absoluteAltoMelodicIntervals) + "\n";
    message += "absoluteSopranoMelodicIntervals = \t" + intVarArray_to_string(absoluteSopranoMelodicIntervals) + "\n\n";

    message += "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵"
               "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵\n\n";
    message += "FullChordsVoicing = " + intVarArray_to_string(FullChordsVoicing) + "\n\n";
    message += "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵"
               "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵\n\n";

    message += "-------------------------------cost-related auxiliary arrays------------------------------\n";

    message += "nDifferentValuesInDiminishedChord = \t" + intVarArray_to_string(nDifferentValuesInDiminishedChord) + "\n";
    message += "nDifferentValuesInAllChords = \t\t" + intVarArray_to_string(nDifferentValuesAllChords) + "\n";
    message += "nOccurrencesBassInFundamentalState = \t" + intVarArray_to_string(nOccurrencesBassInFundamentalState) + "\n";
    message += "commonNotesInSameVoice = \t\t" + intVarArray_to_string(commonNotesInSameVoice) + "\n";
    message += "nOFDifferentNotesInChords = \t\t" + intVarArray_to_string(nOFDifferentNotesInChords) + "\n\n";

    message += "------------------------------------cost variables----------------------------------------\n";

    message += "nOfDiminishedChordsWith4notes = " + intVar_to_string(nOfDiminishedChordsWith4notes) + "\n";
    message += "nOfChordsWithLessThan4notes = " + intVar_to_string(nOfChordsWithLessThan4notes) + "\n";
    message += "nOfFundamentalStateChordsWithoutDoubledBass = " +
            intVar_to_string(nOfFundamentalStateChordsWithoutDoubledBass) + "\n";
    message += "nOfIncompleteChords = " + intVar_to_string(nOfIncompleteChords) + "\n";
    message += "nOfCommonNotesInSameVoice = " + intVar_to_string(nOfCommonNotesInSameVoice) + "\n";
    message += "sumOfMelodicIntervals = " + intVar_to_string(sumOfMelodicIntervals) + "\n";
    message += "nOfCommonNotesInSoprano = " + intVar_to_string(nOfCommonNotesInSoprano) + "\n\n";

    return message;
}