# Diatony: a model of tonal harmony voicing
This repository contains a constraint formalization of general western tonal music rules for voice leading using constraint programming. It is mainly based on the books "L'harmonie en liberté" by Isabelle Duha and "Harmonic Practice in Tonal Music" by Robert Gauldin. The constraint solver used for modeling the rules is [Gecode](http://www.gecode.org/). This repository includes the [midifile](https://github.com/craigsapp/midifile) library to generate MIDI files. The model has been presented at IJCAI2024 and the article can be found in the proceedings. The full article and model can be found [here](http://hdl.handle.net/2078.1/287476)

## Contents
The **c++** folder contains the source code for the model, the Makefile to run the model, as well as the main file that implements an example of use. Currently, only MacOS compilation instructions are provided but it is straightforward to compile the code for other Operating Systems. See the [Gecode website](http://www.gecode.org/) for instructions on how to install and use Gecode.
### Source code
The headers and src folders are organised in three different folders:
- diatony: This folder contains the code for the model in itself, including the definition of the problem and the musical constraints.
- midifile: This file contains the [midifile](https://github.com/craigsapp/midifile) library that is used to generate MIDI files from solutions.
- aux: This folder contains auxiliary code used in the model. It contains the models for tonality, the constants and utility functions as well as functions to generate MIDI files that can be read by any DAW.

### Other files
- Makefile: The makefile contains several targets. Each target is explained in the comments, along with the arguments they might require.
- main.cpp: This file contains an example of how the model can be used to generate a 4-voice chord progression from a series of chord names. It contains the search strategy we used. The strategy uses restarts with a hybrid cutoff generator to find optimal solutions in a reasonable amount of time.

## Measurements
- the **efficiency_measurment** folder contains scripts used to perform measurements on the model.
