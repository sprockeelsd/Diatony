#include "headers/Constraints.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                  Generic constraints                                                *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Link the melodic intervals arrays to the FullChordsVoicing array
 * @param home The instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 */
void link_melodic_arrays(const Home& home, int n, IntVarArray FullChordsVoicing, IntVarArray bassMelodicIntervals,
                         IntVarArray tenorMelodicIntervals, IntVarArray altoMelodicIntervals,
                         IntVarArray sopranoMelodicIntervals){
    for (int i = 0; i < n - 1; ++i)
    {
        rel(home, bassMelodicIntervals[i] == FullChordsVoicing[(i + 1) * 4] - FullChordsVoicing[i * 4]);
        rel(home, tenorMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + 1] - FullChordsVoicing[(i * 4) + 1]);
        rel(home, altoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + 2] - FullChordsVoicing[(i * 4) + 2]);
        rel(home, sopranoMelodicIntervals[i] == FullChordsVoicing[((i + 1) * 4) + 3] - FullChordsVoicing[(i * 4) + 3]);
    }
}

/**
 * Link the absolute melodic intervals arrays to the corresponding melodic interval arrays
 * @param home the instance of the problem
 * @param bassMelodicIntervals the melodic intervals of the bass
 * @param tenorMelodicIntervals the melodic intervals of the tenor
 * @param altoMelodicIntervals the melodic intervals of the alto
 * @param sopranoMelodicIntervals the melodic intervals of the soprano
 * @param absoluteBassMelodicIntervals the absolute melodic intervals of the bass
 * @param absoluteTenorMelodicIntervals the absolute melodic intervals of the tenor
 * @param absoluteAltoMelodicIntervals the absolute melodic intervals of the alto
 * @param absoluteSopranoMelodicIntervals the absolute melodic intervals of the soprano
 */
void link_absolute_melodic_arrays(const Home& home, IntVarArray bassMelodicIntervals, IntVarArray tenorMelodicIntervals,
                                  IntVarArray altoMelodicIntervals, IntVarArray sopranoMelodicIntervals,
                                  IntVarArray absoluteBassMelodicIntervals, IntVarArray absoluteTenorMelodicIntervals,
                                  IntVarArray absoluteAltoMelodicIntervals, IntVarArray absoluteSopranoMelodicIntervals){
    for(int i = 0; i < bassMelodicIntervals.size(); ++i){
        abs(home, bassMelodicIntervals[i], absoluteBassMelodicIntervals[i]);
        abs(home, tenorMelodicIntervals[i], absoluteTenorMelodicIntervals[i]);
        abs(home, altoMelodicIntervals[i], absoluteAltoMelodicIntervals[i]);
        abs(home, sopranoMelodicIntervals[i], absoluteSopranoMelodicIntervals[i]);
    }
}

/**
 * Link the harmonic intervals arrays to the FullChordsVoicing array
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 * @param bassTenorHarmonicIntervals the harmonic intervals between bass and tenor
 * @param tenorAltoHarmonicIntervals the harmonic intervals between tenor and alto
 * @param altoSopranoHarmonicIntervals the harmonic intervals between alto and soprano
 */
void link_harmonic_arrays(const Home& home, int n, IntVarArray FullChordsVoicing, IntVarArray bassTenorHarmonicIntervals,
                          IntVarArray tenorAltoHarmonicIntervals, IntVarArray altoSopranoHarmonicIntervals){
    for(int i = 0; i < n; ++i){
        rel(home, bassTenorHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + 1] - FullChordsVoicing[4 * i]);
        rel(home, tenorAltoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + 2] - FullChordsVoicing[(4 * i) + 1]);
        rel(home, altoSopranoHarmonicIntervals[i] == FullChordsVoicing[(4 * i) + 3] - FullChordsVoicing[(4 * i) + 2]);
    }
}

/**
 * Sets the domains of the different voices to their range
 *      bass: [40, 60] E2 -> C3
 *      tenor: [48, 69] C2 -> A3
 *      alto: [55, 75] G2 -> D3
 *      soprano: [60, 84] C3 -> A4
 * @param home the instance of the problem
 * @param n the number of chords
 * @param FullChordsVoicing the array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 */
void restrain_voices_domains(const Home& home, int n, IntVarArray FullChordsVoicing){
    for (int i = 0; i < n; ++i)
    {
        IntVarArgs currentChord(FullChordsVoicing.slice(4 * i, 1, 4));
        rel(home, currentChord, IRT_LQ); // bass[i] <= tenor[i] <= alto[i] <= soprano[i]

        // E2 <= bass <= C3
        rel(home, currentChord[0], IRT_GQ, 40);
        rel(home, currentChord[0], IRT_LQ, 60);
        // C2 <= tenor <= A3
        rel(home, currentChord[1], IRT_GQ, 48);
        rel(home, currentChord[1], IRT_LQ, 69);
        // G2 <= alto <= D3
        rel(home, currentChord[2], IRT_GQ, 55);
        rel(home, currentChord[2], IRT_LQ, 75);
        // C3 <= soprano <= A4
        rel(home, currentChord[3], IRT_GQ, 60);
        rel(home, currentChord[3], IRT_LQ, 84);
    }
}

/**
 * Forbids parallel intervals between two chords in the same voices
 * @todo change to argument variables later + maybe make it cleaner
 * @param home the instance of the problem
 * @param forbiddenParallelInterval the interval to forbid
 * @param currentPosition the current chord position
 * @param lowerVoiceID the ID of the lower voice (0 -> bass, 1 -> tenor, 2 -> alto, 3 -> soprano)
 * @param voicesHarmonicIntervals The variable array containing the harmonic intervals between the two voices
 * @param FullChordsVoicing the variable array containing all the chords in the form [bass0, alto0, tenor0, soprano0, bass1, alto1, tenor1, soprano1, ...]
 */
void forbid_parallel_intervals(Home home, int forbiddenParallelInterval, int currentPosition, int lowerVoiceID,
                               IntVarArray voicesHarmonicIntervals, IntVarArray FullChordsVoicing){
    //bassTenorIntervalForbidden is true if the interval is forbiddenParallelInterval
    BoolVar harmonicIntervalForbidden(home, 0, 1);
    rel(home, harmonicIntervalForbidden, IRT_EQ, expr(home, voicesHarmonicIntervals[currentPosition] %
                                                             PERFECT_OCTAVE == forbiddenParallelInterval));

    // is true if lower voice is the same note in both chords
    BoolVar notesLowerVoiceEqual(home, 0, 1);
    rel(home, notesLowerVoiceEqual, IRT_EQ, expr(home, FullChordsVoicing[currentPosition * 4 + lowerVoiceID]
                                                        == FullChordsVoicing[(currentPosition + 1) *4 + lowerVoiceID]));

    // is true if upper voice is the same note in both chords
    BoolVar notesUpperVoiceEqual(home,0,1);
    rel(home, notesUpperVoiceEqual, IRT_EQ, expr(home, FullChordsVoicing[currentPosition * 4 + lowerVoiceID + 1]
                                                    == FullChordsVoicing[(currentPosition + 1) *4 + lowerVoiceID + 1]));

    // is true if both voices are the same in both chords
    BoolVar bothVoicesEqual(home, 0, 1);
    rel(home, notesLowerVoiceEqual, BOT_AND, notesUpperVoiceEqual, bothVoicesEqual);

    // is true if the harmonic interval between the voices is not forbidden
    BoolVar nextIntervalNotParallel(home, 0,1);
    rel(home, nextIntervalNotParallel, IRT_EQ, expr(home, voicesHarmonicIntervals[currentPosition+1] %
                                                                PERFECT_OCTAVE != forbiddenParallelInterval));

    BoolVar isNextIntervalValid(home,0,1);
    rel(home, bothVoicesEqual, BOT_OR, nextIntervalNotParallel, isNextIntervalValid); // ok

    rel(home, harmonicIntervalForbidden, BOT_IMP, isNextIntervalValid, true);

}


/***********************************************************************************************************************
 *                                                                                                                     *
 *                                               Chord-related constraints                                             *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Set the notes of the FullChordsVoicing array to the notes of the given chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setToChord(const Home& home, Tonality* tonality, int degree, const IntVarArgs& currentChord){
    std::cout << "chord : " << tonality->get_scale_degree_chord(degree) << std::endl;
    dom(home, currentChord, tonality->get_scale_degree_chord(degree));
}

/**
 * Set the bass of the chord to be the given note
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param state the state of the chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void setBass(const Home& home, Tonality *tonality, int degree, int state, IntVarArgs currentChord){
    dom(home, currentChord[0], tonality->get_scale_degree((degree + 2*state) % 7));
}

/** ---------------------------------------------Fundamental state chords--------------------------------------------- */

/**
 * @todo change this for complete and incomplete chords later (third must be <=1 depending on the chord before and after if they are 5->1 and complete/incomplete)
 * @todo maybe make it a preference later
 * Sets the number of times each note of the notes of the chord are present in the chord
 * @param home the instance of the problem
 * @param tonality the tonality of the piece
 * @param degree the degree of the chord
 * @param previous_chord_degree the degree of the following chord
 * @param currentChord the array containing a chord in the form [bass, alto, tenor, soprano]
 */
void chordNoteOccurrenceFundamentalState(const Home& home, Tonality *tonality, int degree, int previous_chord_degree,
                                         const IntVarArgs& currentChord){
    /// exceptions
    //@todo see what Karim says about this
    if(degree == SIXTH_DEGREE && previous_chord_degree == FIFTH_DEGREE){ // 5->6 degree in fundamental state (broken cadence)
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_EQ,1); // the fundamental should be present once
        count(home, currentChord, tonality->get_scale_degree((degree + 2) % 7), IRT_EQ,2); // double the third
        count(home, currentChord, tonality->get_scale_degree((degree + 4) % 7), IRT_EQ, 1); // the fifth should be present once
    }
    /// default = double the fundamental
    else{
        count(home, currentChord, tonality->get_scale_degree(degree), IRT_EQ,2); // double the bass which is also the tonic
        count(home, currentChord, tonality->get_scale_degree((degree + 2) % 7), IRT_EQ,1); // the third should be present once
        count(home, currentChord, tonality->get_scale_degree((degree + 4) % 7), IRT_EQ, 1); // the fifth should be present once
    }
    /// exceptions that add on top of the rest
    if(tonality->get_chord_qualities()[degree] == DIMINISHED_CHORD){
        nvalues(home, currentChord, IRT_EQ,3); // there should only be 3 different notes
    }
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                            Voice leading related constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the rules for the melodic movements between chords in fundamental state
 * For chords that are 1 degree apart, the other voices must move in contrary motion to the bass
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param chordDegrees the array containing the degrees of the chords in the progression
 * @param bassMelodicInterval The melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 */
void fundamentalStateChordToFundamentalStateChord(const Home& home, int currentPosition, vector<int> chordDegrees,
                                                  Tonality &tonality,
                                                  const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                                  const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                                  IntVarArray fullChordsVoicing){
    int degreeDifference = abs(chordDegrees[currentPosition+1] - chordDegrees[currentPosition]);

    /** First, corner cases that differ from the general rules */
    if(chordDegrees[currentPosition] == FIFTH_DEGREE && chordDegrees[currentPosition+1] == SIXTH_DEGREE){
        fifthDegreeFSToSixthDegreeFS(home, currentPosition, tonality, tenorMelodicInterval, altoMelodicInterval, sopranoMelodicInterval, fullChordsVoicing);
    }
    /** general cases */
    else if(degreeDifference == MINOR_SECOND || degreeDifference == MAJOR_SECOND){
        // other voices need to move by contrary motion to the bass
        rel(home, expr(home, bassMelodicInterval[currentPosition] > 0), BOT_EQV, expr(home, tenorMelodicInterval[currentPosition] < 0), true);
        rel(home, expr(home, bassMelodicInterval[currentPosition] > 0), BOT_EQV, expr(home, altoMelodicInterval[currentPosition] < 0), true);
        rel(home, expr(home, bassMelodicInterval[currentPosition] > 0), BOT_EQV, expr(home, sopranoMelodicInterval[currentPosition] < 0), true);

        rel(home, expr(home, bassMelodicInterval[currentPosition] < 0), BOT_EQV, expr(home, tenorMelodicInterval[currentPosition] > 0), true);
        rel(home, expr(home, bassMelodicInterval[currentPosition] < 0), BOT_EQV, expr(home, altoMelodicInterval[currentPosition] > 0), true);
        rel(home, expr(home, bassMelodicInterval[currentPosition] < 0), BOT_EQV, expr(home, sopranoMelodicInterval[currentPosition] > 0), true);
    }
    else{ // there is at least one common note in the 2 chords -> keep that (these) notes in the same voices and move the other to the closest one
            //@todo calculer l'intersection des domaines et si la valeur est dedans => la suivante doit l'être aussi (sauf à la basse) -> dom cst reified
    }
}

/**
 * Sets the constraint for a fifth degree followed by a sixth degree in fundamental state
 *      the seventh of the scale must rise to the tonic and the other voices are going down (except for the bass)
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param tenorMelodicInterval the melodic intervals of the tenor
 * @param altoMelodicInterval the melodic intervals of the alto
 * @param sopranoMelodicInterval the melodic intervals of the soprano
 * @param fullChordsVoicing the array containing the notes of the chords in the progression
 */
void fifthDegreeFSToSixthDegreeFS(const Home& home, int currentPosition, Tonality& tonality,
                                  const IntVarArray& tenorMelodicInterval, const IntVarArray& altoMelodicInterval,
                                  const IntVarArray& sopranoMelodicInterval, IntVarArray fullChordsVoicing){

    // @todo : check that it actually doubles the third of the sixth chord which is the tonic + make it cleaner with loops

    // soprano note is the seventh of the scale
    // -> that voice must raise to the tonic by a minor second
    /// If the leading tone is in the soprano, it must rise to the tonic regardless of the mode
    rel(home, expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
        BOT_IMP, expr(home, sopranoMelodicInterval[currentPosition] ==1 ), true);
    // other voices must go down
    rel(home, expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
        BOT_IMP, expr(home, tenorMelodicInterval[currentPosition] < 0), true);
    rel(home, expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
        BOT_IMP, expr(home, altoMelodicInterval[currentPosition] < 0), true);

    /// If the mode is minor, then the leading tone always has to rise to the tonic
    if(tonality.get_mode() == MINOR_MODE){
        // tenor note is the seventh of the scale
        // -> that voice must raise to the tonic by a minor second
        rel(home, expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
            BOT_IMP,expr(home, tenorMelodicInterval[currentPosition] ==1 ), true);
        //other voices must go down
        rel(home, expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
            BOT_IMP,expr(home, altoMelodicInterval[currentPosition] < 0), true);
        rel(home, expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
            BOT_IMP,expr(home, sopranoMelodicInterval[currentPosition] < 0), true);

        // alto note is the seventh of the scale
        // -> that voice must raise to the tonic by a minor second
        rel(home, expr(home, fullChordsVoicing[currentPosition +  ALTO] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
            BOT_IMP, expr(home, altoMelodicInterval[currentPosition] ==1 ), true);
        // other voices must go down
        rel(home, expr(home, fullChordsVoicing[currentPosition +  ALTO] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
            BOT_IMP, expr(home, tenorMelodicInterval[currentPosition] < 0), true);
        rel(home, expr(home, fullChordsVoicing[currentPosition +  ALTO] % PERFECT_OCTAVE + PERFECT_OCTAVE == tonality.get_degree_note(SEVENTH_DEGREE)),
            BOT_IMP, expr(home, sopranoMelodicInterval[currentPosition] < 0), true);
    }
}