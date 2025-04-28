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

    /**---------------------------------------- costs ------------------------------------*/
    /// Initialisation of the cost variable arrays
    costsAllMelodicIntervals                        = IntVarArray(*this, nVoices * (params->get_totalNumberOfChords() - 1), UNISON_COST, MAX_MELODIC_COST);
    nDifferentValuesInDiminishedChord               = IntVarArray(*this, params->get_totalNumberOfChords(), 0, nVoices);

    nOfUnisons                                     = IntVar(*this, 0, nVoices * (params->get_totalNumberOfChords() - 1));

    /// Initialization of cost variables
    costOfMelodicIntervals                          = IntVar(*this, 0, nVoices*(params->get_totalNumberOfChords()-1)* MAX_MELODIC_COST);
    nOfFundStateDiminishedChordsWith4notes          = IntVar(*this, 0, params->get_totalNumberOfChords());

    /// Creation of the subproblems for each progression
    for (int i = 0; i < params->get_numberOfSections(); i++) {
        tonalProgressions.push_back(new TonalProgression(*this, this->params->get_sectionParameters(i), fullVoicing,
            bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals,
            allMelodicIntervals, nDifferentValuesInDiminishedChord));
    }

    /// weighted sum of melodic intervals (cost to minimize)
    compute_cost_for_melodic_intervals(*this, allMelodicIntervals, nOfUnisons,
                                       costOfMelodicIntervals, costsAllMelodicIntervals);

    /// number of diminished chords in fundamental state with more than 3 notes (cost to minimize)
    for (int i = 0; i < params->get_numberOfSections(); i++) {
        auto sp  = params->get_sectionParameters()[i];
        compute_diminished_chords_cost(*this, 4, sp->get_size(),
                                       sp->get_chordStates(), sp->get_chordQualities(), fullVoicing, tonalProgressions[i]->getNDifferentValuesInDiminishedChord());
    }
    /// costVar = number of diminished chords with 4 notes
    count(*this, nDifferentValuesInDiminishedChord, 4, IRT_EQ, nOfFundStateDiminishedChordsWith4notes);


    // costVector = {nOfIncompleteChords, nOfFundStateDiminishedChordsWith4notes, nOfChordsWithLessThan4Values,
    //           costOfMelodicIntervals, nOfCommonNotesInSameVoice};

    costVector = {nOfFundStateDiminishedChordsWith4notes, costOfMelodicIntervals};


    branch(*this, fullVoicing, INT_VAR_NONE(), INT_VAL_MIN());
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

    nOfUnisons.update( *this, s.nOfUnisons);

    costsAllMelodicIntervals.update(*this, s.costsAllMelodicIntervals);
    nDifferentValuesInDiminishedChord.update(*this, s.nDifferentValuesInDiminishedChord);

    costOfMelodicIntervals.update(*this, s.costOfMelodicIntervals);
    nOfFundStateDiminishedChordsWith4notes.update(*this, s.nOfFundStateDiminishedChordsWith4notes);

    costVector = {nOfFundStateDiminishedChordsWith4notes, costOfMelodicIntervals};

    for (auto p : s.tonalProgressions)
        tonalProgressions.push_back(new TonalProgression(*this, *p));
}

Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
}

IntVarArgs FourVoiceTexture::cost() const {
    return {};
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

    message += "-------------------------------cost-related auxiliary arrays------------------------------\n";

    message += "nDifferentValuesInDiminishedChord = \t" + intVarArray_to_string(nDifferentValuesInDiminishedChord) + "\n";
    // message += "nDifferentValuesInAllChords = \t\t" + intVarArray_to_string(nDifferentValuesAllChords) + "\n";
    // message += "commonNotesInSameVoice = \t\t" + intVarArray_to_string(commonNotesInSameVoice) + "\n";
    // message += "nOFDifferentNotesInChords = \t\t" + intVarArray_to_string(nOFDifferentNotesInChords) + "\n";
    message += "costsAllMelodicIntervals = \t\t" + intVarArray_to_string(costsAllMelodicIntervals) + "\n\n";

    message += "nOfUnisons = \t\t" + intVar_to_string(nOfUnisons) + "\n\n";

    message += "------------------------------------cost variables----------------------------------------\n";

    message += "nOfFundStateDiminishedChordsWith4notes = " + intVar_to_string(nOfFundStateDiminishedChordsWith4notes) + "\n";
    // message += "nOfChordsWithLessThan4Values = " + intVar_to_string(nOfChordsWithLessThan4Values) + "\n";
    // message += "nOfIncompleteChords = " + intVar_to_string(nOfIncompleteChords) + "\n";
    // message += "nOfCommonNotesInSameVoice = " + intVar_to_string(nOfCommonNotesInSameVoice,true) + "\n";
    // message += "nOfCommonNotesTenor = " + intVar_to_string(commonNotesInSameVoice[TENOR]) + "\n";
    // message += "nOfCommonNotesAlto = " + intVar_to_string(commonNotesInSameVoice[ALTO]) + "\n";
    // message += "nOfCommonNotesSoprano = " + intVar_to_string(commonNotesInSameVoice[SOPRANO]) + "\n";
    // message += "nOfCommonNotesBass = " + intVar_to_string(commonNotesInSameVoice[BASS]) + "\n";
    message += "costOfMelodicIntervals = " + intVar_to_string(costOfMelodicIntervals) + "\n\n";

    message += "Cost vector = {" + intVarArgs_to_string(costVector) + "}\n\n";

    return message;
    //"Four Voice Texture object:\n\n" + tonalProgression->to_string();
}
