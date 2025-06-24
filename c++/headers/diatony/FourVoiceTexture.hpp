//
// Created by Damien Sprockeels on 01/04/2025.
//

#ifndef FOURVOICETEXTURE_HPP
#define FOURVOICETEXTURE_HPP

#include "TonalProgression.hpp"
#include "FourVoiceTextureParameters.hpp"
#include "../aux/Utilities.hpp"

/**
 * This class represents a four-voice texture problem, where the goal is to harmonize a series of chord progressions.
 * All the chord progression related input is given through the FourVoiceTextureParameters object. The variables are then defined
 * and the constraints are posted based on the parameters.
 */
class FourVoiceTexture : public IntLexMinimizeSpace { // todo add IntLexMinimizeSpace again when the changes to the structure are over
protected:
    int                             nVoices = 4;
    FourVoiceTextureParameters*     params;                         // The parameters that define the problem
    vector<TonalProgression*>       tonalProgressions;              // The tonal progressions subproblems

    IntVarArray                     fullVoicing;                    // The voicing of the chords for the whole piece

    /// Melodic intervals
    IntVarArray                     bassMelodicIntervals;
    IntVarArray                     tenorMelodicIntervals;
    IntVarArray                     altoMelodicIntervals;
    IntVarArray                     sopranoMelodicIntervals;
    IntVarArray                     allMelodicIntervals;

    /// Harmonic intervals (always positive)
    IntVarArray                     bassTenorHarmonicIntervals;
    IntVarArray                     bassAltoHarmonicIntervals;
    IntVarArray                     bassSopranoHarmonicIntervals;
    IntVarArray                     tenorAltoHarmonicIntervals;
    IntVarArray                     tenorSopranoHarmonicIntervals;
    IntVarArray                     altoSopranoHarmonicIntervals;


    /**------------------------------------------------- costs --------------------------------------------------**/
    /// Variables for each type of interval
    IntVar                          nOfUnisons;

    /// cost variables auxiliary arrays
    IntVarArray                     costsAllMelodicIntervals;
    IntVarArray                     nDifferentValuesInDiminishedChord;
    IntVarArray                     nDifferentValuesAllChords;
    IntVarArray                     nOfDifferentNotesInChords;
    IntVarArray                     nIncompleteChordsForEachSection;
    IntVarArray                     commonNotesInSameVoice;

    /// cost variables
    IntVar                          costOfMelodicIntervals;
    IntVar                          nOfFundStateDiminishedChordsWith4notes;
    IntVar                          nOfChordsWithLessThan4Values;
    IntVar                          nOfIncompleteChords;
    IntVar                          nOfCommonNotesInSameVoice;                  // /!\ this cost needs to be maximized, so its value is negative

    IntVarArgs                      costVector;                                 // the costs in lexicographical order for minimization

public:
    /**
     * Constructor for FourVoiceTexture objects.
     * @param params An object containing the parameters for the whole piece.
     */
    explicit FourVoiceTexture(FourVoiceTextureParameters* params);

    /**
     * Copy constructor for FourVoiceTexture objects.
     * @param s A FourVoiceTexture object to copy.
     */
    FourVoiceTexture(FourVoiceTexture& s);

    /**
     * Returns the values taken by the variables vars in a solution as a pointer to an integer array
     * @return an array of integers representing the values of the variables in a solution
     */
    int* return_solution() const;

    /**                     getters                     **/
    int getNVoices() const { return nVoices; }

    FourVoiceTextureParameters* getParameters() const { return params; }

    /**
     * General space copy method
     * @return a new FourVoiceTexture object that is a copy of this one, as a Space pointer
     */
    Space* copy() override;

    /**
     * Cost function
     * @return the cost vector containing the cost variables in lexicographical order
     */
    IntVarArgs cost() const override;

    /**
     * to_string method for the FourVoiceTexture object.
     * @return a string representation of the FourVoiceTexture object
     */
    string to_string() const;

    /**
     * to_string method for the FourVoiceTexture object in a more readable format.
     * @return a string representation of the FourVoiceTexture object in a more readable format
     */
    string pretty() const;
};



#endif //FOURVOICETEXTURE_HPP
