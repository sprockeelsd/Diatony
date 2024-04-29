#include "aux/Utilities.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    const char* homeDir = std::getenv("HOME"); // Get the user's home directory
    if (homeDir) {
        std::string filePath(homeDir);
        filePath += "/Documents/Libraries/MusicConstraints/c++/TestCases.txt"; // Specify the desired file path, such as $HOME/log.txt

        vector<int> tonics = {C, C, A_FLAT, B_FLAT, E, C_SHARP}; //

        vector<int> modes = {MAJOR_MODE, MINOR_MODE, MAJOR_MODE, MINOR_MODE, MAJOR_MODE, MINOR_MODE}; //

        vector<int> var_sel = {RIGHT_TO_LEFT}; //DEGREE_MAX, DOM_SIZE_MIN, LEFT_TO_RIGHT,
        vector<int> val_sel = {VAL_RND};

        vector<vector<vector<int>>> testCases{
                {
                        {FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, SECOND_DEGREE,
                         FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE}, /// degrees
                        {MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD,
                         MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD}, /// major qualities
                        {MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD,
                         DIMINISHED_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD}, /// minor qualities
                        {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FIRST_INVERSION,
                         FIRST_INVERSION, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE} /// states
                },
                {
                        {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE,
                         SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE},
                        {MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD, MAJOR_CHORD, MINOR_CHORD,
                            MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD},
                        {MINOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD, MAJOR_CHORD,
                            MAJOR_CHORD, DIMINISHED_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD},
                        {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FIRST_INVERSION, FUNDAMENTAL_STATE,
                            FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE}
                },
                {
                        {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIFTH_DEGREE, FOURTH_DEGREE, FIRST_DEGREE,
                            SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIFTH_DEGREE,
                            FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE},
                        {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                            MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD,
                            MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD},
                        {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                            DIMINISHED_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                            MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD},
                        {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                            FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION,
                            FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE}
                },
                {
                        {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE,
                            SECOND_DEGREE, FIRST_DEGREE, FIFTH_DEGREE},
                        {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                            MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD},
                        {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                            DIMINISHED_CHORD, MINOR_CHORD, MINOR_CHORD},
                        {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                            FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE}
                },
                {
                        {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
                            THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE},
                        {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD,
                            MINOR_CHORD, MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD},
                        {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD,
                            MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD},
                        {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                            FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
                            FUNDAMENTAL_STATE}
                },
                {
                        {FIRST_DEGREE, SECOND_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE,
                            SIXTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE,
                            FIRST_DEGREE},
                        {MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD,
                            MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                            MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD},
                        {MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD,
                            MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                            DOMINANT_SEVENTH_CHORD, MINOR_CHORD},
                        {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, SECOND_INVERSION, FIRST_INVERSION,
                            FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                            SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE}
                }
        };

        std::ofstream myfile(filePath, std::ios::out); // append mode
        if(myfile.is_open()){
            for(int j = 0; j < testCases.size(); j++){
                for (int i = 0; i < tonics.size(); i++){
                    for(auto vsh: var_sel){
                        for(auto valsh : val_sel){
                            myfile << j << " " << tonics[i] << " " << modes[i] << " " << vsh << " "
                                   << valsh << " " ;
                            myfile << endl;
                        }
                    }
                }
            }
        }
    }
    return 0;
}