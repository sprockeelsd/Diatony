//
// Created by Damien Sprockeels on 23/04/2025.
//

#ifndef FOURVOICETEXTUREPARAMETERS_HPP
#define FOURVOICETEXTUREPARAMETERS_HPP

#include <utility>

#include "../aux/Utilities.hpp"
#include "TonalProgressionParameters.hpp"
#include "ModulationParameters.hpp"

class FourVoiceTextureParameters {
protected:
    const int                               totalNumberOfChords;        // total number of chords in the piece
    const int                               numberOfSections;           // number of sections of different tonalities in the piece
    vector<TonalProgressionParameters*>     sectionParameters;          // parameter objects for each section
    vector<ModulationParameters*>           modulationParameters;       // modulation parameters for each modulation

public:
    /**
     * Constructor
     * @param nChords number of chords in the piece
     * @param nSections number of sections in the piece
     * @param sParams a vector of TonalProgressionParameters objects for each section
     * @param mParams a vector of ModulationParameters objects for each modulation
     */
    FourVoiceTextureParameters(const int nChords, const int nSections, vector<TonalProgressionParameters*> sParams,
        vector<ModulationParameters*> mParams) :
        totalNumberOfChords(nChords), numberOfSections(nSections), sectionParameters(std::move(sParams)),
        modulationParameters(std::move(mParams)) {}

    /**                             getters                             **/
    int get_totalNumberOfChords() const { return totalNumberOfChords; }

    int get_numberOfSections() const { return numberOfSections; }

    int get_sectionStart(const int section) const { return sectionParameters[section]->get_start(); }

    int get_sectionEnd(const int section) const { return sectionParameters[section]->get_end(); }

    Tonality* get_sectionTonality(const int section) const { return sectionParameters[section]->get_tonality(); }

    vector<TonalProgressionParameters*> get_sectionParameters() const { return sectionParameters; }

    TonalProgressionParameters* get_sectionParameters(const int section) const { return sectionParameters[section]; }

    /**
     * toString method
     * Prints the total number of chords of the piece, the number of sections, the section starts and ends, the tonalities of
     * the sections, and the parameters for each of the sections
     * @return a string representation of the object
     */
    string toString() const;

};
#endif //FOURVOICETEXTUREPARAMETERS_HPP