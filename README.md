# Music Constraints
This repository contains a constraint formalization of general western tonal music rules for harmony using constraint programming. It is mainly based on the book "L'harmonie en liberté" by Isabelle Duha, and the book "Harmonic Practice in Tonal Music" by Robert Gauldin. The constraint solver used for modeling the rules is Gecode. This repository includes the midifile library (https://github.com/craigsapp/midifile) to generate MIDI files.

## Contents

The patch "example_patch" contains an example of generating a 4 voice chord progression. The key can be set by giving an integer as the first argument to the "new-4-voice" function. 0 corresponds to C, etc up to 11 for B. The mode can be set through functions, currently only major and minor mode are supported. The third argument is the list of degrees of the chords. For convenience, there is a function that substracts 1 from the list given so the degrees correspond to their number. Inversions are also given in a list, with 0 representing fundamental state, 1 representing first inversion, and so on.

The "create-solver" function creates the solver, and takes the output of the "new-4-voice" function as a first argument and the solver type as a second argument. 

The solutions can then be visualised by evaluating the voice object. For now, the branch-and-bound has to be used to find a solution that follows all the rules as closely as possible. Multiple evaluations can be necessary to find the last solution. Please note that intermediate solutions might not follow all rules, especially those related to doubling.

Right now, only 3 note chords in fundamental state and first inversion are supported.
