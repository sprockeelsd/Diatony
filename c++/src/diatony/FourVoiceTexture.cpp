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

}

FourVoiceTexture::FourVoiceTexture(FourVoiceTexture& s) : Space(s) { //IntLexMinimizeSpace
    size = s.size;
    tonalProgression = new TonalProgression(*this, *s.tonalProgression);

}

Space* FourVoiceTexture::copy() {
    return new FourVoiceTexture(*this);
}

//IntVarArgs FourVoiceTexture::cost() const {
    //return {};
//}
