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
    vector<TonalProgression*>   tonalProgressions;

    IntVarArray                 fullVoicing;

    /// Melodic intervals
    IntVarArray                 bassMelodicIntervals;
    IntVarArray                 tenorMelodicIntervals;
    IntVarArray                 altoMelodicIntervals;
    IntVarArray                 sopranoMelodicIntervals;

    IntVarArray                 allMelodicIntervals;

public:
    FourVoiceTexture(FourVoiceTextureParameters* params);

    FourVoiceTexture(FourVoiceTexture& s);

    /**
     * Returns the values taken by the variables vars in a solution as a pointer to an integer array
     * @return an array of integers representing the values of the variables in a solution
     */
    int* return_solution() const;

    int getNVoices() const { return nVoices; }

    FourVoiceTextureParameters* getParameters() const { return params; }

    Space* copy() override;

    //virtual IntVarArgs cost() const;

    string to_string() const;

};



#endif //FOURVOICETEXTURE_HPP
