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

    vector<int> sectionStarts;
    vector<int> sectionEnds;
    vector<Tonality*> sectionTonalities;
    vector<TonalProgressionParameters*> sectionParameters;

    //    vector<int> modulationStarts;
    //    vector<int> modulationEnds;

public:
    FourVoiceTextureParameters(int nChords, int nSections, vector<int> sStarts, vector<int> sEnds,
                             vector<Tonality*> sTonalities, vector<TonalProgressionParameters*> sParams) :
        totalNumberOfChords(nChords), numberOfSections(nSections), sectionStarts(std::move(sStarts)),
        sectionEnds(std::move(sEnds)), sectionTonalities(std::move(sTonalities)), sectionParameters(std::move(sParams)) {}

    /**                             getters                             **/
    int get_totalNumberOfChords() const { return totalNumberOfChords; }

    int get_numberOfSections() const { return numberOfSections; }

    vector<int> get_sectionStarts() const { return sectionStarts; }

    int get_sectionStart(int section) const { return sectionStarts[section]; }

    vector<int> get_sectionEnds() const { return sectionEnds; }

    int get_sectionEnd(int section) const { return sectionEnds[section]; }

    vector<Tonality*> get_sectionTonalities() const { return sectionTonalities; }

    Tonality* get_sectionTonality(int section) const { return sectionTonalities[section]; }

    vector<TonalProgressionParameters*> get_sectionParameters() const { return sectionParameters; }

    TonalProgressionParameters* get_sectionParameters(int section) const { return sectionParameters[section]; }

    /**
    *
    */
    string toString() const;

};
#endif //FOURVOICETEXTUREPARAMETERS_HPP
