//
// Created by Damien Sprockeels on 01/04/2025.
//

#ifndef FOURVOICETEXTURE_HPP
#define FOURVOICETEXTURE_HPP

#include "TonalProgression.hpp"
#include "FourVoiceTextureParameters.hpp"
#include "../aux/Utilities.hpp"

class FourVoiceTexture : public Space { // todo add IntLexMinimizeSpace again when the changes to the structure are over
private:
    int                         nVoices = 4;
    FourVoiceTextureParameters* params;
    vector<TonalProgression*>           tonalProgressions;

    IntVarArray                 fullVoicing;

public:
    FourVoiceTexture(FourVoiceTextureParameters* params);

    FourVoiceTexture(FourVoiceTexture& s);

    int getNVoices() const { return nVoices; }

    Space* copy() override;

    //virtual IntVarArgs cost() const;

    string to_string() const;

};



#endif //FOURVOICETEXTURE_HPP
