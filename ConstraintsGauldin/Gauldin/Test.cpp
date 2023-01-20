/**
 * @file Test.cpp
 * @author Sprockeels Damien (damien.sprockeels@uclouvain.be)
 * @brief This file is used for testing
 * @version 0.1
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Test.h"
/* #include "Gauldin_csts.h"
#include "Utilities.h"
#include "Tonality.h" */

using namespace Gecode;
using namespace std;

/***************************************************************************************************************************************
 *                                                                                                                                     *
 * @brief This function creates the problem and searches for solutions respecting the constraints specified in the Gauldin_csts class  *
 *                                                                                                                                     *
 ***************************************************************************************************************************************/

int main(int argc, char *argv[])
{
    printIntVector(getAllNotesTonality(60, majorType));

    return 0;
}//TODO fix the error with major and minor as names