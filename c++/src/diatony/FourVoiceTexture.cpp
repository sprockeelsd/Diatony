//
// Created by Damien Sprockeels on 01/04/2025.
//

#include "../../headers/diatony/FourVoiceTexture.hpp"

FourVoiceTexture::FourVoiceTexture(FourVoiceTextureParameters* params) : params(params) {;

    fullVoicing = IntVarArray(*this, nVoices * params->get_totalNumberOfChords(), 0, 127);

    for (int i = 0; i < params->get_numberOfSections(); i++) {
        tonalProgressions.push_back(new TonalProgression(*this, this->params->get_sectionParameters(i), fullVoicing));
        std::cout << "here: " << tonalProgressions.back()->to_string() << std::endl;
    }


    /// go <-- soprano->bass: 4-3-2-1-8-7-6-5 etc
    auto r_to_l = [](const Space& home, const IntVar& x, int i) {
        return i;
    };

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

string FourVoiceTexture::to_string() const {
    string message;
    message += "Four Voice Texture object:\n";
    message += "Parameters: " + params->toString() + "\n";
    message += "Full voicing array:" + intVarArray_to_string(fullVoicing) + "\n";
    message += "Tonal Progressions:\n";
    for (auto p : tonalProgressions) {
        message += p->to_string() + "\n";
    }
    return message;
    //"Four Voice Texture object:\n\n" + tonalProgression->to_string();
}
