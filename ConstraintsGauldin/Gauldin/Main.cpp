/**
 * @file Main.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This function creates the problem and searches for solutions respecting the constraints specified in the Gauldin_csts class
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Main.h"
#include "Gauldin_csts.h"
#include "Utilities.h"
#include "Tonality.h"

using namespace Gecode;
using namespace std;

/***************************************************************************************************************************************
 *                                                                                                                                     *
 * @brief This function creates the problem and searches for solutions respecting the constraints specified in the Gauldin_csts class  *
 *                                                                                                                                     *
 ***************************************************************************************************************************************/

int main(int argc, char *argv[])
{
    fillNoteNameToValue(); // initialize the array of pairs (noteName, value)

    // create a tonality, set a chord progression


    // get the other notes of the chord

    // set the domains by giving the notes as an argument to the constructor

    Gauldin_csts *m = new Gauldin_csts(4, 1, 2); // instantiate model (maybe take parameters from command line)

    // Search options
    Gecode::Search::Options opts;
    opts.threads = 0; // as many as available
    // Gecode::Search::TimeStop maxTime(1000);
    // opts.stop = &maxTime;

    // Create the search engine
    DFS<Gauldin_csts> e(m);
    delete m;

    int nbSol = 0;
    // search for and print all solutions
    while (Gauldin_csts *s = e.next())
    {
        s->print();
        delete s;
        ++nbSol;
        // maxTime.reset();

        // Stop the program early to see prints
        if (nbSol >= 2)
        {
            break;
        }
    }

    std::cout << "number of solutions : " << nbSol << "\n";
    return 0;
}