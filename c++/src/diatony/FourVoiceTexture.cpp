//
// Created by Damien Sprockeels on 01/04/2025.
//

#include "../../headers/diatony/FourVoiceTexture.hpp"

FourVoiceTexture::FourVoiceTexture(FourVoiceTextureParameters* params) : params(params) {

    /// General arrays initialization
    fullVoicing = IntVarArray(*this, nVoices * params->get_totalNumberOfChords(), 0, 127);

    bassMelodicIntervals    = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    tenorMelodicIntervals   = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    altoMelodicIntervals    = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);
    sopranoMelodicIntervals = IntVarArray(*this, params->get_totalNumberOfChords() - 1, -PERFECT_OCTAVE, PERFECT_OCTAVE);

    allMelodicIntervals     = IntVarArray(*this, nVoices * (params->get_totalNumberOfChords() - 1), -PERFECT_OCTAVE, PERFECT_OCTAVE);

    /// General arrays connection
    link_melodic_arrays(*this, nVoices, params->get_totalNumberOfChords(), fullVoicing, bassMelodicIntervals,
                        altoMelodicIntervals, tenorMelodicIntervals, sopranoMelodicIntervals);

    /// global arrays
    for(int i = 0; i < params->get_totalNumberOfChords()-1; i++){
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + BASS]            == bassMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + TENOR]           == tenorMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + ALTO]            == altoMelodicIntervals[i]));
        rel(*this, expr(*this, allMelodicIntervals[nVoices * i + SOPRANO]         == sopranoMelodicIntervals[i]));
    }

    for (int i = 0; i < params->get_numberOfSections(); i++) {
        tonalProgressions.push_back(new TonalProgression(*this, this->params->get_sectionParameters(i), fullVoicing,
            bassMelodicIntervals, tenorMelodicIntervals, altoMelodicIntervals, sopranoMelodicIntervals,
            allMelodicIntervals));
    }

    branch(*this, fullVoicing, INT_VAR_NONE(), INT_VAL_MIN());
}

FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s) : Space(s) { //IntLexMinimizeSpace
    nVoices = s.nVoices;
    params = s.params;

    fullVoicing.update(*this, s.fullVoicing);

    bassMelodicIntervals.update(*this, s.bassMelodicIntervals);
    tenorMelodicIntervals.update(*this, s.tenorMelodicIntervals);
    altoMelodicIntervals.update(*this, s.altoMelodicIntervals);
    sopranoMelodicIntervals.update(*this, s.sopranoMelodicIntervals);

    allMelodicIntervals.update(*this, s.allMelodicIntervals);

    for (auto p : s.tonalProgressions)
        tonalProgressions.push_back(new TonalProgression(*this, *p));
}

Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
}

//IntVarArgs FourVoiceTexture::cost() const {
    //return {};
//}

/**
 * Returns the values taken by the variables vars in a solution as a pointer to an integer array
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

    return message;
    //"Four Voice Texture object:\n\n" + tonalProgression->to_string();
}
