#include "MajorTonality.hpp"

/**
 * @brief Construct a new Major Tonality object
 *
 * @param root an integer representing the root of the tonality.
 */
MajorTonality::MajorTonality(int root)
{
    key = root % 12 + 12;
    mode = MAJOR_SCALE;
    tonalityNotes = getAllNotesFromTonality(key, mode);
    scaleDegrees = {unisson, majorSecond, majorThird, perfectFourth, perfectFifth, majorSixth, majorSeventh};
    scaleDegreesChordQualities = {MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD};

    int i = 0;
    for (int deg : scaleDegrees)
    {
        scaleDegreeNotes.insert(make_pair(deg, getAllGivenNote(key + deg)));
        scaleDegreeChords.insert(make_pair(deg, getAllNotesFromChord(key, scaleDegreesChordQualities[i])));
        i += 1;
    }
}