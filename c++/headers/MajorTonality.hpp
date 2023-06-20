#ifndef MAJORTONALITY
#define MAJORTONALITY

#include <gecode/int.hh>

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace Gecode;
using namespace std;

class MajorTonality : public Tonality {
protected:

public:
    MajorTonality(int t);

    map<int, IntSet> get_scale_degrees_chords();
};


#endif
