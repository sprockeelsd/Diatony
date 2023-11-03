#include "headers/VoiceLeadingConstraints.hpp"

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
 * Forbids a given parallel interval between two voices
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
 * Sets the general rules for the melodic movements between chords
 * Ensures that common notes between chords are kept in the same voice
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param chordDegrees the array containing the degrees of the chords in the progression
 * @param tonality the tonality of the piece
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
 void keep_common_notes_in_same_voice(const Home &home, int nVoices, int currentPosition, vector<int> chordDegrees,
                                      Tonality *tonality, IntVarArray fullChordsVoicing) {
    /// keep common notes in the same voice and move other voices as closely as possible (cost)
    // chord qualities
    vector<int> thisChordQuality = tonality->get_chord_qualities()[chordDegrees[currentPosition]];
    vector<int> nextChordQuality = tonality->get_chord_qualities()[chordDegrees[currentPosition+1]];

    /// notes of the current chord @todo maybe add sets in the tonality class with the notes for each chord
    int new_note = tonality->get_degree_note(chordDegrees[currentPosition]);
    set<int> thisChord = {new_note}; // notes of the chord
    for(int i = 0; i < thisChordQuality.size() - 1; ++i){ // -1 because the last note is the same as the first
        new_note = (new_note + thisChordQuality[i]) % PERFECT_OCTAVE;
        thisChord.insert(new_note);
    }

    /// notes of the next chord
    int new_note_new_chord = tonality->get_degree_note(chordDegrees[currentPosition+1]);
    set<int> nextChord = {new_note_new_chord};
    for(int i = 0; i < nextChordQuality.size() - 1; ++i){ // -1 because the last note is the same as the first
        new_note_new_chord = (new_note_new_chord + nextChordQuality[i]) % PERFECT_OCTAVE;
        nextChord.insert(new_note_new_chord);
    }

    /// for each note in the current chord domain, if the note is in the next chord as well, it has to be in the same voice
    /// but if it is in the soprano, then increase the cost
    for(auto it : thisChord){ // for each note in the current chord domain
        if(nextChord.find(it) != nextChord.end()){ // if the note is in the next chord as well
            for(int i = TENOR; i <= SOPRANO; ++i){ // for all voices except the bass
                // the note in the current chord in the voice i must be the same in the next chord
                BoolVar thisVoiceThisChordContainsTheNote = expr(home, fullChordsVoicing[currentPosition * nVoices + i] % PERFECT_OCTAVE == it);
                BoolVar thisVoiceNextChordContainsTheNote = expr(home, fullChordsVoicing[(currentPosition + 1) * nVoices + i] % PERFECT_OCTAVE == it);

                rel(home, thisVoiceThisChordContainsTheNote,BOT_IMP,expr(home,fullChordsVoicing[(currentPosition + 1) * nVoices + i] ==
                               fullChordsVoicing[currentPosition * nVoices + i]), true);
                rel(home, thisVoiceNextChordContainsTheNote,BOT_IMP, expr(home, fullChordsVoicing[currentPosition * nVoices + i] ==
                                fullChordsVoicing[(currentPosition + 1) * nVoices + i]), true);
            }
        }
    }
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

    /// If the leading tone is in the soprano, it must rise to the tonic regardless of the mode
    rel(home, expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE ==
            tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
        expr(home, sopranoMelodicInterval[currentPosition] ==1 ), true);
    /// other voices must go down
    rel(home, expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE ==
            tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
        expr(home, tenorMelodicInterval[currentPosition] < 0), true);
    rel(home, expr(home, fullChordsVoicing[currentPosition +  SOPRANO] % PERFECT_OCTAVE ==
            tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
        expr(home, altoMelodicInterval[currentPosition] < 0), true);

    /// If the mode is minor, then the leading tone always has to rise to the tonic
    if(tonality->get_mode() == MINOR_MODE){
        // tenor note is the seventh of the scale
        // -> that voice must raise to the tonic by a minor second
        rel(home, expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE ==
                tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
            expr(home, tenorMelodicInterval[currentPosition] ==1 ), true);
        //other voices must go down
        rel(home, expr(home, fullChordsVoicing[currentPosition +  TENOR] % PERFECT_OCTAVE ==
                tonality->get_degree_note(SEVENTH_DEGREE)),BOT_IMP,
            expr(home, altoMelodicInterval[currentPosition] < 0), true);
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
void fifth_degree_appogiatura(Home home, int nVoices, int currentPosition, Tonality *tonality, IntVarArray fullChordsVoicing,
                              IntVarArray bassMelodicInterval, IntVarArray tenorMelodicInterval,
                              IntVarArray altoMelodicInterval, IntVarArray sopranoMelodicInterval){
    BoolVar bassRises(home, 0,1);/// = 1 if the bass goes up, 0 if it goes down
    rel(home, expr(home,bassMelodicInterval[currentPosition] < 0), BOT_EQV,expr(home, !bassRises), true);
    rel(home, expr(home,bassMelodicInterval[currentPosition] >= 0), BOT_EQV,bassRises, true);

    vector<IntVarArray> melodicIntervals({bassMelodicInterval, tenorMelodicInterval, altoMelodicInterval, sopranoMelodicInterval});

    for(int voice = TENOR; voice <= SOPRANO; ++voice){
        /// if voice is playing the tonic and the bass rises, this voice must go down or stay the same
        rel(home,expr(home,fullChordsVoicing[(currentPosition + 1) * nVoices + voice] % 12 == tonality->get_tonic() % 12),
            BOT_IMP,
            expr(home,melodicIntervals[voice][currentPosition] <= 0), bassRises);
        /// if voice is playing the tonic and the bass goes down, this voice must go up or stay the same
        rel(home,expr(home,fullChordsVoicing[(currentPosition + 1) * nVoices + voice] % 12 == tonality->get_tonic() % 12),
            BOT_IMP,
            expr(home,melodicIntervals[voice][currentPosition] >= 0), expr(home, !bassRises));
    }
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                         Constraints independent of chord state                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Forces the tritone to resolve properly
 * @param home the instance of the problem
 * @param nvoices the number of voices in the piece
 * @param currentPosition the current position in the chord progression
 * @param tonality the tonality of the piece
 * @param chordDegs the degrees of the chords
 * @param chordStas the state of the chords
 * @param bassMelodicInterval the melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void tritone_resolution(const Home &home, int nVoices, int currentPosition, Tonality *tonality, vector<int> chordDegs,
                        vector<int> chordStas, const IntVarArray &bassMelodicInterval,
                        const IntVarArray &tenorMelodicInterval, const IntVarArray &altoMelodicInterval,
                        const IntVarArray &sopranoMelodicInterval, IntVarArray fullChordsVoicing) {

    IntVarArgs currentChord(fullChordsVoicing.slice(nVoices * currentPosition, 1, nVoices));

    vector<IntVarArray> melodicIntervals(
            {bassMelodicInterval, tenorMelodicInterval, altoMelodicInterval, sopranoMelodicInterval});
    for(int voice = BASS; voice <= SOPRANO; voice++){
        /// special case
        /// if the chords are VII (1st inversion) -> I (1st inversion)
        if (chordDegs[currentPosition] == SEVENTH_DEGREE && chordStas[currentPosition] == FIRST_INVERSION &&
            chordDegs[currentPosition+1] == FIRST_DEGREE && chordStas[currentPosition+1] == FIRST_INVERSION){
            /// the fourth of the scale must go up by a step
            rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + PERFECT_FOURTH) %
            PERFECT_OCTAVE), BOT_IMP, expr(home, expr(home, melodicIntervals[voice][currentPosition] > 0) &&
            expr(home, melodicIntervals[voice][currentPosition] <= 2)), true);
        }
        else{ /// the fourth of the scale must go down by a step
            rel(home,expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + PERFECT_FOURTH) %
                PERFECT_OCTAVE),BOT_IMP,expr(home, expr(home, melodicIntervals[voice][currentPosition] < 0)
                && expr(home, melodicIntervals[voice][currentPosition] >= -2)), true);
        }

        /// if the note is the major seventh of the scale, it must go up to the tonic by step
        rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + MAJOR_SEVENTH) % PERFECT_OCTAVE),
            BOT_IMP, expr(home, melodicIntervals[voice][currentPosition] == 1), true); /// must move up by a semitone to (the tonic)
    }
}

