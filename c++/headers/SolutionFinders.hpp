#ifndef MYPROJECT_SOLUTIONFINDERS_HPP
#define MYPROJECT_SOLUTIONFINDERS_HPP

#include "FourVoiceTexture.hpp"
#include "Utilities.hpp"
#include "Tonality.hpp"
#include "MajorTonality.hpp"
#include "MinorTonality.hpp"

using namespace Gecode;
using namespace std;

FourVoiceTexture* find_best_solution(FourVoiceTexture *pb, int size);

#endif
