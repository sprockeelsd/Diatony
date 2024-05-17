#include "TestCases.hpp"

using namespace std;
/**
 * This function generates a .txt file containing all the test cases (tonality - branching strategy - chord progression)
 * that are used as a benchmark
*/
int main(int argc, char* argv[]) {
    const char* homeDir = std::getenv("HOME"); // Get the user's home directory
    if (homeDir) {
        std::string filePath(homeDir);
        filePath += "/Documents/Libraries/MusicConstraints/efficiency_measurment/TestCases.txt"; // Specify the desired file path, such as $HOME/log.txt

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