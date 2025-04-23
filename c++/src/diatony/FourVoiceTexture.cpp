//
// Created by Damien Sprockeels on 01/04/2025.
//

#include "../../headers/diatony/FourVoiceTexture.hpp"

FourVoiceTexture::FourVoiceTexture(FourVoiceTextureParameters* params) : params(params) {

    fullVoicing = IntVarArray(*this, nVoices * params->get_totalNumberOfChords(), 0, 127);

    for (int i = 0; i < params->get_numberOfSections(); i++) {
        tonalProgressions.push_back(new TonalProgression(*this, this->params->get_sectionParameters(i), fullVoicing));
        std::cout << "progression:\n" << tonalProgressions[i]->to_string() << std::endl;
    }

    branch(*this, fullVoicing, INT_VAR_NONE(), INT_VAL_MIN());

}

FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s) : Space(s) { //IntLexMinimizeSpace
    nVoices = s.nVoices;
    params = s.params;

    fullVoicing.update(*this, s.fullVoicing);

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
    message += "Full voicing array:" + intVarArray_to_string(fullVoicing) + "\n";
    return message;
    //"Four Voice Texture object:\n\n" + tonalProgression->to_string();
}
