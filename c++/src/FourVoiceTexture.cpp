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
                                   vector<int> chordStas, int variableSelectionStrategy,
                                   int valueSelectionStrategy) {
    /// basic data
    size                        = s;
    tonality                    = t;
    chordDegrees                = chordDegs;
    chordQualities              = chordQuals;
    chordStates                 = chordStas;
    nOfNotesInChord             = IntArgs(size);
    variableBranchingStrategy   = variableSelectionStrategy;
    valueBranchingStrategy      = valueSelectionStrategy;

    /// keep track of the number of notes that should be in each chord ideally
    for(int i = 0; i < size; i++)
        nOfNotesInChord[i] = chordQualitiesIntervals[chordQualities[i]].size();

    /// solution array
    FullChordsVoicing = IntVarArray(*this, nOfVoices * size, 0, 127); // tonality->get_tonality_notes()

    /// variable arrays for melodic intervals for each voice
    bassMelodicIntervals        = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    tenorMelodicIntervals       = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    altoMelodicIntervals        = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    sopranoMelodicIntervals     = IntVarArray(*this, size - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);

    allMelodicIntervals         = IntVarArray(*this, nOfVoices* (size - 1), -PERFECT_OCTAVE, PERFECT_OCTAVE);
    allSquaredMelodicIntervals  = IntVarArray(*this, nOfVoices * (size - 1), 0, PERFECT_OCTAVE * PERFECT_OCTAVE);

    /// variable arrays for absolute melodic intervals for each voice
    squaredBassMelodicIntervals    = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE * PERFECT_OCTAVE);
    squaredTenorMelodicIntervals   = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE * PERFECT_OCTAVE);
    squaredAltoMelodicIntervals    = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE * PERFECT_OCTAVE);
    squaredSopranoMelodicIntervals = IntVarArray(*this, size - 1, 0, PERFECT_OCTAVE * PERFECT_OCTAVE);

    /// variable arrays for harmonic intervals between adjacent voices (only positive because there is no direction)
    bassTenorHarmonicIntervals      = IntVarArray(*this, size, 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    bassAltoHarmonicIntervals       = IntVarArray(*this, size, 0, 2 * PERFECT_OCTAVE + PERFECT_FIFTH);
    bassSopranoHarmonicIntervals    = IntVarArray(*this, size, 0, 3 * PERFECT_OCTAVE + PERFECT_FIFTH);
    tenorAltoHarmonicIntervals      = IntVarArray(*this, size, 0, PERFECT_OCTAVE);
    tenorSopranoHarmonicIntervals   = IntVarArray(*this, size, 0, 2 * PERFECT_OCTAVE);
    altoSopranoHarmonicIntervals    = IntVarArray(*this, size, 0, PERFECT_OCTAVE);

    /// cost variables auxiliary arrays
    nDifferentValuesInDiminishedChord   = IntVarArray(*this, size, 0, nOfVoices);
    nDifferentValuesAllChords           = IntVarArray(*this, size, 0, nOfVoices);
    nOccurrencesBassInFundamentalState  = IntVarArray(*this, size, 0, nOfVoices);
    nOFDifferentNotesInChords           = IntVarArray(*this, size, 0, nOfVoices);
    commonNotesInSameVoice              = IntVarArray(*this, nOfVoices, 0, size - 1);
    negativeCommonNotesInSameVoice      = IntVarArray(*this, nOfVoices, -(size - 1), 0);

    nOfSeconds  = IntVar(*this, 0, nOfVoices * (size - 1));
    nOfThirds   = IntVar(*this, 0, nOfVoices * (size - 1));
    nOfFourths  = IntVar(*this, 0, nOfVoices * (size - 1));
    nOfFifths   = IntVar(*this, 0, nOfVoices * (size - 1));
    nOfSixths   = IntVar(*this, 0, nOfVoices * (size - 1));
    nOfSevenths = IntVar(*this, 0, nOfVoices * (size - 1));
    nOfOctaves  = IntVar(*this, 0, nOfVoices * (size - 1));

    /// cost variables
    nOfDiminishedChordsWith4notes               = IntVar(*this, 0, size);
    nOfChordsWithLessThan4notes                 = IntVar(*this, 0, size);
    nOfFundamentalStateChordsWithoutDoubledBass = IntVar(*this, 0, size);
    nOfIncompleteChords                         = IntVar(*this, 0, size);
    nOfCommonNotesInSameVoice                   = IntVar(*this, - nOfVoices * (size - 1), 0);
    costOfMelodicIntervals                      = IntVar(*this, 0, nOfVoices*(size-1)* MAX_MELODIC_COST);

    costVector = {nOfDiminishedChordsWith4notes, nOfChordsWithLessThan4notes, nOfFundamentalStateChordsWithoutDoubledBass,
                  nOfIncompleteChords, negativeCommonNotesInSameVoice[TENOR], negativeCommonNotesInSameVoice[ALTO],
                  commonNotesInSameVoice[SOPRANO], costOfMelodicIntervals};

    /// Test constraints
    /// example 1
//    rel(*this, bassMelodicIntervals, IRT_EQ, IntArgs({-1, -2, -2, -2, -1, -2, 5, 5, -1, -2, -2, -2, 2, -7}));
//
//    rel(*this, FullChordsVoicing[0* nOfVoices + SOPRANO]            == 68);
//    rel(*this, FullChordsVoicing[(size-1) * nOfVoices + SOPRANO]    == 68);

    /// example 2 verse
//    rel(*this, FullChordsVoicing, IRT_LQ, 79);
    /// example 2 chorus
//    rel(*this, FullChordsVoicing, IRT_LQ, 68);
//    rel(*this, FullChordsVoicing[3*nOfVoices + SOPRANO], IRT_GQ, 68);
    //rel(*this, FullChordsVoicing[4*nOfVoices + SOPRANO], IRT_GQ, 67);
//    rel(*this, sopranoMelodicIntervals[2] == -1);
    //rel(*this, sopranoMelodicIntervals[3] == -2);

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                              link the arrays together                                            |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    link_melodic_arrays(*this, size, nOfVoices, FullChordsVoicing, bassMelodicIntervals,
                        altoMelodicIntervals, tenorMelodicIntervals, sopranoMelodicIntervals);

    /// global arrays for branching
    for(int i = 0; i < size-1; i++){
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + BASS]    == bassMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + TENOR]   == tenorMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + ALTO]    == altoMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nOfVoices * i + SOPRANO] == sopranoMelodicIntervals[i]));

        rel(*this, expr(*this, allSquaredMelodicIntervals[nOfVoices * i + BASS] == squaredBassMelodicIntervals[i]));
        rel(*this, expr(*this, allSquaredMelodicIntervals[nOfVoices * i + TENOR] == squaredTenorMelodicIntervals[i]));
        rel(*this, expr(*this, allSquaredMelodicIntervals[nOfVoices * i + ALTO] == squaredAltoMelodicIntervals[i]));
        rel(*this, expr(*this, allSquaredMelodicIntervals[nOfVoices * i + SOPRANO] == squaredSopranoMelodicIntervals[i]));
    }

    link_squared_melodic_arrays(*this, bassMelodicIntervals, tenorMelodicIntervals,
                                altoMelodicIntervals, sopranoMelodicIntervals, squaredBassMelodicIntervals,
                                squaredTenorMelodicIntervals, squaredAltoMelodicIntervals,
                                squaredSopranoMelodicIntervals);

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

    /// number of chords that don't have all their possible note values (cost to minimize)
    compute_cost_for_incomplete_chords(*this, size, nOfVoices, nOfNotesInChord,
                                       FullChordsVoicing,nOFDifferentNotesInChords, nOfIncompleteChords);

    /// count the number of common notes in the same voice between consecutive chords (cost to MAXIMIZE)
    /// /!\ The variable nOfCommonNotesInSameVoice has a NEGATIVE value so the minimization will maximize its absolute value
    compute_cost_for_common_notes_not_in_same_voice(*this, bassMelodicIntervals, tenorMelodicIntervals,
                                                    altoMelodicIntervals, sopranoMelodicIntervals,
                                                    commonNotesInSameVoice, negativeCommonNotesInSameVoice,
                                                    nOfCommonNotesInSameVoice);

    /// weighted sum of melodic intervals (cost to minimize)
    compute_cost_for_melodic_intervals(*this, bassMelodicIntervals, tenorMelodicIntervals,
                                       altoMelodicIntervals, sopranoMelodicIntervals, nOfSeconds,
                                       nOfThirds, nOfFourths, nOfFifths, nOfSixths, nOfSevenths, nOfOctaves,
                                       costOfMelodicIntervals);

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
                                                    chordQualities[i], tonality, currentChord,
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
                               chordQualities, chordStates, bassMelodicIntervals,
                               tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals, FullChordsVoicing);
        }

        /// Exceptions to the general voice leading rules

        /// special rule for interrupted cadence
        if (chordDegs[i] == FIFTH_DEGREE && chordStas[i] == FUNDAMENTAL_STATE &&
        chordDegs[i + 1] == SIXTH_DEGREE && chordStas[i + 1] == FUNDAMENTAL_STATE) {
            interrupted_cadence(*this, nOfVoices, i, tonality,
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
    //branch(*this, costVector, INT_VAR_NONE(), INT_VAL_MIN()); /// in their order because they are sorted by importance

    branch(*this, FullChordsVoicing, variable_selection_heuristics[variableBranchingStrategy],
           value_selection_heuristics[valueBranchingStrategy]);
}

//void FourVoiceTexture::first(void){//    /// start by finding the last chord
////    branch(*this, FullChordsVoicing.slice(nOfVoices * (size - 1), 1, nOfVoices),
////           variable_selection_heuristics[RIGHT_TO_LEFT],
////           INT_VAL_RND(1U));
////    /// go right to left branching on melodic intervals, choosing the smallest one in absolute value
////
////    /// value selection heuristic
////
////
////    //@todo branch on cost values that are the most important
////
////    branch(*this, allSquaredMelodicIntervals, variable_selection_heuristics[variableSelectionStrategy],
////           value_selection_heuristics[valueSelectionStrategy]); //@todo change
////
////    branch(*this, allMelodicIntervals, variable_selection_heuristics[variableSelectionStrategy],
////           value_selection_heuristics[valueSelectionStrategy]); //@todo change
//
//
//    branch(*this, FullChordsVoicing, variable_selection_heuristics[variableBranchingStrategy],
//           value_selection_heuristics[valueBranchingStrategy]);
//}
//
//void FourVoiceTexture::next(const FourVoiceTexture& s) {
//    this->constrain(s);
//    //relax(*this, FullChordsVoicing, s.FullChordsVoicing, Rnd(1U), 0.7); /// relax 70% of the variables based on RNG with seed 1
//    /// todo post additional constraints for LNS
//}
//
//bool FourVoiceTexture::slave(const MetaInfo& mi){
//    if(mi.type() == MetaInfo::RESTART && mi.restart() > 0){
//        next(static_cast<const FourVoiceTexture&>(*mi.last()));
//        return false; /// search is incomplete
//    }
//    else{
//        return true;
//    }
//}

/**
 * Cost function for lexicographical minimization. The order is as follows:
 * 1. number of diminished chords with more than 3 notes. @todo maybe efficiency can improve if I add a cst for this
 * 2. number of chords with less than 4 note values.
 * 3. number of fundamental state chords without doubled bass.
 * 4. Number of incomplete chords. They should only occur when necessary.
 * 5. Number of common notes in the tenor. This cost is negative because we want to maximize it
 * 6. Number of common notes in the alto. This cost is negative because we want to maximize it
 * 7. Number of common notes in the soprano. This cost is positive because we want to avoid it. We don't do the same for
 *      the bass because we want to allow it to jump octaves if it leads to better solutions.
 * 8. sum of melodic intervals minimizes the melodic movement between chords. @todo change it to give different costs to different intervals
 * @return the cost variables in order of importance
 */
IntVarArgs FourVoiceTexture::cost() const {
    //@todo change the sum of melodic intervals to minimize the number of skips and prefer the steps
    return {nOfDiminishedChordsWith4notes, nOfChordsWithLessThan4notes, nOfFundamentalStateChordsWithoutDoubledBass,
            nOfIncompleteChords, negativeCommonNotesInSameVoice[TENOR],
            negativeCommonNotesInSameVoice[ALTO], commonNotesInSameVoice[SOPRANO], costOfMelodicIntervals};
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
    variableBranchingStrategy = s.variableBranchingStrategy;
    valueBranchingStrategy = s.valueBranchingStrategy;

    bassMelodicIntervals.update(*this, s.bassMelodicIntervals);
    tenorMelodicIntervals.update(*this, s.tenorMelodicIntervals);
    altoMelodicIntervals.update(*this, s.altoMelodicIntervals);
    sopranoMelodicIntervals.update(*this, s.sopranoMelodicIntervals);

    squaredBassMelodicIntervals.update(*this, s.squaredBassMelodicIntervals);
    squaredTenorMelodicIntervals.update(*this, s.squaredTenorMelodicIntervals);
    squaredAltoMelodicIntervals.update(*this, s.squaredAltoMelodicIntervals);
    squaredSopranoMelodicIntervals.update(*this, s.squaredSopranoMelodicIntervals);
    allMelodicIntervals.update(*this, s.allMelodicIntervals);
    allSquaredMelodicIntervals.update(*this, s.allSquaredMelodicIntervals);

    bassTenorHarmonicIntervals.update(*this, s.bassTenorHarmonicIntervals);
    tenorAltoHarmonicIntervals.update(*this, s.tenorAltoHarmonicIntervals);
    altoSopranoHarmonicIntervals.update(*this, s.altoSopranoHarmonicIntervals);

    FullChordsVoicing.update(*this, s.FullChordsVoicing);

    nDifferentValuesInDiminishedChord.update(*this, s.nDifferentValuesInDiminishedChord);
    nDifferentValuesAllChords.update(*this, s.nDifferentValuesAllChords);
    nOccurrencesBassInFundamentalState.update(*this, s.nOccurrencesBassInFundamentalState);
    nOFDifferentNotesInChords.update(*this, s.nOFDifferentNotesInChords);
    commonNotesInSameVoice.update(*this, s.commonNotesInSameVoice);
    negativeCommonNotesInSameVoice.update(*this, s.negativeCommonNotesInSameVoice);

    nOfSeconds.update(*this, s.nOfSeconds);
    nOfThirds.update(*this, s.nOfThirds);
    nOfFourths.update(*this, s.nOfFourths);
    nOfFifths.update(*this, s.nOfFifths);
    nOfSixths.update(*this, s.nOfSixths);
    nOfSevenths.update(*this, s.nOfSevenths);
    nOfOctaves.update(*this, s.nOfOctaves);

    nOfDiminishedChordsWith4notes.update(*this, s.nOfDiminishedChordsWith4notes);
    nOfChordsWithLessThan4notes.update(*this, s.nOfChordsWithLessThan4notes);
    nOfFundamentalStateChordsWithoutDoubledBass.update(*this, s.nOfFundamentalStateChordsWithoutDoubledBass);
    nOfIncompleteChords.update(*this, s.nOfIncompleteChords);
    nOfCommonNotesInSameVoice.update(*this, s.nOfCommonNotesInSameVoice);
    costOfMelodicIntervals.update(*this, s.costOfMelodicIntervals);

    costVector = {nOfDiminishedChordsWith4notes, nOfChordsWithLessThan4notes, nOfFundamentalStateChordsWithoutDoubledBass,
                  nOfIncompleteChords, negativeCommonNotesInSameVoice[TENOR], negativeCommonNotesInSameVoice[ALTO],
                  commonNotesInSameVoice[SOPRANO], costOfMelodicIntervals};
}

/**
 * Returns the size of the problem
 * @return an integer representing the size of the vars array
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
 * Returns the values taken by the cost vector in a solution
 * @return an IntVarArgs representing the values of the cost vector in a solution
 */
IntVarArgs FourVoiceTexture::get_cost_vector() const{
    return costVector;
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
    message += "AllMelodicIntervals = \t\t" + intVarArray_to_string(allMelodicIntervals) + "\n\n";

    message += "squaredBassMelodicIntervals = \t\t" + intVarArray_to_string(squaredBassMelodicIntervals) + "\n";
    message += "squaredTenorMelodicIntervals = \t" + intVarArray_to_string(squaredTenorMelodicIntervals) + "\n";
    message += "squaredAltoMelodicIntervals = \t\t" + intVarArray_to_string(squaredAltoMelodicIntervals) + "\n";
    message += "squaredSopranoMelodicIntervals = \t" + intVarArray_to_string(squaredSopranoMelodicIntervals) + "\n\n";
    message += "allSquaredMelodicIntervals = \t\t" + intVarArray_to_string(allSquaredMelodicIntervals) + "\n\n";

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
    message += "negativeCommonNotesInSameVoice = \t" + intVarArray_to_string(negativeCommonNotesInSameVoice) + "\n";
    message += "nOFDifferentNotesInChords = \t\t" + intVarArray_to_string(nOFDifferentNotesInChords) + "\n\n";

    message += "nOfSeconds = \t\t" + intVar_to_string(nOfSeconds) + "\n";
    message += "nOfThirds = \t\t" + intVar_to_string(nOfThirds) + "\n";
    message += "nOfFourths = \t\t" + intVar_to_string(nOfFourths) + "\n";
    message += "nOfFifths = \t\t" + intVar_to_string(nOfFifths) + "\n";
    message += "nOfSixths = \t\t" + intVar_to_string(nOfSixths) + "\n";
    message += "nOfSevenths = \t\t" + intVar_to_string(nOfSevenths) + "\n";
    message += "nOfOctaves = \t\t" + intVar_to_string(nOfOctaves) + "\n\n";

    message += "------------------------------------cost variables----------------------------------------\n";

    message += "nOfDiminishedChordsWith4notes = " + intVar_to_string(nOfDiminishedChordsWith4notes) + "\n";
    message += "nOfChordsWithLessThan4notes = " + intVar_to_string(nOfChordsWithLessThan4notes) + "\n";
    message += "nOfFundamentalStateChordsWithoutDoubledBass = " +
            intVar_to_string(nOfFundamentalStateChordsWithoutDoubledBass) + "\n";
    message += "nOfIncompleteChords = " + intVar_to_string(nOfIncompleteChords) + "\n";
    message += "nOfCommonNotesInSameVoice = " + intVar_to_string(nOfCommonNotesInSameVoice) + "\n";
    message += "nOfCommonNotesTenor = " + intVar_to_string(commonNotesInSameVoice[TENOR]) + "\n";
    message += "nOfCommonNotesAlto = " + intVar_to_string(commonNotesInSameVoice[ALTO]) + "\n";
    message += "nOfCommonNotesSoprano = " + intVar_to_string(commonNotesInSameVoice[SOPRANO]) + "\n";
    message += "nOfCommonNotesBass = " + intVar_to_string(commonNotesInSameVoice[BASS]) + "\n";
    message += "costOfMelodicIntervals = " + intVar_to_string(costOfMelodicIntervals) + "\n\n";

    message += "Cost vector = {" + intVarArgs_to_string(costVector) + "}\n\n";

    return message;
}