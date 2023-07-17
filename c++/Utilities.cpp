#include "headers/Utilities.hpp"

/**
 * For a given set of intervals between notes that loops and a starting note, returns all the possible notes
 * @param n the starting note
 * @param intervals_loop the set of intervals between notes. It must make a loop. For example, to get all notes from a major
 * scale from note, use {2, 2, 1, 2, 2, 2, 1}. To get all notes from a minor chord, use {3, 4, 5}.
 * @return vector<int> all the notes
 */
IntSet getAllNotesFromIntervalLoop(int n, vector<int> intervals_loop)
{
    int note = n % 12 + 12; // bring the root back to [12,23] in case the argument is wrong
    vector<int> notes;

    int i = 0;
    while (note <= 127)
    {
        notes.push_back(note);
        note += intervals_loop[i % intervals_loop.size()];
        ++i;
    }
    IntSet set((const vector<int>)notes);
    return set;
}

/**
 * For a given tonality (root + mode), returns all the possible notes
 * @param root the root of the tonality (in [12,23])
 * @param scale the set of tones and semitones that define the scale
 * @return vector<int> all the possible notes from that tonality
 */
IntSet getAllNotesFromTonality(int root, vector<int> scale)
{
    return getAllNotesFromIntervalLoop(root, scale);
}

/**
 * For a given chord (root + mode), returns all the possible notes
 * @param root the root of the chord
 * @param quality the set of tones and semitones that define the chord
 * @return vector<int> all the possible notes from that chord
 */
IntSet getAllNotesFromChord(int root, vector<int> quality)
{
    return getAllNotesFromIntervalLoop(root, quality);
}

/**
 * Get all values for a given note
 * @param note a note
 * @return const vector<int> a vector containing all the given notes
 */
vector<int> getAllGivenNote(int note)
{
    int current = note % 12 + 12;
    vector<int> notes;
    while (current < 127)
    {
        notes.push_back(current);
        current += 12;
    }
    return notes;
}

/**
 * Transforms a vector of integers into a string
 * @param vector a vector of integers
 * @return string the string representation of the vector
 */
string int_vector_to_string(vector<int> vector){
    string s = "{";
    for (int i = 0; i < vector.size(); ++i) {
        s += to_string(vector[i]) + " ";
    }
    return s + "}";
}

vector<int> int_pointer_to_vector(int* ptr, int size){
    vector<int> v;
    for(int i = 0; i < size; i++){
        v.push_back(ptr[i]);
    }
    return v;
}

string statistics_to_string(Search::Statistics stats){
    string s = "Nodes traversed: " + to_string(stats.node) + "\n";
    s += "Failed nodes explored: " + to_string(stats.fail) + "\n";
    s += "Restarts performed: " + to_string(stats.restart) + "\n";
    s += "Propagators executed: " + to_string(stats.propagate) + "\n";
    s += "No goods generated: " + to_string(stats.nogood) + "\n";
    s += "Maximal depth of explored tree: " + to_string(stats.depth) + "\n";
    return s;
}

/**
 * Prints A note with its name (e.g. 60 = C)
 * @param var an integer variable
 */
void printNoteInLetter(IntVar var){
    std::cout << noteNames[var.val() % 12] << var.val() / 12 << " ";
}

/**
 * @brief Prints a node in MIDIcent value
 *
 * @param var an integer variable
 */
void printNoteForOM(IntVar var){
    std::cout << var.val() * 100 << " ";
}
