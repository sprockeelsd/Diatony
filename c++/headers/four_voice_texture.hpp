#ifndef space_wrapper_hpp
#define space_wrapper_hpp

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <exception>

#include "gecode/kernel.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"
#include "gecode/minimodel.hh"
#include "gecode/set.hh"

#include "Utilities.hpp"
#include "Tonality.hpp"
#include "MajorTonality.hpp"

using namespace Gecode;
using namespace Gecode::Search;
using namespace std;

/***********************************************************************************************************************
 *                                                FourVoiceTexture class                                               *
 ***********************************************************************************************************************/
 // This class represents a constraint problem to be solved
class FourVoiceTexture: public Space {
protected:
    int size; // The size of the variable array of interest
    Tonality* tonality; // The tonality of the piece
    int lower_bound_domain;
    int upper_bound_domain;
    IntVarArray vars; // The variables of interest
    /* @todo Add here any additional attributes you need to represent your problem */

public:
    /**
     * Constructor
     * @todo Modify this constructor depending on your problem. This constructor is where the problem is defined
     * @todo (variables, constraints, branching, ...)
     * @param s the size of the array of variables
     * @param l the lower bound of the domain of the variables
     * @param u the upper bound of the domain of the variables
     */
    FourVoiceTexture(int s, int l, int u);

    /**
     * Copy constructor
     * @param s an instance of the FourVoiceTexture class
     * @todo modify this copy constructor to also copy any additional attributes you add to the class
     */
    FourVoiceTexture(FourVoiceTexture &s);

    /**
     * Returns the size of the problem
     * @return an integer representing the size of the vars array
     */
    int getSize();

    /**
     * Returns the values taken by the variables vars in a solution
     * @todo Modify this to return the solution for your problem. This function uses @param size to generate an array of integers
     * @return an array of integers representing the values of the variables in a solution
     */
    int* return_solution();

    /**
     * Copy method
     * @return a copy of the current instance of the FourVoiceTexture class. Calls the copy constructor
     */
    virtual Space *copy(void);

    /**
     * Constrain method for bab search
     * @todo modify this function if you want to use branch and bound
     * @param _b a space to constrain the current instance of the FourVoiceTexture class with upon finding a solution
     */
    virtual void constrain(const Space& _b);

    /**
     * Prints the solution in the console
     */
    void print_solution();

    /**
     * toString method
     * @return a string representation of the current instance of the FourVoiceTexture class.
     * Right now, it returns a string "FourVoiceTexture object. size = <size>"
     * If a variable is not assigned when this function is called, it writes <not assigned> instead of the value
     * @todo modify this method to also print any additional attributes you add to the class
     */
    string toString();
};


/***********************************************************************************************************************
 *                                                Search engine methods                                                *
 ***********************************************************************************************************************/

/**
 * Creates a search engine for the given problem
 * @todo Modify this function to add search options etc
 * @param pb an instance of the FourVoiceTexture class representing a given problem
 * @param type the type of search engine to create (see enumeration in headers/gecode_problem.hpp)
 * @return a search engine for the given problem
 */
Search::Base<FourVoiceTexture>* make_solver(FourVoiceTexture* pb, int type);

/**
 * Returns the next solution space for the problem
 * @param solver a solver for the problem
 * @return an instance of the FourVoiceTexture class representing the next solution to the problem
 */
FourVoiceTexture* get_next_solution_space(Search::Base<FourVoiceTexture>* solver);


/***********************************************************************************************************************
 *                                                 Auxiliary functions                                                 *
 ***********************************************************************************************************************/

/**
 * Write a text into a log file
 * @param message the text to write
 */
void writeToLogFile(const char* message);

#endif