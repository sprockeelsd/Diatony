//
// Created by Damien Sprockeels on 01/04/2025.
//

#include "../../headers/diatony/FourVoiceTexture.hpp"

FourVoiceTexture::FourVoiceTexture(int s, Tonality *t,
    vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas) {
    size = s;

    fullVoicing                                     = IntVarArray(*this, nVoices * size, 0, 127);

    tonalProgression = new TonalProgression(*this, size, t, chordDegs, chordQuals, chordStas, fullVoicing);
    std::cout << "here: " << tonalProgression->to_string() << std::endl;

    /// go <-- soprano->bass: 4-3-2-1-8-7-6-5 etc
    auto r_to_l = [](const Space& home, const IntVar& x, int i) {
        return i;
    };

    branch(*this, fullVoicing, INT_VAR_NONE(), INT_VAL_MIN());

}

FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s) : Space(s) { //IntLexMinimizeSpace
    size = s.size;
    nVoices = s.nVoices;

    fullVoicing.update(*this, s.fullVoicing);
    tonalProgression = new TonalProgression(*this, *s.tonalProgression);

}

Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
}

//IntVarArgs FourVoiceTexture::cost() const {
    //return {};
//}

string FourVoiceTexture::to_string() const {
    return "Four Voice Texture object:\n\n" + tonalProgression->to_string();
}
