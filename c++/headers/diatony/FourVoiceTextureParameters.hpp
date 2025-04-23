//
// Created by Damien Sprockeels on 23/04/2025.
//

#ifndef FOURVOICETEXTUREPARAMETERS_HPP
#define FOURVOICETEXTUREPARAMETERS_HPP

#include "../aux/Utilities.hpp"
#include "TonalProgressionParameters.hpp"

class FourVoiceTextureParameters {
protected:
    int totalNumberOfChords;
    int numberOfSections;

    vector<TonalProgressionParameters*> sectionParameters;

    //    vector<int> modulationStarts;
    //    vector<int> modulationEnds;

public:
    FourVoiceTextureParameters(int nChords, int nSections, vector<TonalProgressionParameters*> sParams) :
        totalNumberOfChords(nChords), numberOfSections(nSections), sectionParameters(std::move(sParams)) {}

    /**                             getters                             **/
    int get_totalNumberOfChords() const { return totalNumberOfChords; }

    int get_numberOfSections() const { return numberOfSections; }

    int get_sectionStart(int section) const { return sectionParameters[section]->get_start(); }

    int get_sectionEnd(int section) const { return sectionParameters[section]->get_end(); }

    Tonality* get_sectionTonality(int section) const { return sectionParameters[section]->get_tonality(); }

    vector<TonalProgressionParameters*> get_sectionParameters() const { return sectionParameters; }

    TonalProgressionParameters* get_sectionParameters(int section) const { return sectionParameters[section]; }

    /**
    *
    */
    string toString() const;

};
#endif //FOURVOICETEXTUREPARAMETERS_HPP
