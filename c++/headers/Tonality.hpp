#ifndef TONALITY
#define TONALITY

#include <gecode/int.hh>

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace Gecode;
using namespace std;

class Tonality { // abstract class
protected:
    int tonic;
    vector<int> mode;
    
public:
    // abstract methods are virtual
    Tonality();
};

#endif
