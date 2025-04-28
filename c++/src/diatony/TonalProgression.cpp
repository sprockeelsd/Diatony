// Programmer:    Damien Sprockeels <damien.sprockeels@uclouvain.be>
// Creation Date: Jan 20 2023
// Description:   A class that creates a constraint representation of a four voice harmonic problem of tonal music.
//
#include <utility>

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
 * This class models a classic 4 voice harmonic problem of tonal music. It takes as arguments a number of chords, a
 * tonality, and a series of chords identified by their degree, quality and state. It then generates a 4 voice chord
 * progression following traditional rules of western tonal harmony.
 */

/**
 * Constructor
 * @param home the space of the problem
 * @param params an object containing the parameters of the problem
 * @param fullVoicing the general array for the voicing of the whole piece
 * @param bassIntervals
 * @param tenorIntervals
 * @param altoIntervals
 * @param sopranoIntervals
 * @param allMIntervals
 * @param nDifferentValuesInDimChord
 * @return an object constraining the variables on which the problem is defined
 * /!\ dominant diminished seventh chords are considered as minor ninth dominant chords without their fundamental
 */
TonalProgression::TonalProgression(Home home, TonalProgressionParameters* params,
        IntVarArray& fullVoicing, IntVarArray& bassIntervals, IntVarArray& tenorIntervals,
        IntVarArray& altoIntervals, IntVarArray& sopranoIntervals, IntVarArray& allMIntervals,
        IntVarArray& nDifferentValuesInDimChord) : params(params){

    if (params->get_size() != (params->get_end() - params->get_start()) + 1)
        throw std::runtime_error("TonalProgression: the length is not coherent with the start and end positions");

    //todo: support seventh chords for all chord degrees + diminished seventh chords

    /// keep track of the number of notes that should be in each chord if it is complete
    nOfNotesInChord  = IntArgs(params->get_size());
    for(int i = 0; i < params->get_size(); i++)
        nOfNotesInChord[i] = static_cast<int>(chordQualitiesIntervals.at(params->get_chordQualities()[i]).size()) + 1;

    /// solution array
    //take a subset of the general piece array based on this section's start and end
    voicing = IntVarArray(home, fullVoicing.slice(params->get_start()*nVoices, 1, params->get_size()*nVoices));

    /// variable arrays for melodic intervals for each voice (max an octave both ways)
    bassMelodicIntervals        = IntVarArray(home, bassIntervals   .slice(params->get_start(), 1, params->get_size() - 1));
    tenorMelodicIntervals       = IntVarArray(home, tenorIntervals  .slice(params->get_start(), 1, params->get_size() - 1));
    altoMelodicIntervals        = IntVarArray(home, altoIntervals   .slice(params->get_start(), 1, params->get_size() - 1));
    sopranoMelodicIntervals     = IntVarArray(home, sopranoIntervals.slice(params->get_start(), 1, params->get_size() - 1));

    allMelodicIntervals         = IntVarArray(home, allMIntervals   .slice(params->get_start() * nVoices, 1, nVoices* (params->get_size() - 1)));

    /// variable arrays for harmonic intervals between adjacent voices (only positive because there is no direction)
    bassTenorHarmonicIntervals                      = IntVarArray(home, params->get_size(), 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    bassAltoHarmonicIntervals                       = IntVarArray(home, params->get_size(), 0, 2 * PERFECT_OCTAVE + PERFECT_FIFTH);
    bassSopranoHarmonicIntervals                    = IntVarArray(home, params->get_size(), 0, 3 * PERFECT_OCTAVE + PERFECT_FIFTH);
    tenorAltoHarmonicIntervals                      = IntVarArray(home, params->get_size(), 0, PERFECT_OCTAVE);
    tenorSopranoHarmonicIntervals                   = IntVarArray(home, params->get_size(), 0, 2 * PERFECT_OCTAVE);
    altoSopranoHarmonicIntervals                    = IntVarArray(home, params->get_size(), 0, PERFECT_OCTAVE);

    link_harmonic_arrays(home, nVoices, params->get_size(), voicing,
                 bassTenorHarmonicIntervals, bassAltoHarmonicIntervals, bassSopranoHarmonicIntervals,
                 tenorAltoHarmonicIntervals, tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);

    /// cost variables auxiliary arrays
    nDifferentValuesInDiminishedChord               = IntVarArray(home, nDifferentValuesInDimChord.slice(params->get_start(), 1, params->get_size()));
    nDifferentValuesAllChords                       = IntVarArray(home, params->get_size(), 0, nVoices);
    nOFDifferentNotesInChords                       = IntVarArray(home, params->get_size(), 0, nVoices);
    commonNotesInSameVoice                          = IntVarArray(home, nVoices, 0, params->get_size() - 1);

    nOfUnisons                                     = IntVar(home, 0, nVoices * (params->get_size() - 1));
    count(home, allMelodicIntervals, UNISSON, IRT_EQ, nOfUnisons);

    /// cost variables
    nOfChordsWithLessThan4Values                    = IntVar(home, 0, params->get_size());
    nOfIncompleteChords                             = IntVar(home, 0, params->get_size());
    nOfCommonNotesInSameVoice                       = IntVar(home, - nVoices * (params->get_size() - 1), 0);

    /// Test constraints


    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                             set up costs computation                                             |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    // @todo add a cost for doubled notes that are not tonal notes -> if a value is not in the tonal notes (1-(2)-4-5), then its occurrence cannot be greater than 1 for each chord

    /// number of chords with less than 4 note values (cost to minimize)
    compute_n_of_notes_in_chord_cost(home, nVoices, params->get_size(), voicing,
                                     nDifferentValuesAllChords, nOfChordsWithLessThan4Values);

    /// number of chords that don't have all their possible note values (cost to minimize)
    compute_cost_for_incomplete_chords(home, nVoices, params->get_size(), nOfNotesInChord,
                                       voicing, nOFDifferentNotesInChords, nOfIncompleteChords);

    /// count the number of common notes in the same voice between consecutive chords (cost to MAXIMIZE)
    /// /!\ The variable nOfCommonNotesInSameVoice has a NEGATIVE value so the minimization will maximize its absolute value
    compute_cost_for_common_notes_not_in_same_voice(home, bassMelodicIntervals, tenorMelodicIntervals,
                                                    altoMelodicIntervals, sopranoMelodicIntervals, nOfUnisons,
                                                    commonNotesInSameVoice,
                                                    nOfCommonNotesInSameVoice);

    /**-----------------------------------------------------------------------------------------------------------------
    |                                                                                                                  |
    |                                              generic constraints                                                 |
    |                                                                                                                  |
    -------------------------------------------------------------------------------------------------------------------*/

    ///todo check the constraint for Vda again with the new representation where it is its own chord!
    ///todo check that all constraints still work as expected with the new additions

    /// restrain the domain of the voices to their range + state that bass <= tenor <= alto <= soprano
    restrain_voices_domains(home, nVoices, params->get_size(),
                            {BASS_MIN, TENOR_MIN, ALTO_MIN, SOPRANO_MIN},
                            {BASS_MAX, TENOR_MAX, ALTO_MAX, SOPRANO_MAX},
                            voicing);

     for(int i = 0; i < params->get_size(); i++) {
         IntVarArgs currentChord(voicing.slice(nVoices * i, 1, nVoices));

         //set the chord's domain to the notes of the degree chordDegrees[i]'s chord with the right quality
         set_to_chord(home, params->get_tonality(), params->get_chordDegrees()[i], params->get_chordQualities()[i], currentChord);

         //set the bass based on the chord's state
         set_bass(home, params->get_tonality(), params->get_chordDegrees()[i], params->get_chordQualities()[i], params->get_chordStates()[i], currentChord);
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
            /// each note should be present at least once, doubling is determined with costs
            chord_note_occurrence_fundamental_state(home, nVoices, i, params->get_chordDegrees(),
                                                    params->get_chordQualities(), params->get_tonality(), currentChord,
                                                    nDifferentValuesInDiminishedChord[i],
                                                    nOFDifferentNotesInChords[i % nVoices]);
        }
        /// post the constraints specific to first inversion chords
        else if(params->get_chordStates()[i] == FIRST_INVERSION){
            chord_note_occurrence_first_inversion(home, params->get_size(), nVoices, i, params->get_tonality(),
                                                  params->get_chordDegrees(), params->get_chordQualities(), currentChord,
                                                  bassMelodicIntervals, sopranoMelodicIntervals);
        }
        /// post the constraints specific to second inversion chords
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
            /// @todo maybe do it also <--- so that it can propagate in both directions, if the harmonic interval is a perfect fifth or octave the previous and next chords can't
            //todo maybe rewrite this
            forbid_parallel_intervals(home, params->get_size(), nVoices, {PERFECT_FIFTH, PERFECT_OCTAVE, UNISSON},
                                      voicing, bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                                      bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                                      tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);
        }

        /// resolve the tritone if there is one and it needs to be resolved
        if ((params->get_chordDegrees()[i] == SEVENTH_DEGREE && params->get_chordQualities()[i] == DIMINISHED_CHORD &&
            params->get_chordDegrees()[i + 1] == FIRST_DEGREE) ||
            (params->get_chordDegrees()[i] == FIFTH_DEGREE && params->get_chordDegrees()[i+1] == FIRST_DEGREE) ||
            ((params->get_chordDegrees()[i] >= FIVE_OF_TWO && params->get_chordDegrees()[i] <= FIVE_OF_SEVEN) &&
                params->get_chordDegrees()[i+1] != FIFTH_DEGREE_APPOGIATURA)){
            //@todo add other chords that have the tritone
            tritone_resolution(home, nVoices, i, params->get_tonality(), params->get_chordDegrees(),
                               params->get_chordQualities(), params->get_chordStates(), bassMelodicIntervals,
                               tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals, voicing);
        }

        /// Exceptions to the general voice leading rules

        /// special rule for interrupted cadence
        if (params->get_chordDegrees()[i] == FIFTH_DEGREE && params->get_chordStates()[i] == FUNDAMENTAL_STATE &&
        params->get_chordDegrees()[i + 1] == SIXTH_DEGREE && params->get_chordStates()[i + 1] == FUNDAMENTAL_STATE) {
            interrupted_cadence(home, i, params->get_tonality(),
                                voicing, tenorMelodicIntervals,
                                altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// special rules for augmented sixth chords
        else if (params->get_chordDegrees()[i] == AUGMENTED_SIXTH) {
            italian_augmented_sixth(home, nVoices, i, params->get_tonality(), voicing,
                                    bassMelodicIntervals, tenorMelodicIntervals,
                                    altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// if we have an appogiatura for the V degree chord, the voice with the fundamental must move in contrary
        /// motion to the bass
        else if(params->get_chordDegrees()[i] == FIRST_DEGREE && params->get_chordStates()[i] == SECOND_INVERSION &&
                params->get_chordDegrees()[i+1] == FIFTH_DEGREE && (params->get_chordQualities()[i] == MAJOR_CHORD ||
                    params->get_chordQualities()[i] == DOMINANT_SEVENTH_CHORD)){
            fifth_degree_appogiatura(home, nVoices, i, params->get_tonality(), voicing,
                                     bassMelodicIntervals, tenorMelodicIntervals,
                                     altoMelodicIntervals, sopranoMelodicIntervals);
        }
        /// general voice leading rules
        else {
            if ((params->get_chordQualities()[i+1] == MAJOR_SEVENTH_CHORD || params->get_chordQualities()[i+1] == MINOR_SEVENTH_CHORD ||
                params->get_chordQualities()[i+1] == DIMINISHED_SEVENTH_CHORD
                || params->get_chordQualities()[i+1] == HALF_DIMINISHED_CHORD)  && params->get_chordQualities()[i+1] <= SEVENTH_DEGREE) {
                /// the seventh must be prepared
                species_seventh(home, nVoices, i, params->get_tonality(), params->get_chordDegrees(), params->get_chordQualities(), voicing);
            }

            /// If the bass moves by a step, other voices should move in contrary motion
            const int bassFirstChord = (params->get_tonality()->get_degree_note(params->get_chordDegrees()[i] + 2 * params->get_chordStates()[i])
                    % PERFECT_OCTAVE);
            const int bassSecondChord = (params->get_tonality()->get_degree_note(params->get_chordDegrees()[i + 1] + 2 * params->get_chordStates()[i + 1])
                    % PERFECT_OCTAVE);
            const int bassMelodicMotion = abs(bassSecondChord - bassFirstChord);
            /// if the bass moves by a step between fund. state chords @todo check if this needs to apply in other cases
            if ((bassMelodicMotion == MINOR_SECOND || bassMelodicMotion == MAJOR_SECOND ||
                bassMelodicMotion == MINOR_SEVENTH || bassMelodicMotion == MAJOR_SEVENTH) &&
                (params->get_chordStates()[i] == FUNDAMENTAL_STATE && params->get_chordStates()[i+1] == FUNDAMENTAL_STATE)){
                /// move other voices in contrary motion
                contrary_motion_to_bass(home, i,bassMelodicIntervals,
                                        tenorMelodicIntervals,altoMelodicIntervals,
                                        sopranoMelodicIntervals);
            }
            /// if II -> V, move voices in contrary motion to bass
            else if(params->get_chordDegrees()[i] == SECOND_DEGREE && params->get_chordDegrees()[i+1] == FIFTH_DEGREE){
                contrary_motion_to_bass(home, i, bassMelodicIntervals,
                                        tenorMelodicIntervals, altoMelodicIntervals,
                                        sopranoMelodicIntervals);
            }
            else if(params->get_chordDegrees()[i] != params->get_chordDegrees()[i + 1]){
                /// Otherwise, keep common notes in the same voice whenever possible (cost to minimize)
            }
        }
    }

}

/**
 * todo redo
 * Constructor to find all optimal solutions (with or without margin) based on the cost vector for one of the best ones.
 * @param home
 * @param s the number of chords in the progression
 */
// TonalProgression::TonalProgression(Home home, int s, Tonality *t, vector<int> chordDegs, vector<int> chordQuals,
//     vector<int> chordStas, IntVarArray& fullVoicing, vector<int> costs, double margin) :
//         TonalProgression(home, s, t,chordDegs, chordQuals, chordStas, fullVoicing) {
//     double lb_factor = 1.0 - margin;
//     double ub_factor = 1.0 + margin;
//     std::cout << lb_factor << "   " << ub_factor << std::endl;
//     ///todo c'est moche mais comme le dernier cout est négatif faut faire l'inverse sinon bah il y a pas de solutions
//     for(int i = 0; i < costs.size()-1; i++){
//         std::cout << "lower bound = " << floor(costs[i] * lb_factor) << " upper bound = " << ceil(costs[i] * ub_factor) << std::endl;
//         rel(home, floor(costs[i] * lb_factor) <= costVector[i]);
//         rel(home, costVector[i] <= ceil(costs[i] * ub_factor));
//     }
//     //todo rendre ça beau plz
//     std::cout << "lower bound = " << floor(costs[costs.size()-1] * ub_factor) << " upper bound = " << ceil(costs[costs.size()-1] * lb_factor) << std::endl;
//     rel(home, costVector[costs.size()-1] <= ceil(costs[costs.size()-1] * lb_factor));
// }

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
    nDifferentValuesAllChords.update(home, s.nDifferentValuesAllChords);
    nOFDifferentNotesInChords.update(home, s.nOFDifferentNotesInChords);
    commonNotesInSameVoice.update(home, s.commonNotesInSameVoice);

    nOfUnisons.update( home, s.nOfUnisons);

    nOfChordsWithLessThan4Values.update(home, s.nOfChordsWithLessThan4Values);
    nOfIncompleteChords.update(home, s.nOfIncompleteChords);
    nOfCommonNotesInSameVoice.update(home, s.nOfCommonNotesInSameVoice);
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
string TonalProgression::parameters(){
    string message = "-----------------------------------------parameters-----------------------------------------\n";
    message += "Number of chords: " + std::to_string(params->get_size()) + "\n";
    message += "Tonality: " + midi_to_letter(params->get_tonality()->get_tonic()) + " " + mode_int_to_name(params->get_tonality()->get_mode()) + "\n";
    message += "Chords: \n";
    for(int i = 0; i < params->get_size(); i++){
        message += degreeNames[params->get_chordDegrees()[i]] + " (" + chordQualityNames[params->get_chordQualities()[i]] + ") in " +
                   stateNames[params->get_chordStates()[i]];
        if(i != params->get_size() - 1)
            message += ",\n";
    }
    message += ".\n Maximum number of notes in each chord: { ";
    for(int i = 0; i < params->get_size(); i++){
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
string TonalProgression::to_string(){
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
    message += "nDifferentValuesInAllChords = \t\t" + intVarArray_to_string(nDifferentValuesAllChords) + "\n";
    message += "commonNotesInSameVoice = \t\t" + intVarArray_to_string(commonNotesInSameVoice) + "\n";
    message += "nOFDifferentNotesInChords = \t\t" + intVarArray_to_string(nOFDifferentNotesInChords) + "\n";

    message += "nOfUnisons = \t\t" + intVar_to_string(nOfUnisons) + "\n\n";

    message += "------------------------------------cost variables----------------------------------------\n";

    message += "nOfChordsWithLessThan4Values = " + intVar_to_string(nOfChordsWithLessThan4Values) + "\n";
    message += "nOfIncompleteChords = " + intVar_to_string(nOfIncompleteChords) + "\n";
    message += "nOfCommonNotesInSameVoice = " + intVar_to_string(nOfCommonNotesInSameVoice,true) + "\n";
    message += "nOfCommonNotesTenor = " + intVar_to_string(commonNotesInSameVoice[TENOR]) + "\n";
    message += "nOfCommonNotesAlto = " + intVar_to_string(commonNotesInSameVoice[ALTO]) + "\n";
    message += "nOfCommonNotesSoprano = " + intVar_to_string(commonNotesInSameVoice[SOPRANO]) + "\n";
    message += "nOfCommonNotesBass = " + intVar_to_string(commonNotesInSameVoice[BASS]) + "\n";
    return message;
}