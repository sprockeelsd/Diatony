//
// Created by Damien Sprockeels on 01/04/2025.
//

#ifndef FOURVOICETEXTURE_HPP
#define FOURVOICETEXTURE_HPP

#include "TonalProgression.hpp"
#include "../aux/Utilities.hpp"

class FourVoiceTexture : public Space { // todo add IntLexMinimizeSpace again when the changes to the structure are over
private:
    int                         nVoices = 4;
    int                         size; /// the number of chords in the progression
    TonalProgression*           tonalProgression;

    IntVarArray                 fullVoicing;

public:
    FourVoiceTexture(int s, Tonality *t, vector<int> chordDegs, vector<int> chordQuals, vector<int> chordStas);

    FourVoiceTexture(FourVoiceTexture& s);

    int getNVoices() const { return nVoices; }

    Space* copy() override;

    //virtual IntVarArgs cost() const;

};



#endif //FOURVOICETEXTURE_HPP
