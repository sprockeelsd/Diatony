// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A class that creates a constraint representation of a four voice harmonic problem of tonal music.
//

#include "../../headers/diatony/TonalProgression.hpp"

#include "../../headers/diatony/FourVoiceTexture.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                                                                                     *
 *                                             FourVoiceTexture class methods                                          *
 *                                                                                                                     *
 *                                                                                                                     *
 ***********************************************************************************************************************/
/**
 * This class models a classic 4-voice harmonic problem of tonal music. It takes as arguments a number of chords, a
 * tonality, and a series of chords identified by their degree, quality and state. It then generates a 4-voice chord
 * progression following traditional rules of western tonal harmony.
 */

    /**
     * Constructor
     * @param home the space of the problem
     * @param params an object containing the parameters of the problem
     * @param fullVoicing the general array for the voicing of the whole piece
     * @param bassIntervals the melodic intervals for the bass voice
     * @param tenorIntervals the melodic intervals for the tenor voice
     * @param altoIntervals the melodic intervals for the alto voice
     * @param sopranoIntervals the melodic intervals for the soprano voice
     * @param allMIntervals the melodic intervals for all voices
     * @param bassTenorIntervals the harmonic intervals between the bass and tenor voices
     * @param bassAltoIntervals the harmonic intervals between the bass and alto voices
     * @param bassSopranoIntervals the harmonic intervals between the bass and soprano voices
     * @param tenorAltoIntervals the harmonic intervals between the tenor and alto voices
     * @param tenorSopranoIntervals the harmonic intervals between the tenor and soprano voices
     * @param altoSopranoIntervals the harmonic intervals between the alto and soprano voices
     * @param nDifferentValuesInDimChord the number of different values in diminished chords
     * @param nDNotesInChords the number of different notes in each chord
     * @param nIncompleteChords the number of incomplete chords in the progression
     * @return an object constraining the variables on which the problem is defined
     * /!\ dominant diminished seventh chords are considered as minor ninth dominant chords without their fundamental
     */
    TonalProgression::TonalProgression(Home home, TonalProgressionParameters* params,
        IntVarArray& fullVoicing,                   IntVarArray& bassIntervals,         IntVarArray& tenorIntervals,
        IntVarArray& altoIntervals,                 IntVarArray& sopranoIntervals,      IntVarArray& allMIntervals,
        IntVarArray& bassTenorIntervals,            IntVarArray& bassAltoIntervals,     IntVarArray& bassSopranoIntervals,
        IntVarArray& tenorAltoIntervals,            IntVarArray& tenorSopranoIntervals, IntVarArray& altoSopranoIntervals,
        IntVarArray& nDifferentValuesInDimChord,    IntVarArray& nDNotesInChords,       IntVar& nIncompleteChords) : params(params){

    if (params->get_size() != (params->get_end() - params->get_start()) + 1)
        throw std::runtime_error("TonalProgression: the length is not coherent with the start and end positions. length: "
             + std::to_string(params->get_size()) + ", start: " + std::to_string(params->get_start())
             + ", end: " + std::to_string(params->get_end()));

    /// keep track of the number of notes that should be in each chord if it is complete
    nOfNotesInChord  = IntArgs(params->get_size());
    for(int i = 0; i < params->get_size(); i++)
        nOfNotesInChord[i] = static_cast<int>(chordQualitiesIntervals.at(params->get_chordQualities()[i]).size()) + 1;

    /// solution array
    //take a subset of the general piece array based on this section's start and end
    voicing = IntVarArray(home, fullVoicing.slice(params->get_start()*nVoices, 1, params->get_size()*nVoices));

    /// take a subset of the melodic intervals arrays based on this section's start and end
    bassMelodicIntervals        = IntVarArray(home, bassIntervals   .slice(params->get_start(), 1, params->get_size() - 1));
    tenorMelodicIntervals       = IntVarArray(home, tenorIntervals  .slice(params->get_start(), 1, params->get_size() - 1));
    altoMelodicIntervals        = IntVarArray(home, altoIntervals   .slice(params->get_start(), 1, params->get_size() - 1));
    sopranoMelodicIntervals     = IntVarArray(home, sopranoIntervals.slice(params->get_start(), 1, params->get_size() - 1));

    allMelodicIntervals         = IntVarArray(home, allMIntervals   .slice(params->get_start() * nVoices, 1, nVoices* (params->get_size() - 1)));

    /// take a subset of the harmonic intervals arrays based on this section's start and end
    bassTenorHarmonicIntervals          = IntVarArray(home, bassTenorIntervals      .slice(params->get_start(), 1, params->get_size()));
    bassAltoHarmonicIntervals           = IntVarArray(home, bassAltoIntervals       .slice(params->get_start(), 1, params->get_size()));
    bassSopranoHarmonicIntervals        = IntVarArray(home, bassSopranoIntervals    .slice(params->get_start(), 1, params->get_size()));
    tenorAltoHarmonicIntervals          = IntVarArray(home, tenorAltoIntervals      .slice(params->get_start(), 1, params->get_size()));
    tenorSopranoHarmonicIntervals       = IntVarArray(home, tenorSopranoIntervals   .slice(params->get_start(), 1, params->get_size()));
    altoSopranoHarmonicIntervals        = IntVarArray(home, altoSopranoIntervals    .slice(params->get_start(), 1, params->get_size()));

    /// cost variables auxiliary arrays
    nDifferentValuesInDiminishedChord       = IntVarArray(home, nDifferentValuesInDimChord  .slice(params->get_start(), 1, params->get_size()));
    noFDifferentNotesInChords               = IntVarArray(home, nDNotesInChords             .slice(params->get_start(), 1, params->get_size()));

    nOfUnisons                              = IntVar(home, 0, nVoices * (params->get_size() - 1));
    count(home, allMelodicIntervals, UNISON, IRT_EQ, nOfUnisons);

    /// cost variables
    nOfIncompleteChords                             = IntVar(nIncompleteChords);

    /// Test constraints


    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                             set up costs computation                                             |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    // @todo add a cost for doubled notes that are not tonal notes -> if a value is not in the tonal notes (1-(2)-4-5), then its occurrence cannot be greater than 1 for each chord

    /// number of diminished chords in fundamental state with more than 3 notes (cost to minimize)
    compute_diminished_chords_cost(home, 4, params->get_size(), params->get_chordStates(),
        params->get_chordQualities(), fullVoicing, nDifferentValuesInDiminishedChord);

    /// number of chords that don't have all their possible note values (cost to minimize)
    compute_cost_for_incomplete_chords(home, nVoices, params->get_size(), nOfNotesInChord, voicing,
        noFDifferentNotesInChords, nOfIncompleteChords);

    /**----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                              generic constraints                                                 |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

     for(int i = 0; i < params->get_size(); i++) {
         IntVarArgs currentChord(voicing.slice(nVoices * i, 1, nVoices));

         //set the chord's domain to the notes of the degree chord_degrees[i]'s chord with the right quality
         set_to_chord(home, params->get_tonality(), params->get_chordDegrees()[i],
             params->get_chordQualities()[i], currentChord);

         //set the bass based on the chord's state
         set_bass(home, params->get_tonality(), params->get_chordDegrees()[i], params->get_chordQualities()[i],
             params->get_chordStates()[i], currentChord);
    }

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |     Harmonic constraints: loop over each chord and post the constraints depending on the degree and state        |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    for(int i = 0; i < params->get_size(); i++){
        IntVarArgs currentChord(voicing.slice(nVoices * i, 1, nVoices));

        /// post the constraints depending on the chord's state
        if(params->get_chordStates()[i] == FUNDAMENTAL_STATE){
            chord_note_occurrence_fundamental_state(home, nVoices, i, params->get_chordDegrees(),
                params->get_chordQualities(), params->get_tonality(), currentChord,
                nDifferentValuesInDiminishedChord[i],
                noFDifferentNotesInChords[i % nVoices]);
        }
        else if(params->get_chordStates()[i] == FIRST_INVERSION){
            chord_note_occurrence_first_inversion(home, params->get_size(), nVoices, i, params->get_tonality(),
            params->get_chordDegrees(), params->get_chordQualities(), currentChord, bassMelodicIntervals,
            sopranoMelodicIntervals);
        }
        else if(params->get_chordStates()[i] == SECOND_INVERSION){
            chord_note_occurrence_second_inversion(home, params->get_size(), nVoices, i, params->get_tonality(),
            params->get_chordDegrees(), params->get_chordQualities(), currentChord);
        }
        else if (params->get_chordStates()[i] == THIRD_INVERSION){
            chord_note_occurrence_third_inversion(home, params->get_size(), nVoices, i, params->get_tonality(),
            params->get_chordDegrees(), params->get_chordQualities(), currentChord);
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
    for(int i = 0; i < params->get_size()-1; i++) {
        /// parallel unissons, fifths and octaves are forbidden unless we have the same chord twice in a row
        if(params->get_chordDegrees()[i] != params->get_chordDegrees()[i + 1]){
            /// @todo maybe do it also <--- so that it can propagate in both directions, if the harmonic interval is a
            /// @todo perfect fifth or octave the previous and next chords can't
            forbid_parallel_intervals(home, params->get_size(), nVoices, {PERFECT_FIFTH, PERFECT_OCTAVE, UNISON},
                voicing, bassTenorHarmonicIntervals, bassAltoHarmonicIntervals, bassSopranoHarmonicIntervals,
                tenorAltoHarmonicIntervals, tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);
        }
        /// resolve the tritone in the following cases:
        /// VII° ->I, V->I, V/... ->...
        if ((params->get_chordDegrees()[i] == SEVENTH_DEGREE && params->get_chordQualities()[i] == DIMINISHED_CHORD && params->get_chordDegrees()[i + 1] == FIRST_DEGREE)
        || ( params->get_chordDegrees()[i] == FIFTH_DEGREE   && params->get_chordDegrees()[i+1] == FIRST_DEGREE)
        || ((params->get_chordDegrees()[i] >= FIVE_OF_TWO    && params->get_chordDegrees()[i] <= FIVE_OF_SEVEN)     && params->get_chordDegrees()[i+1] != FIFTH_DEGREE_APPOGIATURA)){
            //@todo add other chords that have the tritone
            tritone_resolution(home, nVoices, i, params->get_tonality(), params->get_chordDegrees(),
            params->get_chordQualities(), params->get_chordStates(), bassMelodicIntervals,
            tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals, voicing);
        }
        /// Exceptions to the general voice leading rules

        /// special rule for interrupted cadence (V -> VI)
        if (params->get_chordDegrees()[i] == FIFTH_DEGREE && params->get_chordStates()[i] == FUNDAMENTAL_STATE &&
        params->get_chordDegrees()[i + 1] == SIXTH_DEGREE && params->get_chordStates()[i + 1] == FUNDAMENTAL_STATE) {
            interrupted_cadence(home, i, params->get_tonality(), voicing, tenorMelodicIntervals,
            altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// special rules for augmented sixth chords
        else if (params->get_chordDegrees()[i] == AUGMENTED_SIXTH) {
            italian_augmented_sixth(home, nVoices, i, params->get_tonality(), voicing,
                bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// special rule for the fifth degree appogiatura
        else if(params->get_chordDegrees()[i] == FIRST_DEGREE && params->get_chordStates()[i] == SECOND_INVERSION &&
        params->get_chordDegrees()[i+1] == FIFTH_DEGREE && (params->get_chordQualities()[i] == MAJOR_CHORD ||
        params->get_chordQualities()[i] == DOMINANT_SEVENTH_CHORD)){
            fifth_degree_appogiatura(home, nVoices, i, params->get_tonality(), voicing,
                bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// general voice leading rules
        else {
            // Species seventh chords
            if ((params->get_chordQualities()[i+1] == MAJOR_SEVENTH_CHORD       || params->get_chordQualities()[i+1] == MINOR_SEVENTH_CHORD ||
                params->get_chordQualities()[i+1] == DIMINISHED_SEVENTH_CHORD   || params->get_chordQualities()[i+1] == HALF_DIMINISHED_CHORD)
                && params->get_chordQualities()[i+1] <= SEVENTH_DEGREE) {
                /// the seventh must be prepared
                species_seventh(home, nVoices, i, params->get_tonality(), params->get_chordDegrees(), params->get_chordQualities(), voicing);
            }

            /// If the bass moves by a step, other voices should move in contrary motion
            const int bassFirstChord = (params->get_tonality()->get_degree_note(params->get_chordDegrees()[i] + 2 * params->get_chordStates()[i]) % PERFECT_OCTAVE);
            const int bassSecondChord = (params->get_tonality()->get_degree_note(params->get_chordDegrees()[i + 1] + 2 * params->get_chordStates()[i + 1]) % PERFECT_OCTAVE);
            const int bassMelodicMotion = abs(bassSecondChord - bassFirstChord);

            if ((bassMelodicMotion == MINOR_SECOND || bassMelodicMotion == MAJOR_SECOND ||
                bassMelodicMotion == MINOR_SEVENTH || bassMelodicMotion == MAJOR_SEVENTH) &&
                params->get_chordStates()[i] == FUNDAMENTAL_STATE && params->get_chordStates()[i+1] == FUNDAMENTAL_STATE){
                /// move other voices in contrary motion
                contrary_motion_to_bass(home, i,bassMelodicIntervals, tenorMelodicIntervals,altoMelodicIntervals, sopranoMelodicIntervals);
            }
            /// if II -> V, move voices in contrary motion to bass
            else if(params->get_chordDegrees()[i] == SECOND_DEGREE && params->get_chordDegrees()[i+1] == FIFTH_DEGREE){
                contrary_motion_to_bass(home, i, bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals);
            }
            else if(params->get_chordDegrees()[i] != params->get_chordDegrees()[i + 1]){
                /// Otherwise, keep common notes in the same voice whenever possible (cost to minimize)
            }
        }
    }

}

/**
 * Copy constructor
 * @param home the space of the problem
 * @param s an instance of the TonalProgression class
 * @return a copy of the given instance of the TonalProgression class
 * /!\ It is important to copy every variable instance variable of the given instance to the new instance
 */
TonalProgression::TonalProgression(Home home, TonalProgression& s) : params(s.params) {
    nOfNotesInChord = s.nOfNotesInChord;

    bassMelodicIntervals.update(home, s.bassMelodicIntervals);
    tenorMelodicIntervals.update(home, s.tenorMelodicIntervals);
    altoMelodicIntervals.update(home, s.altoMelodicIntervals);
    sopranoMelodicIntervals.update(home, s.sopranoMelodicIntervals);
    allMelodicIntervals.update(home, s.allMelodicIntervals);

    bassTenorHarmonicIntervals.update(home, s.bassTenorHarmonicIntervals);
    bassAltoHarmonicIntervals.update(home, s.bassAltoHarmonicIntervals);
    bassSopranoHarmonicIntervals.update(home, s.bassSopranoHarmonicIntervals);
    tenorAltoHarmonicIntervals.update(home, s.tenorAltoHarmonicIntervals);
    tenorSopranoHarmonicIntervals.update(home, s.tenorSopranoHarmonicIntervals);
    altoSopranoHarmonicIntervals.update(home, s.altoSopranoHarmonicIntervals);

    voicing.update(home, s.voicing);

    nDifferentValuesInDiminishedChord.update(home, s.nDifferentValuesInDiminishedChord);
    noFDifferentNotesInChords.update(home, s.noFDifferentNotesInChords);

    nOfUnisons.update( home, s.nOfUnisons);

    nOfIncompleteChords.update(home, s.nOfIncompleteChords);
}

/**
 * Returns the values taken by the variables vars in a solution as a pointer to an integer array
 * @return an array of integers representing the values of the variables in a solution
 */
int* TonalProgression::return_solution() const{
    int* solution = new int[params->get_size()*4];
    for(int i = 0; i < 4*params->get_size(); i++){
        solution[i] = voicing[i].val();
    }
    return solution;
}

/**
 * returns the parameters in a string
 * @return a string containing the parameters of the problem
 */
string TonalProgression::parameters() const {
    return params->to_string();
}

/**
 * to_string method
 * @return A string representation of the current instance of the FourVoiceTexture class.
 * Right now, it returns a string "FourVoiceTexture object. Size = <size>"
 * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
 */
string TonalProgression::to_string() const {
    string message;
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
    message += "fullChordsVoicing = " + intVarArray_to_string(voicing) + "\n\n";
    message += "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵"
               "🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵🎵\n\n";

    message += "-------------------------------cost-related auxiliary arrays------------------------------\n";

    message += "nDifferentValuesInDiminishedChord = \t" + intVarArray_to_string(nDifferentValuesInDiminishedChord) + "\n";
    message += "noFDifferentNotesInChords = \t\t" + intVarArray_to_string(noFDifferentNotesInChords) + "\n";

    message += "nOfUnisons = \t\t" + intVar_to_string(nOfUnisons) + "\n\n";

    message += "------------------------------------cost variables----------------------------------------\n";

    message += "nOfIncompleteChords = " + intVar_to_string(nOfIncompleteChords) + "\n";
    return message;
}
