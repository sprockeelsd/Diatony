/**
 * @file Main.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This function creates the problem
 * @version 1.0
 * @date 2023-01-20
 *
 */
#include "Main.h"



/**********************************************
 *                                            *
 * @brief This function creates the problem   *
 *                                            *
 **********************************************/

int main(int argc, char *argv[])
{
    // INPUT
    vector<int> chordRoots = {C,F,G,C};
    vector<vector<int>> chordQualities = {MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD};
    vector<int> chordBass = {C,F,G,C};

    FourVoiceTexture *problem = new FourVoiceTexture(4,C,MAJOR_SCALE, chordRoots, chordQualities, chordBass);

    // Search options
    Gecode::Search::Options opts;
    opts.threads = 0; // As many as possible
    Gecode::Search::TimeStop maxTime(1); // Search for max 1s
    opts.stop = &maxTime;

    // Create the search engine
    DFS<FourVoiceTexture> e(problem, opts);
    delete problem;

    int nbSol = 0;
    while (FourVoiceTexture *s = e.next())
    {
        s->printDevelop();
        delete s;
        ++nbSol;
    }

    std::cout << "Finished. Number of solutions found: " << nbSol << std::endl;

    return 0;
}