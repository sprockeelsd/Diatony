#include "headers/four_voice_texture.hpp"
#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    std::cout << "Tests of Tonality and MajorTonality classes.\n\n";
    Tonality* tonality = new MajorTonality(C);

    std::cout << "\n\nTonic : " << tonality->get_degree_note(firstDegree) << ", leading tone : " << tonality->get_degree_note(seventhDegree) << std::endl;
    std::cout << "Expected : " << to_string(C) << ", " << to_string(B) << std::endl;

    std::cout << "\n Tonic values : " << tonality->get_degree_note(firstDegree) << ", degrees_notes[" << fifthDegree + 1 << " : " << tonality->get_degree_note(fifthDegree) << "]" << std::endl;

    std::cout << "\n Tonic values : " << tonality->get_scale_degree(firstDegree) << ", degrees_notes[" << fifthDegree + 1 << " : " << tonality->get_scale_degree(fifthDegree) << "]" << std::endl;

    std::cout << "\n Tonality notes :" << tonality->get_tonality_notes() << std::endl;

    std::cout << "\n Tonal notes : " << tonality->get_tonal_notes() << std::endl;

    std::cout << "\n Modal notes : " << tonality->get_modal_notes() << std::endl;

    std::cout << "\n Tonic chord : " << tonality->get_scale_degree_chord(firstDegree) << ", leading tone chord : " << tonality->get_scale_degree_chord(seventhDegree) << std::endl;

    return 0;
}
