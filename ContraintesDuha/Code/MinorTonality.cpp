#include "MinorTonality.hpp"

/**
 * @brief Construct a new Major Tonality object
 *
 * @param root an integer representing the root of the tonality.
 */
MinorTonality::MinorTonality(int root)
{
    key = root % 12 + 12;
    mode = MINOR_SCALE;
    tonalityNotes = getAllNotesFromTonality(key, mode);
    scaleDegrees = {unisson, majorSecond, minorThird, perfectFourth, perfectFifth, minorSixth, majorSixth, minorSeventh, majorSeventh};
    scaleDegreesChordQualities = {  };// Think about how I can represent different chords for the same scale degree (e.g. diminished and minor for second degree)

    // TODO check how I need to modify it
    int i = 0;
    for (int deg : scaleDegrees)
    {
        scaleDegreeNotes.insert(make_pair(deg, getAllGivenNote(key + deg)));
        scaleDegreeChords.insert(make_pair(deg, getAllNotesFromChord(key, scaleDegreesChordQualities[i])));
        i += 1;
    }
}