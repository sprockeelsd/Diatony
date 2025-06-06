//
// Created by Damien Sprockeels on 01/04/2025.
//

#include "../../headers/diatony/FourVoiceTexture.hpp"

FourVoiceTexture::FourVoiceTexture(FourVoiceTextureParameters* params) : params(params) {

    /// General arrays initialization
    fullVoicing = IntVarArray(*this, nVoices * params->get_totalNumberOfChords(), 0, 127);

    /// Melodic interval arrays initialization
    bassMelodicIntervals    = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    tenorMelodicIntervals   = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    altoMelodicIntervals    = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    sopranoMelodicIntervals = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);

    allMelodicIntervals     = IntVarArray(*this, nVoices * (params->get_totalNumberOfChords() - 1), -PERFECT_OCTAVE, PERFECT_OCTAVE);

    /// Link between voicing and melodic intervals
    link_melodic_arrays(*this, nVoices, params->get_totalNumberOfChords(), fullVoicing, bassMelodicIntervals,
                        altoMelodicIntervals, tenorMelodicIntervals, sopranoMelodicIntervals);

    for(int i = 0; i < params->get_totalNumberOfChords()-1; i++){
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + BASS]            == bassMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + TENOR]           == tenorMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + ALTO]            == altoMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + SOPRANO]         == sopranoMelodicIntervals[i]));
    }

    // Harmonic interval arrays initialization
    bassTenorHarmonicIntervals                      = IntVarArray(*this, params->get_totalNumberOfChords(), 0, PERFECT_OCTAVE + PERFECT_FIFTH);
    bassAltoHarmonicIntervals                       = IntVarArray(*this, params->get_totalNumberOfChords(), 0, 2 * PERFECT_OCTAVE + PERFECT_FIFTH);
    bassSopranoHarmonicIntervals                    = IntVarArray(*this, params->get_totalNumberOfChords(), 0, 3 * PERFECT_OCTAVE + PERFECT_FIFTH);
    tenorAltoHarmonicIntervals                      = IntVarArray(*this, params->get_totalNumberOfChords(), 0, PERFECT_OCTAVE);
    tenorSopranoHarmonicIntervals                   = IntVarArray(*this, params->get_totalNumberOfChords(), 0, 2 * PERFECT_OCTAVE);
    altoSopranoHarmonicIntervals                    = IntVarArray(*this, params->get_totalNumberOfChords(), 0, PERFECT_OCTAVE);

    link_harmonic_arrays(*this, nVoices, params->get_totalNumberOfChords(), fullVoicing,
             bassTenorHarmonicIntervals, bassAltoHarmonicIntervals, bassSopranoHarmonicIntervals,
             tenorAltoHarmonicIntervals, tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals);

    /// Initialisation of the cost variable arrays
    costsAllMelodicIntervals                        = IntVarArray(*this, nVoices * (params->get_totalNumberOfChords() - 1), UNISON_COST, MAX_MELODIC_COST);
    nDifferentValuesInDiminishedChord               = IntVarArray(*this, params->get_totalNumberOfChords(), 0, nVoices);
    nDifferentValuesAllChords                       = IntVarArray(*this, params->get_totalNumberOfChords(), 0, nVoices);
    nOfDifferentNotesInChords                       = IntVarArray(*this, params->get_totalNumberOfChords(), 0, nVoices);
    nIncompleteChordsForEachSection                   = IntVarArray(*this, params->get_numberOfSections(), 0, params->get_totalNumberOfChords()); // upper bound
    commonNotesInSameVoice                          = IntVarArray(*this, nVoices, 0, params->get_totalNumberOfChords() - 1);

    nOfUnisons                                     = IntVar(*this, 0, nVoices * (params->get_totalNumberOfChords() - 1));

    /// Initialization of cost variables
    costOfMelodicIntervals                          = IntVar(*this, 0, nVoices*(params->get_totalNumberOfChords()-1)* MAX_MELODIC_COST);
    nOfFundStateDiminishedChordsWith4notes          = IntVar(*this, 0, params->get_totalNumberOfChords());
    nOfChordsWithLessThan4Values                    = IntVar(*this, 0, params->get_totalNumberOfChords());
    nOfIncompleteChords                             = IntVar(*this, 0, params->get_totalNumberOfChords());
    nOfCommonNotesInSameVoice                       = IntVar(*this, - nVoices * (params->get_totalNumberOfChords() - 1), 0);

    /// Creation of the subproblems for each progression
    for (int i = 0; i < params->get_numberOfSections(); i++) {
        tonalProgressions.push_back(new TonalProgression(*this, this->params->get_sectionParameters(i), fullVoicing,
                                                         bassMelodicIntervals, tenorMelodicIntervals,
                                                         altoMelodicIntervals, sopranoMelodicIntervals,
                                                         allMelodicIntervals,
                                                            bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                                                            bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                                                            tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals,
                                                         nDifferentValuesInDiminishedChord,
                                                         nOfDifferentNotesInChords, nIncompleteChordsForEachSection[i]));
    }


    /// Modulation constraints
    for (int i = 0; i < this->params->get_numberOfSections() - 1; i++) {
        switch (this->params->get_modulationParameters(i)->get_type()) {
            case PERFECT_CADENCE_MODULATION: // make sure that parallel fifths are not allowed
                forbid_parallel_intervals(*this, params->get_totalNumberOfChords(), nVoices,
                                          {PERFECT_FIFTH, PERFECT_OCTAVE, UNISSON}, fullVoicing,
                                          bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                                          bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                                          tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals,
                                          this->params->get_modulationParameters(i)->get_from()->get_end(),
                                            this->params->get_modulationParameters(i)->get_to()->get_start()
                                          );
            case PIVOT_CHORD_MODULATION: // after the pivot chord, the chords are voiced as if they were in the new tonality
                // nothing to do
                break;
            case ALTERATION_MODULATION: // make sure parallel fifths are not allowed
                forbid_parallel_intervals(*this, params->get_totalNumberOfChords(), nVoices,
                                          {PERFECT_FIFTH, PERFECT_OCTAVE, UNISSON}, fullVoicing,
                                          bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                                          bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                                          tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals,
                                          this->params->get_modulationParameters(i)->get_from()->get_end(),
                                            this->params->get_modulationParameters(i)->get_to()->get_start()
                                          );
                break;
            case CHROMATIC_MODULATION: {
                // The chromatism must be in the same voice
                auto leading_tone = this->params->get_modulationParameters(i)->get_to()->get_tonality()->get_degree_note(SEVENTH_DEGREE);
                auto modulation_start = this->params->get_modulationParameters(i)->get_start();

                for (int j = BASS; j <= SOPRANO; j++) {
                    // the leading tone must be preceded by the note one semitone below it in the same voice
                    rel(*this, expr(*this, fullVoicing[(modulation_start + 1) * nVoices + j] % PERFECT_OCTAVE == leading_tone), BOT_IMP,
                        expr(*this, fullVoicing[(modulation_start) * nVoices + j] % PERFECT_OCTAVE == leading_tone - MINOR_SECOND), true);
                    // If the note leading to the chromatism is doubled, the one not going to the leading tone must go down
                    rel(*this, expr(*this, fullVoicing[modulation_start] % PERFECT_OCTAVE ==
                        (leading_tone + PERFECT_OCTAVE - MINOR_SECOND) % PERFECT_OCTAVE &&
                        fullVoicing[(modulation_start + 1) * nVoices + j] % PERFECT_OCTAVE != leading_tone), BOT_IMP,
                        expr(*this, allMelodicIntervals[modulation_start * nVoices + j] < 0), true);
                }
                // forbid parallel intervals in the modulation
                forbid_parallel_intervals(*this, params->get_totalNumberOfChords(), nVoices,
                          {PERFECT_FIFTH, PERFECT_OCTAVE, UNISSON}, fullVoicing,
                          bassTenorHarmonicIntervals, bassAltoHarmonicIntervals,
                          bassSopranoHarmonicIntervals, tenorAltoHarmonicIntervals,
                          tenorSopranoHarmonicIntervals, altoSopranoHarmonicIntervals,
                          this->params->get_modulationParameters(i)->get_from()->get_end(),
                            this->params->get_modulationParameters(i)->get_to()->get_start()
                          );
            }
                break;
            default:
                throw std::invalid_argument("Unknown modulation type: " +
                                            modulation_type_names[this->params->get_modulationParameters(i)->get_type()] +
                                            ". Could not post voicing-related constraints associated to this modulation type.");
        }
    }

    /// test constraints

    /**------------------------------------ cost linking ---------------------------------------------------**/
    /// weighted sum of melodic intervals (cost to minimize)
    compute_cost_for_melodic_intervals(*this, allMelodicIntervals, nOfUnisons,
                                       costOfMelodicIntervals, costsAllMelodicIntervals);

    // costVar = number of diminished chords with 4 notes
    count(*this, nDifferentValuesInDiminishedChord, 4, IRT_EQ, nOfFundStateDiminishedChordsWith4notes);

    /// number of chords with less than 4 note values (cost to minimize)
    compute_n_of_notes_in_chord_cost(*this, nVoices, params->get_totalNumberOfChords(), fullVoicing,
                                     nDifferentValuesAllChords, nOfChordsWithLessThan4Values);

    /// the sum of incomplete chords in each section
    linear(*this, nIncompleteChordsForEachSection, IRT_EQ, nOfIncompleteChords);

    /// count the number of common notes in the same voice between consecutive chords (cost to MAXIMIZE)
    /// /!\ The variable nOfCommonNotesInSameVoice has a NEGATIVE value so the minimization will maximize its absolute value
    compute_cost_for_common_notes_not_in_same_voice(*this, bassMelodicIntervals, tenorMelodicIntervals,
                                                    altoMelodicIntervals, sopranoMelodicIntervals, nOfUnisons,
                                                    commonNotesInSameVoice,
                                                    nOfCommonNotesInSameVoice);


    costVector = {nOfIncompleteChords, nOfFundStateDiminishedChordsWith4notes, nOfChordsWithLessThan4Values,
        costOfMelodicIntervals, nOfCommonNotesInSameVoice};


    /// go <-- soprano->bass: 4-3-2-1-8-7-6-5 etc
    auto r_to_l = [](const Space& home, const IntVar& x, int i) {
        return i;
    };

    branch(*this, fullVoicing, INT_VAR_MERIT_MAX(r_to_l), INT_VAL_RND(1U));
}

FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s) : IntLexMinimizeSpace(s) { //IntLexMinimizeSpace
    nVoices = s.nVoices;
    params = s.params;

    fullVoicing.update(*this, s.fullVoicing);

    bassMelodicIntervals.update(*this, s.bassMelodicIntervals);
    tenorMelodicIntervals.update(*this, s.tenorMelodicIntervals);
    altoMelodicIntervals.update(*this, s.altoMelodicIntervals);
    sopranoMelodicIntervals.update(*this, s.sopranoMelodicIntervals);

    allMelodicIntervals.update(*this, s.allMelodicIntervals);

    bassTenorHarmonicIntervals.update(*this, s.bassTenorHarmonicIntervals);
    bassAltoHarmonicIntervals.update(*this, s.bassAltoHarmonicIntervals);
    bassSopranoHarmonicIntervals.update(*this, s.bassSopranoHarmonicIntervals);
    tenorAltoHarmonicIntervals.update(*this, s.tenorAltoHarmonicIntervals);
    tenorSopranoHarmonicIntervals.update(*this, s.tenorSopranoHarmonicIntervals);
    altoSopranoHarmonicIntervals.update(*this, s.altoSopranoHarmonicIntervals);

    nOfUnisons.update( *this, s.nOfUnisons);

    /// cost-related arrays
    costsAllMelodicIntervals.update(*this, s.costsAllMelodicIntervals);
    nDifferentValuesInDiminishedChord.update(*this, s.nDifferentValuesInDiminishedChord);
    nDifferentValuesAllChords.update(*this, s.nDifferentValuesAllChords);
    nOfDifferentNotesInChords.update(*this, s.nOfDifferentNotesInChords);
    nIncompleteChordsForEachSection.update(*this, s.nIncompleteChordsForEachSection);
    commonNotesInSameVoice.update(*this, s.commonNotesInSameVoice);

    /// cost variables
    costOfMelodicIntervals.update(*this, s.costOfMelodicIntervals);
    nOfFundStateDiminishedChordsWith4notes.update(*this, s.nOfFundStateDiminishedChordsWith4notes);
    nOfChordsWithLessThan4Values.update(*this, s.nOfChordsWithLessThan4Values);
    nOfIncompleteChords.update(*this, s.nOfIncompleteChords);
    nOfCommonNotesInSameVoice.update(*this, s.nOfCommonNotesInSameVoice);


    costVector = {nOfIncompleteChords, nOfFundStateDiminishedChordsWith4notes, nOfChordsWithLessThan4Values,
        costOfMelodicIntervals, nOfCommonNotesInSameVoice};

    for (auto p : s.tonalProgressions)
        tonalProgressions.push_back(new TonalProgression(*this, *p));
}

Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
}

IntVarArgs FourVoiceTexture::cost() const {
    return costVector;
}

/**
 * Returns the values taken by the variables in a solution as a pointer to an integer array
 * @return an array of integers representing the values of the variables in a solution
 */
int* FourVoiceTexture::return_solution() const{
    int size = params->get_totalNumberOfChords();
    int* solution = new int[size*4];
    for(int i = 0; i < 4*size; i++){
        solution[i] = fullVoicing[i].val();
    }
    return solution;
}

string FourVoiceTexture::to_string() const {
    string message;
    message += "Four Voice Texture object:\n";
    message += "Parameters: " + params->toString() + "\n";
    message += "Tonal Progressions:\n";
    for (auto p : tonalProgressions) {
        message += p->to_string() + "\n";
    }
    message += "Full voicing array:" + intVarArray_to_string(fullVoicing) + "\n\n";

    message += "Bass Melodic Intervals:\n" + intVarArray_to_string(bassMelodicIntervals) + "\n";
    message += "Tenor Melodic Intervals:\n" + intVarArray_to_string(tenorMelodicIntervals) + "\n";
    message += "Alto Melodic Intervals:\n" + intVarArray_to_string(altoMelodicIntervals) + "\n";
    message += "Soprano Melodic Intervals:\n" + intVarArray_to_string(sopranoMelodicIntervals) + "\n";

    message += "All Melodic Intervals:\n" + intVarArray_to_string(allMelodicIntervals) + "\n";

    message += "Bass-Tenor Harmonic Intervals:\n" + intVarArray_to_string(bassTenorHarmonicIntervals) + "\n";
    message += "Bass-Alto Harmonic Intervals:\n" + intVarArray_to_string(bassAltoHarmonicIntervals) + "\n";
    message += "Bass-Soprano Harmonic Intervals:\n" + intVarArray_to_string(bassSopranoHarmonicIntervals) + "\n";
    message += "Tenor-Alto Harmonic Intervals:\n" + intVarArray_to_string(tenorAltoHarmonicIntervals) + "\n";
    message += "Tenor-Soprano Harmonic Intervals:\n" + intVarArray_to_string(tenorSopranoHarmonicIntervals) + "\n";
    message += "Alto-Soprano Harmonic Intervals:\n" + intVarArray_to_string(altoSopranoHarmonicIntervals) + "\n\n";

    message += "-------------------------------cost-related auxiliary arrays------------------------------\n";

    message += "nDifferentValuesInDiminishedChord = \t" + intVarArray_to_string(nDifferentValuesInDiminishedChord) + "\n";
    message += "nDifferentValuesInAllChords = \t\t" + intVarArray_to_string(nDifferentValuesAllChords) + "\n";
    message += "commonNotesInSameVoice = \t\t" + intVarArray_to_string(commonNotesInSameVoice) + "\n";
    message += "noFDifferentNotesInChords = \t\t" + intVarArray_to_string(nOfDifferentNotesInChords) + "\n";
    message += "nIncompleteChordsForEachSection = \t" + intVarArray_to_string(nIncompleteChordsForEachSection) + "\n";
    message += "costsAllMelodicIntervals = \t\t" + intVarArray_to_string(costsAllMelodicIntervals) + "\n\n";

    message += "nOfUnisons = \t\t" + intVar_to_string(nOfUnisons) + "\n\n";

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
