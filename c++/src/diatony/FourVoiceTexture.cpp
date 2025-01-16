// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A class that creates a constraint representation of a four voice harmonic problem of tonal music.
//
#include <utility>

#include "../../headers/diatony/FourVoiceTexture.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                                                                                     *
 *                                             FourVoiceTexture class methods                                          *
 *                                                                                                                     *
 *                                                                                                                     *
 ***********************************************************************************************************************/
/**
 * This class models a classic 4 voice harmonic problem of tonal music. It takes as arguments a number of chords, a
 * tonality, and a series of chords identified by their degree, quality and state. It then generates a 4 voice chord
 * progression following traditional rules of western tonal harmony.
 */

/**
 * Constructor
 * @param s the number of chords in the progression
 * @param t a pointer to a Tonality object
 * @param chordDegs the degrees of the chord of the chord progression
 * @param chordQuals the qualities of the chord of the chord progression
 * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
 * @return an instance of FourVoiceTexture initialized with the given parameters, constraints and branching strategies
 * posted.
 * /!\ dominant diminished seventh chords are considered as minor ninth dominant chords without their fundamental
 */
FourVoiceTexture::FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas) {
    //todo: support seventh chords for all chord degrees + diminished seventh chords
    /// Parameters
    size                                            = s;
    tonality                                        = t;
    chordDegrees                                    = chordDegs;
    chordQualities                                  = std::move(chordQuals);
    chordStates                                     = chordStas;
    nOfNotesInChord                                 = IntArgs(size);
    /// keep track of the number of notes that should be in each chord if it is complete
    for(int i = 0; i < size; i++)
        nOfNotesInChord[i] = chordQualitiesIntervals.at(chordQualities[i]).size() + 1;

    /// solution array
    fullChordsVoicing                               = IntVarArray(*this, nOfVoices * size, 0, 127);

    /// variable arrays for melodic intervals for each voice
    bassMelodicIntervals                            = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    tenorMelodicIntervals                           = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    altoMelodicIntervals                            = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    sopranoMelodicIntervals                         = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);

    allMelodicIntervals                             = IntVarArray(*this, nOfVoices* (size - 1), -PERFECT_OCTAVE, PERFECT_OCTAVE);

    /// variable arrays for harmonic intervals between adjacent voices (only positive because there is no direction)
    bassTenorHarmonicIntervals                      = IntVarArray(*this, size, 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    bassAltoHarmonicIntervals                       = IntVarArray(*this, size, 0, 2 * PERFECT_OCTAVE + PERFECT_FIFTH);
    bassSopranoHarmonicIntervals                    = IntVarArray(*this, size, 0, 3 * PERFECT_OCTAVE + PERFECT_FIFTH);
    tenorAltoHarmonicIntervals                      = IntVarArray(*this, size, 0, PERFECT_OCTAVE);
    tenorSopranoHarmonicIntervals                   = IntVarArray(*this, size, 0, 2 * PERFECT_OCTAVE);
    altoSopranoHarmonicIntervals                    = IntVarArray(*this, size, 0, PERFECT_OCTAVE);

    /// cost variables auxiliary arrays
    nDifferentValuesInDiminishedChord               = IntVarArray(*this, size, 0, nOfVoices);
    nDifferentValuesAllChords                       = IntVarArray(*this, size, 0, nOfVoices);
    nOFDifferentNotesInChords                       = IntVarArray(*this, size, 0, nOfVoices);
    commonNotesInSameVoice                          = IntVarArray(*this, nOfVoices, 0, size - 1);
    costsAllMelodicIntervals                        = IntVarArray(*this, nOfVoices * (size - 1), UNISON_COST, MAX_MELODIC_COST);

    nOfUnissons                                     = IntVar(*this, 0, nOfVoices * (size - 1));

    /// cost variables
    nOfFundStateDiminishedChordsWith4notes          = IntVar(*this, 0, size);
    nOfChordsWithLessThan4Values                    = IntVar(*this, 0, size);
    nOfIncompleteChords                             = IntVar(*this, 0, size);
    nOfCommonNotesInSameVoice                       = IntVar(*this, - nOfVoices * (size - 1), 0);
    costOfMelodicIntervals                          = IntVar(*this, 0, nOfVoices*(size-1)* MAX_MELODIC_COST);

    costVector = {nOfIncompleteChords, nOfFundStateDiminishedChordsWith4notes, nOfChordsWithLessThan4Values,
                  costOfMelodicIntervals, nOfCommonNotesInSameVoice};

    /// Test constraints

    //rel(*this, fullChordsVoicing[nOfVoices * 4 + 1] == 64);

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                              link the arrays together                                            |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    link_melodic_arrays(*this, nOfVoices, size, fullChordsVoicing, bassMelodicIntervals,
                        altoMelodicIntervals, tenorMelodicIntervals, sopranoMelodicIntervals);

    /// global arrays
    for(int i = 0; i < size-1; i++){
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + BASS]            == bassMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + TENOR]           == tenorMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + ALTO]            == altoMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + SOPRANO]         == sopranoMelodicIntervals[i]));
    }

    link_harmonic_arrays(*this, nOfVoices, size, fullChordsVoicing,
                         bassTenorHarmonicIntervals, bassAltoHarmonicIntervals, bassSopranoHarmonicIntervals,
                         tenorAltoHarmonicIntervals, tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                             set up costs computation                                             |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    // @todo add a cost for doubled notes that are not tonal notes -> if a value is not in the tonal notes (1-2-4-5), then its occurence cannot be greater than 1 for each chord

    /// number of diminished chords in fundamental state with more than 3 notes (cost to minimize)
    compute_diminished_chords_cost(*this, nOfVoices, size,
                                   chordStates, chordQualities, fullChordsVoicing, nDifferentValuesInDiminishedChord,
                                   nOfFundStateDiminishedChordsWith4notes);

    /// number of chords with less than 4 note values (cost to minimize)
    compute_n_of_notes_in_chord_cost(*this, nOfVoices, size, fullChordsVoicing,
                                     nDifferentValuesAllChords, nOfChordsWithLessThan4Values);

    /// number of chords that don't have all their possible note values (cost to minimize)
    compute_cost_for_incomplete_chords(*this, nOfVoices, size, nOfNotesInChord,
                                       fullChordsVoicing, nOFDifferentNotesInChords, nOfIncompleteChords);

    /// count the number of common notes in the same voice between consecutive chords (cost to MAXIMIZE)
    /// /!\ The variable nOfCommonNotesInSameVoice has a NEGATIVE value so the minimization will maximize its absolute value
    compute_cost_for_common_notes_not_in_same_voice(*this, bassMelodicIntervals, tenorMelodicIntervals,
                                                    altoMelodicIntervals, sopranoMelodicIntervals, nOfUnissons,
                                                    commonNotesInSameVoice,
                                                    nOfCommonNotesInSameVoice);

    /// weighted sum of melodic intervals (cost to minimize)
    compute_cost_for_melodic_intervals(*this, allMelodicIntervals, nOfUnissons,
                                       costOfMelodicIntervals, costsAllMelodicIntervals);

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                              generic constraints                                                 |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    ///todo check the constraint for Vda again with the new representation where it is its own chord!
    ///todo check that all constraints still work as expected with the new additions

    /// restrain the domain of the voices to their range + state that bass <= tenor <= alto <= soprano
    restrain_voices_domains(*this, nOfVoices, size,
                            {BASS_MIN, TENOR_MIN, ALTO_MIN, SOPRANO_MIN},
                            {BASS_MAX, TENOR_MAX, ALTO_MAX, SOPRANO_MAX},
                            fullChordsVoicing);

    for(int i = 0; i < size; i++) {
        IntVarArgs currentChord(fullChordsVoicing.slice(nOfVoices * i, 1, nOfVoices));

        /// set the chord's domain to the notes of the degree chordDegrees[i]'s chord with the right quality
        set_to_chord(*this, tonality, chordDegrees[i], chordQualities[i], currentChord);

        /// set the bass based on the chord's state
        set_bass(*this, tonality, chordDegrees[i], chordQualities[i], chordStates[i], currentChord);
    }

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |     Harmonic constraints: loop over each chord and post the constraints depending on the degree and state        |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    for(int i = 0; i < size; i++){
        IntVarArgs currentChord(fullChordsVoicing.slice(nOfVoices * i, 1, nOfVoices));

        /// post the constraints depending on the chord's state
        if(chordStas[i] == FUNDAMENTAL_STATE){
            /// each note should be present at least once, doubling is determined with costs
            chord_note_occurrence_fundamental_state(*this, nOfVoices, i, chordDegrees,
                                                    chordQualities, tonality, currentChord,
                                                    nDifferentValuesInDiminishedChord[i],
                                                    nOFDifferentNotesInChords[i % nOfVoices]);
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
        else if (chordStas[i] == THIRD_INVERSION){
            chord_note_occurrence_third_inversion(*this, size, nOfVoices, i, tonality,
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
            /// @todo maybe do it also <--- so that it can propagate in both directions, if the harmonic interval is a perfect fifth or octave the previous and next chords can't
            //todo maybe rewrite this
            forbid_parallel_intervals(*this, size, nOfVoices, {PERFECT_FIFTH, PERFECT_OCTAVE, UNISSON},
                                      fullChordsVoicing, bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                                      bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                                      tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);
        }

        /// resolve the tritone if there is one and it needs to be resolved
        if ((chordDegrees[i] == SEVENTH_DEGREE && chordQualities[i] == DIMINISHED_CHORD && chordDegrees[i + 1] == FIRST_DEGREE) ||
            (chordDegrees[i] == FIFTH_DEGREE && chordDegrees[i+1] == FIRST_DEGREE) ||
            ((chordDegrees[i] >= FIVE_OF_TWO && chordDegrees[i] <= FIVE_OF_SEVEN) && chordDegrees[i+1] != FIFTH_DEGREE_APPOGIATURA)){
            //@todo add other chords that have the tritone
            tritone_resolution(*this, nOfVoices, i, tonality, chordDegrees,
                               chordQualities, chordStates, bassMelodicIntervals,
                               tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals, fullChordsVoicing);
        }

        /// Exceptions to the general voice leading rules

        /// special rule for interrupted cadence
        if (chordDegrees[i] == FIFTH_DEGREE && chordStates[i] == FUNDAMENTAL_STATE &&
        chordDegrees[i + 1] == SIXTH_DEGREE && chordStates[i + 1] == FUNDAMENTAL_STATE) {
            interrupted_cadence(*this, i, tonality,
                                fullChordsVoicing, tenorMelodicIntervals,
                                altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// special rules for augmented sixth chords
        else if (chordDegrees[i] == AUGMENTED_SIXTH) {
            italian_augmented_sixth(*this, nOfVoices, i, tonality, fullChordsVoicing,
                                    bassMelodicIntervals, tenorMelodicIntervals,
                                    altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// if we have an appogiatura for the V degree chord, the voice with the fundamental must move in contrary
        /// motion to the bass
        else if(chordDegs[i] == FIRST_DEGREE && chordStates[i] == SECOND_INVERSION &&
                chordDegs[i+1] == FIFTH_DEGREE && (chordQualities[i] == MAJOR_CHORD || chordQualities[i] == DOMINANT_SEVENTH_CHORD)){
            fifth_degree_appogiatura(*this, nOfVoices, i, tonality, fullChordsVoicing,
                                     bassMelodicIntervals, tenorMelodicIntervals,
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

    /// go <-- soprano->bass: 4-3-2-1-8-7-6-5 etc
    auto r_to_l = [](const Space& home, const IntVar& x, int i) {
        return i;
    };

    branch(*this, fullChordsVoicing, INT_VAR_MERIT_MAX(r_to_l), INT_VAL_RND(1U));
}

/**
 * Constructor to find all optimal solutions (with or without margin) based on the cost vector for one of the best ones.
 * @param s the number of chords in the progression
 * @param *t a pointer to a Tonality object
 * @param chordDegs the degrees of the chord of the chord progression
 * @param chordQuals the qualities of the chord of the chord progression
 * @param chordStas the states of the chord of the chord progression (fundamental, 1st inversion,...)
 * @param costs the cost vector for one of the best solutions, found by solving the optimization problem first
 * @param margin the offset percentage to add to the melodic cost vector to find close to optimal solutions
 */
FourVoiceTexture::FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordQuals,
                                   vector<int> chordStas, vector<int> costs, double margin) :
                                   FourVoiceTexture(s, t,chordDegs, chordQuals, chordStas) {
    double lb_factor = 1.0 - margin;
    double ub_factor = 1.0 + margin;
    std::cout << lb_factor << "   " << ub_factor << std::endl;
    ///todo c'est moche mais comme le dernier cout est négatif faut faire l'inverse sinon bah il y a pas de solutions
    for(int i = 0; i < costs.size()-1; i++){
        std::cout << "lower bound = " << floor(costs[i] * lb_factor) << " upper bound = " << ceil(costs[i] * ub_factor) << std::endl;
        rel(*this, floor(costs[i] * lb_factor) <= costVector[i]);
        rel(*this, costVector[i] <= ceil(costs[i] * ub_factor));
    }
    //todo rendre ça beau plz
    std::cout << "lower bound = " << floor(costs[costs.size()-1] * ub_factor) << " upper bound = " << ceil(costs[costs.size()-1] * lb_factor) << std::endl;
    rel(*this, costVector[costs.size()-1] <= ceil(costs[costs.size()-1] * lb_factor));
}

/**
 * Cost function for lexicographical minimization. The order is as follows:
 * 1. Number of incomplete chords.
 * 2. Number of diminished chords in fundamental state with 4 notes.
 * 3. Number of chords with less than 4 note values.
 * 4. Number of fundamental state chords without doubled bass.
 * 5. Weighted sum of melodic intervals.
 * 6. Number of common notes in the same voice between consecutive chords.
 * @return the cost variables in order of importance
 */
IntVarArgs FourVoiceTexture::cost() const {
    return costVector;
}

/**
 * Copy constructor
 * @param s an instance of the FourVoiceTexture class
 * @return a copy of the given instance of the FourVoiceTexture class
 * /!\ It is important to copy every variable instance variable of the given instance to the new instance
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

    bassTenorHarmonicIntervals.update(*this, s.bassTenorHarmonicIntervals);
    tenorAltoHarmonicIntervals.update(*this, s.tenorAltoHarmonicIntervals);
    altoSopranoHarmonicIntervals.update(*this, s.altoSopranoHarmonicIntervals);

    fullChordsVoicing.update(*this, s.fullChordsVoicing);

    nDifferentValuesInDiminishedChord.update(*this, s.nDifferentValuesInDiminishedChord);
    nDifferentValuesAllChords.update(*this, s.nDifferentValuesAllChords);
    nOFDifferentNotesInChords.update(*this, s.nOFDifferentNotesInChords);
    commonNotesInSameVoice.update(*this, s.commonNotesInSameVoice);
    costsAllMelodicIntervals.update(*this, s.costsAllMelodicIntervals);

    nOfUnissons.update( *this, s.nOfUnissons);

    nOfFundStateDiminishedChordsWith4notes.update(*this, s.nOfFundStateDiminishedChordsWith4notes);
    nOfChordsWithLessThan4Values.update(*this, s.nOfChordsWithLessThan4Values);
    nOfIncompleteChords.update(*this, s.nOfIncompleteChords);
    nOfCommonNotesInSameVoice.update(*this, s.nOfCommonNotesInSameVoice);
    costOfMelodicIntervals.update(*this, s.costOfMelodicIntervals);

    costVector = {nOfIncompleteChords, nOfFundStateDiminishedChordsWith4notes, nOfChordsWithLessThan4Values,
                  costOfMelodicIntervals, nOfCommonNotesInSameVoice};
}

/**
 * Returns the number of chords of the problem
 * @return an integer representing the number of chords of the problem
 */
int FourVoiceTexture::get_size() const{ return size; }

/**
 * Copy method
 * @return a copy of the current instance of the FourVoiceTexture class. Calls the copy constructor
 */
Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
}

/**
 * Returns the values taken by the variables vars in a solution as a pointer to an integer array
 * @return an array of integers representing the values of the variables in a solution
 */
int* FourVoiceTexture::return_solution() const{
    int* solution = new int[size*4];
    for(int i = 0; i < 4*size; i++){
        solution[i] = fullChordsVoicing[i].val();
    }
    return solution;
}

/**
 * Returns the cost variables in lexicographical order
 * @return an IntVarArgs containing cost variables in lexicographical order ONCE A SOLUTION IS FOUND
 */
IntVarArgs FourVoiceTexture::get_cost_vector() const{
    return costVector;
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
    message += "AllMelodicIntervals = \t\t" + intVarArray_to_string(allMelodicIntervals) + "\n\n";

    message += "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵"
               "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵\n\n";
    message += "fullChordsVoicing = " + intVarArray_to_string(fullChordsVoicing) + "\n\n";
    message += "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵"
               "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵\n\n";

    message += "-------------------------------cost-related auxiliary arrays------------------------------\n";

    message += "nDifferentValuesInDiminishedChord = \t" + intVarArray_to_string(nDifferentValuesInDiminishedChord) + "\n";
    message += "nDifferentValuesInAllChords = \t\t" + intVarArray_to_string(nDifferentValuesAllChords) + "\n";
    message += "commonNotesInSameVoice = \t\t" + intVarArray_to_string(commonNotesInSameVoice) + "\n";
    message += "nOFDifferentNotesInChords = \t\t" + intVarArray_to_string(nOFDifferentNotesInChords) + "\n";
    message += "costsAllMelodicIntervals = \t\t" + intVarArray_to_string(costsAllMelodicIntervals) + "\n\n";

    message += "nOfUnissons = \t\t" + intVar_to_string(nOfUnissons) + "\n\n";

    message += "------------------------------------cost variables----------------------------------------\n";

    message += "nOfFundStateDiminishedChordsWith4notes = " + intVar_to_string(nOfFundStateDiminishedChordsWith4notes) + "\n";
    message += "nOfChordsWithLessThan4Values = " + intVar_to_string(nOfChordsWithLessThan4Values) + "\n";
    message += "nOfIncompleteChords = " + intVar_to_string(nOfIncompleteChords) + "\n";
    message += "nOfCommonNotesInSameVoice = " + intVar_to_string(nOfCommonNotesInSameVoice,true) + "\n";
    message += "nOfCommonNotesTenor = " + intVar_to_string(commonNotesInSameVoice[TENOR]) + "\n";
    message += "nOfCommonNotesAlto = " + intVar_to_string(commonNotesInSameVoice[ALTO]) + "\n";
    message += "nOfCommonNotesSoprano = " + intVar_to_string(commonNotesInSameVoice[SOPRANO]) + "\n";
    message += "nOfCommonNotesBass = " + intVar_to_string(commonNotesInSameVoice[BASS]) + "\n";
    message += "costOfMelodicIntervals = " + intVar_to_string(costOfMelodicIntervals) + "\n\n";

    message += "Cost vector = {" + intVarArgs_to_string(costVector) + "}\n\n";

    return message;
}