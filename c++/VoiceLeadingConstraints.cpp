#include "headers/VoiceLeadingConstraints.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 * This file contains all the voice leading constraints, that is constraints on how voices move through successive     *
 * chords.                                                                                                             *
 * It currently contains the following constraints:                                                                    *
 *      - forbid_parallel_intervals: forbids a given parallel interval between two voices                              *
 *      - fundamental_state_chord_to_fundamental_state_chord: sets the rules for the melodic movements between chords in     *
 *        fundamental state                                                                                            *
 *      - fifth_degree_fs_to_sixth_degree_fs: sets the constraint for a fifth degree followed by a sixth degree in funda-    *
 *        mental state                                                                                               *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Forbids a given parallel interval between two voices
 * @todo make it with argument variables + make it cleaner
 * @param home the instance of the problem
 * @param forbiddenParallelInterval the interval that must not be parallel
 * @param currentPosition the current position in the chord progression
 * @param voice1ID the ID of the first voice
 * @param voice2ID the ID of the second voice
 * @param voicesHarmonicIntervals the array containing the harmonic intervals between adjacent voices
 * @param FullChordsVoicing the array containing all the notes of the chords in the progression
 */
void forbid_parallel_intervals(Home home, int forbiddenParallelInterval, int currentPosition, int voice1ID, int voice2ID,
                               IntVarArray voicesHarmonicIntervals, IntVarArray FullChordsVoicing){

    /// harmonicIntervalForbidden is true if the interval is forbiddenParallelInterval
    BoolVar harmonicIntervalForbidden(home, 0, 1);
    rel(home, harmonicIntervalForbidden, IRT_EQ, expr(home, voicesHarmonicIntervals[currentPosition] %
                                                            PERFECT_OCTAVE == forbiddenParallelInterval));

    /// notesLowerVoiceEqual is true if voice1 is the same note in both chords
    BoolVar notesLowerVoiceEqual(home, 0, 1);
    rel(home, notesLowerVoiceEqual, IRT_EQ, expr(home, FullChordsVoicing[currentPosition * 4 + voice1ID]
                                                       == FullChordsVoicing[(currentPosition + 1) *4 + voice1ID]));

    /// notesUpperVoiceEqual is true if voice2 is the same note in both chords
    BoolVar notesUpperVoiceEqual(home,0,1);
    rel(home, notesUpperVoiceEqual, IRT_EQ, expr(home, FullChordsVoicing[currentPosition * 4 + voice2ID]
                                                       == FullChordsVoicing[(currentPosition + 1) *4 + voice2ID]));

    /// bothVoicesEqual is true if both voices are the same in both chords
    BoolVar bothVoicesEqual(home, 0, 1);
    rel(home, notesLowerVoiceEqual, BOT_AND, notesUpperVoiceEqual, bothVoicesEqual);

    /// nextIntervalNotParallel is true if the harmonic interval between the voices is not forbidden
    BoolVar nextIntervalNotParallel(home, 0,1);
    rel(home, nextIntervalNotParallel, IRT_EQ, expr(home, voicesHarmonicIntervals[currentPosition+1] %
                                                          PERFECT_OCTAVE != forbiddenParallelInterval));
    /// isNextIntervalValid is true if the next interval is not parallel or if the both voices are equal in the 2 chords
    BoolVar isNextIntervalValid(home,0,1);
    rel(home, bothVoicesEqual, BOT_OR, nextIntervalNotParallel, isNextIntervalValid); // ok

    rel(home, harmonicIntervalForbidden, BOT_IMP, isNextIntervalValid, true);

}

void general_voice_leading_rules(const Home& home, int currentPosition, vector<int> chordDegrees,
                                 Tonality *tonality,
                                 const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                 const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                 IntVarArray fullChordsVoicing){

    int degreeDifference = abs(chordDegrees[currentPosition+1] - chordDegrees[currentPosition]); // interval between the chords

    /// keep common notes in the same voice
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
    for(auto it : thisChord){ // for each note in the current chord domain
        if(nextChord.find(it) != nextChord.end()){ // if the note is in the next chord as well
            for(int i = TENOR; i <= SOPRANO; ++i){ // for all voices except the bass
                // the note in the current chord in the voice i must be the same in the next chord, @todo make it both ways?
                rel(home, expr(home, fullChordsVoicing[currentPosition * 4 + i] % 12 == it), BOT_IMP,
                    expr(home, fullChordsVoicing[(currentPosition + 1) * 4 + i] ==
                               fullChordsVoicing[currentPosition * 4 + i]), true);
            }
        }
    }
}

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                          Fundamental state chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

/**
 * Sets the rules for the melodic movements between chords in fundamental state
 * For chords that are 1 degree apart, the other voices must move in contrary motion to the bass
 * @param home the instance of the problem
 * @param currentPosition the current position in the chord progression
 * @param chordDegrees the array containing the degrees of the chords in the progression
 * @param tonality the tonality of the piece
 * @param bassMelodicInterval The melodic interval of the bass between the current position and the next
 * @param tenorMelodicInterval the melodic interval of the tenor between the current position and the next
 * @param altoMelodicInterval the melodic interval of the alto between the current position and the next
 * @param sopranoMelodicInterval the melodic interval of the soprano between the current position and the next
 * @param fullChordsVoicing the array containing all the notes of the chords in the progression
 */
void fundamental_state_chord_to_fundamental_state_chord(const Home& home, int currentPosition, vector<int> chordDegrees,
                                                        Tonality *tonality,
                                                        const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                                        const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                                        IntVarArray fullChordsVoicing){

    int degreeDifference = abs(chordDegrees[currentPosition+1] - chordDegrees[currentPosition]); // interval between the chords

    /// First, corner cases that differ from the general rules
    if(chordDegrees[currentPosition] == FIFTH_DEGREE && chordDegrees[currentPosition+1] == SIXTH_DEGREE){
        fifth_degree_fs_to_sixth_degree_fs(home, currentPosition, tonality, tenorMelodicInterval, altoMelodicInterval,
                                           sopranoMelodicInterval, fullChordsVoicing);
    }
    else if(chordDegrees[currentPosition] == SEVENTH_DEGREE && chordDegrees[currentPosition + 1] == FIRST_DEGREE){
        /// tritone resolution
        tritone_resolution(home, currentPosition, tonality, bassMelodicInterval, tenorMelodicInterval,
                           altoMelodicInterval,
                           sopranoMelodicInterval, fullChordsVoicing);
    }
    /// general cases
    else if(degreeDifference == 1){ // the chords are 1 degree apart
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
    else{ /// there is at least one common note in the 2 chords -> keep that (these) notes in the same voices and move the others to the closest note
        general_voice_leading_rules(home, currentPosition, chordDegrees, tonality, bassMelodicInterval,
                                    tenorMelodicInterval, altoMelodicInterval, sopranoMelodicInterval,
                                    fullChordsVoicing);
    }
}
/// @todo change this so we post a constraint on the melodic intervals rather than the note, because here it can go up a seventh instead of down a second for example
void tritone_resolution(Home home, int currentPosition, Tonality *tonality, IntVarArray bassMelodicInterval,
                        IntVarArray tenorMelodicInterval, IntVarArray altoMelodicInterval, IntVarArray sopranoMelodicInterval,
                        IntVarArray fullChordsVoicing){

    IntVarArgs currentChord(fullChordsVoicing.slice(4 * currentPosition, 1, 4));
    IntVarArgs nextChord(fullChordsVoicing.slice(4 * (currentPosition + 1), 1, 4));

    vector<IntVarArray> melodicIntervals({bassMelodicInterval, tenorMelodicInterval, altoMelodicInterval, sopranoMelodicInterval});

    for(int voice = BASS; voice <= SOPRANO; voice++){
        /// if the note is the fourth of the scale, it must go to the third of the scale
        rel(home,expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + PERFECT_FOURTH) % PERFECT_OCTAVE),
            BOT_IMP, expr(home, melodicIntervals[voice][currentPosition] < 0), true); /// must be descending
        rel(home,expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + PERFECT_FOURTH) % PERFECT_OCTAVE),
            BOT_IMP, expr(home, melodicIntervals[voice][currentPosition] >= -2), true); /// must be a second
        /// if the note is the seventh of the scale, it must go to the tonic
        rel(home, expr(home, currentChord[voice] % PERFECT_OCTAVE == (tonality->get_tonic() + MAJOR_SEVENTH) % PERFECT_OCTAVE),
            BOT_IMP, expr(home, melodicIntervals[voice][currentPosition] == 1), true); /// must move up by a semitone to (the tonic)
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
void fifth_degree_fs_to_sixth_degree_fs(const Home& home, int currentPosition, Tonality *tonality,
                                        const IntVarArray& tenorMelodicInterval, const IntVarArray& altoMelodicInterval,
                                        const IntVarArray& sopranoMelodicInterval, IntVarArray fullChordsVoicing){

    // @todo make it cleaner with loops

    /// if the mode is major, then this rule only applies to the soprano voice. Otherwise, it applies for all voices

    /// soprano note is the seventh of the scale
    /// -> that voice must raise to the tonic by a minor second
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
 *                                            First inversion chord constraints                                        *
 *                                                                                                                     *
 ***********************************************************************************************************************/

void from_first_inversion_chord(const Home& home, int currentPosition, vector<int> chordDegrees,
                                Tonality *tonality,
                                const IntVarArray& bassMelodicInterval, const IntVarArray& tenorMelodicInterval,
                                const IntVarArray& altoMelodicInterval, const IntVarArray& sopranoMelodicInterval,
                                IntVarArray fullChordsVoicing){
    /// exceptions
    /// II 1st inv. to V -> contrary motion to the bass
    /// other voices need to move by contrary motion to the bass
    if(chordDegrees[currentPosition] == SECOND_DEGREE && chordDegrees[currentPosition + 1] == FIFTH_DEGREE){
        //@todo make it into a separate function because it is used multiple times and right now the code is duplicated
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
    else{ /// general case
        general_voice_leading_rules(home, currentPosition, chordDegrees, tonality, bassMelodicInterval,
                                    tenorMelodicInterval, altoMelodicInterval, sopranoMelodicInterval,
                                    fullChordsVoicing);
    }
}