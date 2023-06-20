#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"


MajorTonality::MajorTonality(int t) : Tonality(t, MAJOR_SCALE){ // calls super() constructor
    for (int i = 0; i < mode.size(); i++){
        scale_degrees_chords.insert(make_pair(1,tonics)); // @todo change this, temporary structure that works
    }
}

map<int, IntSet> MajorTonality::get_scale_degrees_chords(){
    return scale_degrees_chords;
}