# Music Constraints
This repository contains a constraint formalization of western tonal music rules for harmony. It is mainly based on the book "L'harmonie en liberté" by Isabelle Duha, and partially based on the book "Harmonic Practice in Tonal Music" by Robert Gauldin.

## Contents
TODO

## Supported musical rules
As of now, the following rules are supported:

### General rules
Parallel octaves/fifths/unissons are forbidden

### Fundamental state
All rules for fundamental state chords are implemented, aside from a few exceptions which are :
- resolution of seventh degree chord to first degree chord.
- melodic augmented intervals should be avoided.
Supported rules are:
- The fundamental should be doubled in priority.
- In two successive chords, common notes should be kept in the same voices (except for the bass) and other voices should move as closely as possible (the last part is in the form of an optimization problem for now).
- If there are no common notes, then other voices should move in contrary motion to the bass.
- Diminished chords should be used with 3 voices whenever possible
- When moving from the 5th degree chord to the 6th degree chord, the seventh of the scale must rise to the tonic in the next chord in all voices if the mode is minor, and only in the soprano if the mode is major
