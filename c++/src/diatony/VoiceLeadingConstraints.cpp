// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A file that contains all the melodic constraints for the diatonic harmonic problem
//
#include "../../headers/diatony/VoiceLeadingConstraints.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the voice leading constraints, that is constraints on how voices move through successive     *
 * chords.                                                                                                             *
 * It currently contains the following constraints:                                                                    *
 *      - forbid_parallel_interval: forbids a given parallel interval between two voices                               *
 *      - fundamental_state_chord_to_fundamental_state_chord: sets the rules for the melodic movements between chords  *
 *      in fundamental state                                                                                           *
 *      - interrupted_cadence: sets the constraint for a fifth degree followed by a sixth degree in                    *
 *      fundamental state                                                                                              *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Forbids a list of parallel intervals between two voices. This calls the forbid_parallel_interval function
 * @param home the instance of the problem
 * @param size the size of the chord progression
 * @param nOfVoices the number of voices
 * @param intervals the list of intervals to forbid
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 * @param bassTenorHarmonicIntervals the array containing the harmonic intervals between bass and tenor
 * @param bassAltoHarmonicIntervals the array containing the harmonic intervals between bass and alto
 * @param bassSopranoHarmonicIntervals the array containing the harmonic intervals between bass and soprano
 * @param tenorAltoHarmonicIntervals the array containing the harmonic intervals between tenor and alto
 * @param tenorSopranoHarmonicIntervals the array containing the harmonic intervals between tenor and soprano
 * @param altoSopranoHarmonicIntervals the array containing the harmonic intervals between alto and soprano
 */
void forbid_parallel_intervals(const Home &home, int size, int nOfVoices, const vector<int> &intervals,
                               const IntVarArray &FullChordsVoicing, const IntVarArray &bassTenorHarmonicIntervals,
                               const IntVarArray &bassAltoHarmonicIntervals,
                               const IntVarArray &bassSopranoHarmonicIntervals,
                               const IntVarArray &tenorAltoHarmonicIntervals,
                               const IntVarArray &tenorSopranoHarmonicIntervals,
                               const IntVarArray &altoSopranoHarmonicIntervals) {
    for(int chord = 0; chord < size - 1; chord++){ /// for each chord
        for(int interval : intervals){ /// for each interval
            /// from bass
            forbid_parallel_interval(home, nOfVoices, interval, chord,
                                     BASS, TENOR, bassTenorHarmonicIntervals,
                                     FullChordsVoicing); // between bass and tenor
            forbid_parallel_interval(home, nOfVoices, interval, chord,
                                     BASS, ALTO, bassAltoHarmonicIntervals,
                                     FullChordsVoicing); // between tenor and alto
            forbid_parallel_interval(home, nOfVoices, interval, chord,
                                     BASS,SOPRANO, bassSopranoHarmonicIntervals,
                                     FullChordsVoicing); // between alto and soprano
            /// from tenor
            forbid_parallel_interval(home, nOfVoices, interval, chord,
                                     TENOR,ALTO, tenorAltoHarmonicIntervals,
                                     FullChordsVoicing); // between tenor and alto
            forbid_parallel_interval(home, nOfVoices, interval, chord,
                                     TENOR,SOPRANO, tenorSopranoHarmonicIntervals,
                                     FullChordsVoicing); // between tenor and soprano
            /// from alto
            forbid_parallel_interval(home, nOfVoices, interval, chord,
                                     ALTO,SOPRANO, altoSopranoHarmonicIntervals,
                                     FullChordsVoicing); // between alto and soprano
        }
    }
}

/**
 * Forbids a given parallel interval between two given voices
 * @todo make it with argument variables + make it cleaner
 * @param home the instance of the problem
 * @param nVoices the number of voices
 * @param forbiddenParallelInterval the interval that must not be parallel
 * @param currentPosition the current position in the chord progression
 * @param voice1ID the ID of the first voice
 * @param voice2ID the ID of the second voice
 * @param voicesHarmonicIntervals the array containing the harmonic intervals between adjacent voices
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 */
void forbid_parallel_interval(Home home, int nVoices, int forbiddenParallelInterval, int currentPosition, int voice1ID,
                              int voice2ID, IntVarArray voicesHarmonicIntervals, IntVarArray FullChordsVoicing) {

    /// harmonicIntervalForbidden is true if the interval is forbiddenParallelInterval
    BoolVar harmonicIntervalForbidden(home, 0, 1);
    rel(home, harmonicIntervalForbidden, IRT_EQ, expr(home, voicesHarmonicIntervals[currentPosition] %
                                                            PERFECT_OCTAVE == forbiddenParallelInterval));

    /// notesLowerVoiceEqual is true if voice1 is the same note in both chords
    BoolVar notesLowerVoiceEqual(home, 0, 1);
    rel(home, notesLowerVoiceEqual, IRT_EQ,
        expr(home, FullChordsVoicing[currentPosition * nVoices + voice1ID] ==
                FullChordsVoicing[(currentPosition + 1) *nVoices + voice1ID]));

    /// notesUpperVoiceEqual is true if voice2 is the same note in both chords
    BoolVar notesUpperVoiceEqual(home,0,1);
    rel(home, notesUpperVoiceEqual, IRT_EQ,
        expr(home, FullChordsVoicing[currentPosition * nVoices + voice2ID]
                == FullChordsVoicing[(currentPosition + 1) * nVoices + voice2ID]));

    /// bothVoicesEqual is true if both voices are the same in both chords
    BoolVar bothVoicesEqual(home, 0, 1);
    rel(home, notesLowerVoiceEqual, BOT_AND, notesUpperVoiceEqual, bothVoicesEqual);

    /// nextIntervalNotParallel is true if the harmonic interval between the voices is not forbidden
    BoolVar nextIntervalNotParallel(home, 0,1);
    rel(home, nextIntervalNotParallel, IRT_EQ,
        expr(home, voicesHarmonicIntervals[currentPosition+1] % PERFECT_OCTAVE != forbiddenParallelInterval));

    /// isNextIntervalValid is true if the next interval is not parallel or if the both voices are equal in the 2 chords
    BoolVar isNextIntervalValid(home,0,1);
    rel(home, bothVoicesEqual, BOT_OR, nextIntervalNotParallel, isNextIntervalValid); // ok

    rel(home, harmonicIntervalForbidden, BOT_IMP, isNextIntervalValid, true);
}

/**
 * Adds the constraint that Soprano, Alto and Tenor must move in contrary motion to the bass
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param bassMelodicInterval the array containing the melodic intervals of the bass
 * @param tenorMelodicInterval the array containing the melodic intervals of the tenor
 * @param altoMelodicInterval the array containing the melodic intervals of the alto
 * @param sopranoMelodicInterval the array containing the melodic intervals of the soprano
 */
void contrary_motion_to_bass(const Home& home, int currentPosition, const IntVarArray& bassMelodicInterval,
                             const IntVarArray& tenorMelodicInterval, const IntVarArray& altoMelodicInterval,
                             const IntVarArray& sopranoMelodicInterval){
    /// other voices need to move by contrary motion to the bass
    rel(home, expr(home, bassMelodicInterval[currentPosition] > 0), BOT_EQV,
        expr(home, tenorMelodicInterval[currentPosition] < 0), true);
    rel(home, expr(home, bassMelodicInterval[currentPosition] > 0), BOT_EQV,
        expr(home, altoMelodicInterval[currentPosition] < 0), true);
    rel(home, expr(home, bassMelodicInterval[currentPosition] > 0), BOT_EQV,
        expr(home, sopranoMelodicInterval[currentPosition] < 0), true);

    rel(home, expr(home, bassMelodicInterval[currentPosition] < 0), BOT_EQV,
        expr(home, tenorMelodicInterval[currentPosition] > 0), true);
    rel(home, expr(home, bassMelodicInterval[currentPosition] < 0), BOT_EQV,
        expr(home, altoMelodicInterval[currentPosition] > 0), true);
    rel(home, expr(home, bassMelodicInterval[currentPosition] < 0), BOT_EQV,
        expr(home, sopranoMelodicInterval[currentPosition] > 0), true);
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                          Fundamental state chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the constraint for a fifth degree followed by a sixth degree in fundamental state
 *      the seventh of the scale must rise to the tonic and the other voices are going down (except for the bass)
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing the notes of the chords in the progression
 * @param tenorMelodicInterval the melodic intervals of the tenor
 * @param altoMelodicInterval the melodic intervals of the alto
 * @param sopranoMelodicInterval the melodic intervals of the soprano
 */
void interrupted_cadence(const Home &home, int currentPosition, Tonality *tonality, IntVarArray fullChordsVoicing,
                         const IntVarArray &tenorMelodicInterval, const IntVarArray &altoMelodicInterval,
                         const IntVarArray &sopranoMelodicInterval) {
    // @todo make it cleaner with loops
    /// if the mode is major, then this rule only applies to the soprano voice. Otherwise, it applies for all voices
    /// soprano note is the seventh of the scale -> that voice must raise to the tonic by a minor second
    auto leadingTone = tonality->get_tonic() + MAJOR_SEVENTH % PERFECT_OCTAVE;

    /// If the leading tone is in the soprano, it must rise to the tonic regardless of the mode
    rel(home,
        expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE == leadingTone),
        BOT_IMP,
        expr(home, sopranoMelodicInterval[currentPosition] ==1),
true);
    /// If the leading tone is in the soprano, other voices must go down (except for the bass which goes up by default)
    rel(home,
        expr(home,fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE == leadingTone),
         BOT_IMP,
         expr(home, tenorMelodicInterval[currentPosition] < 0),
 true);
    rel(home,
        expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE == leadingTone),
        BOT_IMP,
        expr(home, altoMelodicInterval[currentPosition] < 0),
true);

    /// If the mode is minor, then the leading tone always has to rise to the tonic -> same as for soprano but for other voices too
    if(tonality->get_mode() == MINOR_MODE){
        /// tenor note is the leading tone -> that voice must raise to the tonic by a minor second
        rel(home,
            expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE == leadingTone),
            BOT_IMP,
            expr(home, tenorMelodicInterval[currentPosition] ==1 ),
    true);
        /// other voices must go down
        rel(home,
            expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE == leadingTone),
            BOT_IMP,
            expr(home, altoMelodicInterval[currentPosition] < 0),
    true);
        rel(home, expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE ==
                tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
            expr(home, sopranoMelodicInterval[currentPosition] < 0), true);

        // alto note is the seventh of the scale
        // -> that voice must raise to the tonic by a minor second
        rel(home, expr(home, fullChordsVoicing[currentPosition +  ALTO] % PERFECT_OCTAVE ==
                tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
            expr(home, altoMelodicInterval[currentPosition] ==1 ), true);
        // other voices must go down
        rel(home, expr(home, fullChordsVoicing[currentPosition +  ALTO] % PERFECT_OCTAVE ==
                tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
            expr(home, tenorMelodicInterval[currentPosition] < 0), true);
        rel(home, expr(home, fullChordsVoicing[currentPosition +  ALTO] % PERFECT_OCTAVE ==
                tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
            expr(home, sopranoMelodicInterval[currentPosition] < 0), true);
    }
}

/**
 * Posts the voicing constraints for augmented sixth chords (italian)
 * @param home
 * @param nOfVoices
 * @param currentPosition
 * @param tonality
 * @param fullChordsVoicing
 * @param bassMelodicIntervals
 * @param tenorMelodicIntervals
 * @param altoMelodicIntervals
 * @param sopranoMelodicIntervals
 */
void italian_augmented_sixth(const Home &home, int nOfVoices, int currentPosition, Tonality *tonality, IntVarArray fullChordsVoicing,
                             const IntVarArray &bassMelodicIntervals, const IntVarArray &tenorMelodicIntervals,
                             const IntVarArray &altoMelodicIntervals, const IntVarArray &sopranoMelodicIntervals) {
    IntVarArgs currentChord = fullChordsVoicing.slice(nOfVoices * currentPosition, 1, nOfVoices);

    vector<IntVarArray> melodicIntervals(
            {bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals});

    for(int voice = TENOR; voice <= SOPRANO; voice++) {
        /// third of the chord goes up by step or down by half step
        rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE == tonality->get_tonic()), BOT_IMP,
        expr(home, melodicIntervals[voice][currentPosition] == -MINOR_SECOND || melodicIntervals[voice][currentPosition] == MAJOR_SECOND), true);

        rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE ==
            (tonality->get_degree_note(AUGMENTED_SIXTH) + MINOR_SIXTH) % PERFECT_OCTAVE), BOT_IMP,
        expr(home, melodicIntervals[voice][currentPosition] == -MINOR_SECOND), true);
    }
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                           First inversion chord constraints                                         *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                           Second inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the constraint for a first degree in second inversion followed by a fifth degree (appogiatura)
 * @param home the instance of the problem
 * @param nVoices the number of voices in the piece
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 * @param bassMelodicInterval the melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 */
void fifth_degree_appogiatura(const Home& home, int nVoices, int currentPosition, Tonality *tonality, IntVarArray fullChordsVoicing,
                              IntVarArray bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                              const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval){
    IntVarArgs currentChord = fullChordsVoicing.slice(nVoices * currentPosition, 1, nVoices);

    vector<IntVarArray> melodicIntervals(
            {bassMelodicInterval, tenorMelodicInterval, altoMelodicInterval, sopranoMelodicInterval});
    /// appogiatura of the fifth degree: the fundamental and third of the I chord must go down
    for(int voice = TENOR; voice <= SOPRANO; voice++){
        /// the fundamental of the tonality must go down by a half step
        rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE == tonality->get_tonic()), BOT_IMP,
            expr(home, melodicIntervals[voice][currentPosition] == -MINOR_SECOND), true);
        /// the third of the scale must go down to the second by step
        rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE == tonality->get_degree_note(THIRD_DEGREE)),
                       BOT_IMP, expr(home, melodicIntervals[voice][currentPosition] < UNISSON), true);
        rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE == tonality->get_degree_note(THIRD_DEGREE)),
            BOT_IMP, expr(home, melodicIntervals[voice][currentPosition] >= -MAJOR_SECOND), true);
    }
    if (currentPosition > 0){ /// if it is not the first chord in the progression
        /// the voice containing the tonic must go in opposite or oblique motion to the bass (the tonic must be approached by direct or oblique motion)
        BoolVar bassRises = expr(home, bassMelodicInterval[currentPosition-1] >= 0);
        for(int voice = TENOR; voice <= SOPRANO; voice++){
            /// if the voice is playing the tonic and the bass rises, this voice must go down or stay the same
            rel(home, expr(home, fullChordsVoicing[currentPosition * nVoices + voice] % PERFECT_OCTAVE ==
                    tonality->get_tonic()), BOT_IMP, expr(home, melodicIntervals[voice][currentPosition - 1] <= 0), bassRises);
        }
    }
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                         Constraints independent of chord state                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * todo modify so that it also works for secondary dominant chords, right now it is based on the scale and not on the chord degree
 * todo make it cleaner, the two parts should be joined
 * Forces the tritone to resolve properly
 * @param home the instance of the problem
 * @param nVoices the number of voices in the piece
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param chordDegs the degrees of the chords
 * @param chordStas the state of the chords
 * @param bassMelodicIntervals the melodic interval of the bass between the current position and the next
 * @param tenorMelodicIntervals the melodic interval of the tenor between the current position and the next
 * @param altoMelodicIntervals the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicIntervals the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void tritone_resolution(const Home &home, int nVoices, int currentPosition, Tonality *tonality, vector<int> chordDegs,
                        vector<int> chordQuals, vector<int> chordStas, const IntVarArray &bassMelodicIntervals,
                        const IntVarArray &tenorMelodicIntervals, const IntVarArray &altoMelodicIntervals,
                        const IntVarArray &sopranoMelodicIntervals, IntVarArray fullChordsVoicing) {

    IntVarArgs currentChord (fullChordsVoicing.slice(nVoices * currentPosition, 1, nVoices));
    vector<IntVarArray> melodicIntervals({bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals});

    if (chordDegs[currentPosition] == FIFTH_DEGREE || chordDegs[currentPosition] == SEVENTH_DEGREE) {
        std::cout << "five or seven" << std::endl;
        for(int voice = BASS; voice <= SOPRANO; voice++){
            /// special case
            /// if the chords are VII (1st inversion) -> I (1st inversion) or V(+6)->I(6)
            if ((chordDegs[currentPosition]     == SEVENTH_DEGREE   && chordStas[currentPosition]   == FIRST_INVERSION &&
                chordDegs[currentPosition+1]    == FIRST_DEGREE     && chordStas[currentPosition+1] == FIRST_INVERSION) ||
                (chordDegs[currentPosition]     == FIFTH_DEGREE     && chordQuals[currentPosition]  == DOMINANT_SEVENTH_CHORD &&
                chordStas[currentPosition]      == SECOND_INVERSION
                && chordDegs[currentPosition+1] == FIRST_DEGREE     && chordStas[currentPosition+1]    == FIRST_INVERSION)){
                /// the fourth of the scale must go up by a step
                rel(home,
                    /// the note in this voice is the fourth of the scale
                    expr(home,currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + PERFECT_FOURTH) % PERFECT_OCTAVE),
                    BOT_IMP,
                    /// it must go up by step
                    expr(home, expr(home, melodicIntervals[voice][currentPosition] > 0) && expr(home, melodicIntervals[voice][currentPosition] <= 2)),
            true);
            }
            else{ /// the fourth of the scale must go down by a step
                rel(home,
                    /// the note is the fourth of the scale
                    expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + PERFECT_FOURTH) % PERFECT_OCTAVE),
                    BOT_IMP,
                    /// it must go down by step
                    expr(home, expr(home, melodicIntervals[voice][currentPosition] < 0) && expr(home, melodicIntervals[voice][currentPosition] >= -2)),
            true);
            }
            /// if the note is the leading tone of the scale, it must go up to the tonic by step
            rel(home,
                expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + MAJOR_SEVENTH) % PERFECT_OCTAVE),
                BOT_IMP,
                expr(home, melodicIntervals[voice][currentPosition] == 1),
                true);
        }
    }
    else { /// secondary dominant chords
        std::cout << "secondary dominant" << std::endl;
        auto third = (tonality->get_degree_note(chordDegs[currentPosition]) + MAJOR_THIRD) % PERFECT_OCTAVE;
        auto seventh = (tonality->get_degree_note(chordDegs[currentPosition]) + MINOR_SEVENTH) % PERFECT_OCTAVE;
        std::cout << "third: " << third << " seventh: " << seventh << std::endl;

        for(int voice = BASS; voice <= SOPRANO; voice++) {
            rel(home,
                        /// the note is the seventh of the chord
                        expr(home, currentChord[voice] % PERFECT_OCTAVE == seventh),
                        BOT_IMP,
                        /// it must go down by step
                        expr(home, expr(home, melodicIntervals[voice][currentPosition] < 0) && expr(home, melodicIntervals[voice][currentPosition] >= -2)),
                true);
            rel(home,
                        /// the note is the third of the chord
                        expr(home, currentChord[voice] % PERFECT_OCTAVE == third),
                        BOT_IMP,
                        /// it must go up by step
                        expr(home, melodicIntervals[voice][currentPosition] == 1),
                true);
        }
    }
}

