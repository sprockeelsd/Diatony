/**
 * @file Main.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This function creates the problem
 * @version 1.0
 * @date 2023-02-01
 *
 */
#include "Main.hpp"

/**********************************************
 *                                            *
 * @brief This function creates the problem   *
 *                                            *
 **********************************************/

int main(int argc, char *argv[])
{
    // INPUT
/*    vector<int> chordRoots = {C, G, A, E, F, C};
    vector<vector<int>> chordQualities = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD};
    vector<int> chordBass = {C, G, A, E, F, C};*/
    vector<int> chordRoots = {B,B,G};
    vector<vector<int>> chordQualities = {DIMINISHED_CHORD, DIMINISHED_CHORD, MAJOR_CHORD};
    vector<int> chordBass = {B,B,B};

    MajorTonality majorTonality(C);

    FourVoiceTexture *problem = new FourVoiceTexture(chordRoots.size(), majorTonality, chordRoots, chordQualities, chordBass); // Create the problem

    // Search options
    Gecode::Search::Options opts;
    opts.threads = 0; // As many as possible
    // Gecode::Search::TimeStop maxTime(10000); // Search for max 1s
    // opts.stop = &maxTime;

    // Create the search engine
    DFS<FourVoiceTexture> e(problem, opts);
    delete problem;

    int nbSol = 0;
    while (FourVoiceTexture *s = e.next())
    {
        std::cout << " Solution " << nbSol + 1 << " : " << std::endl;
        s->print();
        std::cout << std::endl;
        delete s;
        ++nbSol;
        if (nbSol >= 100)
            break;
    }

    std::cout << "Finished. Number of solutions found: " << nbSol << std::endl;

    return 0;
}