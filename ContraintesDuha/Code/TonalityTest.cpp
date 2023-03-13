#include "TonalityTest.hpp"

/**********************************************
 *                                            *
 * @brief This function creates the problem   *
 *                                            *
 **********************************************/

void testTonality(Tonality& tonality){
    std::cout << "Key : " << tonality.getKey() << std::endl;
    std::cout << "Mode : " ; printIntVector(tonality.getMode());
    std::cout << "Scale Degrees : " ; printIntVector(tonality.getScaleDegrees());

}

int main(int argc, char *argv[])
{
    std::cout << "Beginning of Major tonality tests" << std::endl;

    MajorTonality majorTonality(F);
    testTonality(majorTonality);

    return 0;
}