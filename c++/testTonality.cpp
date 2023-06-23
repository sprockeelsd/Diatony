#include "headers/four_voice_texture.hpp"
#include "headers/Utilities.hpp"
#include "headers/Tonality.hpp"
#include "headers/MajorTonality.hpp"

using namespace Gecode;
using namespace std;

int main(int argc, char* argv[]) {
    std::cout << "Tests of Tonality and MajorTonality classes.\n\n";
    Tonality* tonality = new MajorTonality(C);

    std::cout << "\n\nTonic : " << tonality->get_degree_note(tonic) << ", leading tone : " << tonality->get_degree_note(leadingTone) << std::endl;
    std::cout << "Expected : " << to_string(C) << ", " << to_string(B) << std::endl;

    std::cout << "\n Tonic values : " << tonality->get_degree_note(tonic) << ", degrees_notes[" << 5 << " : " << tonality->get_degree_note(5) << "]" << std::endl;

    std::cout << "\n Tonic values : " << tonality->get_scale_degree(tonic) << ", degrees_notes[" << 5 << " : " << tonality->get_scale_degree(5) << "]" << std::endl;

    std::cout << "\n Tonality notes :" << tonality->get_tonality_notes() << std::endl;

    std::cout << "\n Tonal notes : " << tonality->get_tonal_notes() << std::endl;

    std::cout << "\n Modal notes : " << tonality->get_modal_notes() << std::endl;

    std::cout << "\n Tonic chord : " << tonality->get_scale_degree_chord(tonic) << ", leading tone chord : " << tonality->get_scale_degree_chord(leadingTone) << std::endl;

    return 0;
}
