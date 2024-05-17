/**
 * This file contains the different test cases used to validate our model and assess its efficiency
*/

#include "../c++//headers/aux/Utilities.hpp"

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                     Tonalities                                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/
vector<int> tonics = {C, C, A_FLAT, B_FLAT, E, C_SHARP};
vector<int> modes = {MAJOR_MODE, MINOR_MODE, MAJOR_MODE, MINOR_MODE, MAJOR_MODE, MINOR_MODE}; //
// vector<int> tonics = {F_SHARP, D_FLAT, A_FLAT, E_FLAT, B_FLAT, F, C, G, D, A, E, B,
//                       B_FLAT, F, C, G, D, A, E, B, F_SHARP, C_SHARP, G_SHARP, D_SHARP};
// vector<int> modes = {MAJOR_MODE, MAJOR_MODE, MAJOR_MODE, MAJOR_MODE, MAJOR_MODE, MAJOR_MODE, MAJOR_MODE, MAJOR_MODE,
//                      MAJOR_MODE, MAJOR_MODE, MAJOR_MODE, MAJOR_MODE,
//                      MINOR_MODE, MINOR_MODE, MINOR_MODE, MINOR_MODE, MINOR_MODE, MINOR_MODE, MINOR_MODE, MINOR_MODE, 
//                      MINOR_MODE, MINOR_MODE, MINOR_MODE, MINOR_MODE};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                              Branching strategies                                                   *
 *                                                                                                                     *
 ***********************************************************************************************************************/

vector<int> var_sel = {RIGHT_TO_LEFT}; //DEGREE_MAX, DOM_SIZE_MIN, LEFT_TO_RIGHT,
vector<int> val_sel = {VAL_RND};

vector<int> search_strategies = {0,1};

/***********************************************************************************************************************
 *                                                                                                                     *
 *                                                     Test cases                                                      *
 *                                                                                                                     *
 ***********************************************************************************************************************/

string testCase1Name = "I5-V5-I5-V65-I6-II6-V64-V7+-I5";
vector<int> chords1 = {FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, SECOND_DEGREE,
                        FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
vector<int> chords_qualities_major1 = {MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD,
                                        MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
vector<int> chords_qualities_minor1 = {MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD,
                                        DIMINISHED_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
vector<int> states1 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, SECOND_INVERSION, FIRST_INVERSION,
                        FIRST_INVERSION, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
vector<vector<int>> testCase1 = {chords1, chords_qualities_major1, chords_qualities_minor1, states1};


string testCase2Name = "I5-IV5-VII6-I6-III5-VI5-II6-V7+-I5";
vector<int> chords2 = {FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE,
                        SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
vector<int> chords_qualities_major2 = {MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD, MAJOR_CHORD, MINOR_CHORD,
                                        MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
vector<int> chords_qualities_minor2 = {MINOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD, MAJOR_CHORD,
                                        MAJOR_CHORD, DIMINISHED_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
vector<int> states2 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FIRST_INVERSION, FUNDAMENTAL_STATE,
                        FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
vector<vector<int>> testCase2 = {chords2, chords_qualities_major2, chords_qualities_minor2, states2};


string testCase3Name = "I5-V6-VI5-V5-IV5-I6-II5-V5-I5-V6-VI5-V5-IV5-V7+-I5";
vector<int> chords3 = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIFTH_DEGREE, FOURTH_DEGREE, FIRST_DEGREE,
                        SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIFTH_DEGREE,
                        FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
vector<int> chords_qualities_major3 = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                        MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD,
                                        MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
vector<int> chords_qualities_minor3 = {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                                        DIMINISHED_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                        MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
vector<int> states3 = {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                        FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION,
                        FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
vector<vector<int>> testCase3 = {chords3, chords_qualities_major3, chords_qualities_minor3, states3};


string testCase4Name = "I5-V5-VI5-I5-III5-VI5-II5-I5-V5"; //todo test strategies with this one in Ab Major
vector<int> chords4 = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE, SIXTH_DEGREE,
                        SECOND_DEGREE, FIRST_DEGREE, FIFTH_DEGREE};
vector<int> chords_qualities_major4 = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                                MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD};
vector<int> chords_qualities_minor4 = {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                DIMINISHED_CHORD, MINOR_CHORD, MINOR_CHORD};
vector<int> states4 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                        FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
vector<vector<int>> testCase4 = {chords4, chords_qualities_major4, chords_qualities_minor4, states4};


string testCase5Name = "I5-V5-VI5-I5-VI5-VII6-III5-VI5-II6-V7+-I5";
vector<int> chords5 = {FIRST_DEGREE, FIFTH_DEGREE, SIXTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, SEVENTH_DEGREE,
                        THIRD_DEGREE, SIXTH_DEGREE, SECOND_DEGREE, FIFTH_DEGREE, FIRST_DEGREE};
vector<int> chords_qualities_major5 = {MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD,
                                        MINOR_CHORD, MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
vector<int> chords_qualities_minor5 = {MINOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD, DIMINISHED_CHORD,
                                MAJOR_CHORD, MAJOR_CHORD, DIMINISHED_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
vector<int> states5 = {FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                        FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE,
                        FUNDAMENTAL_STATE};
vector<vector<int>> testCase5 = {chords5, chords_qualities_major5, chords_qualities_minor5, states5};


string testCase6Name = "I5-II6-IV5-V+6-I6-III5-VI5-V65/-I5-IV5-I64-V7+-I5";
vector<int> chords6 = {FIRST_DEGREE, SECOND_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, THIRD_DEGREE,
                        SIXTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE,
                        FIRST_DEGREE};
vector<int> chords_qualities_major6 = {MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD,
                                MINOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD, MAJOR_CHORD,
                                MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
vector<int> chords_qualities_minor6 = {MINOR_CHORD, DIMINISHED_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD,
                                        MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD, MINOR_CHORD, MINOR_CHORD,
                                        DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
vector<int> states6 = {FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, SECOND_INVERSION, FIRST_INVERSION,
                        FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FIRST_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                        SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
vector<vector<int>> testCase6 = {chords6, chords_qualities_major6, chords_qualities_minor6, states6};

string testCase7Name = "I5-V64-I5-V+4-I5-IV5-V5-IV5-V5-IV5-V5-I5-Vda-V7+-I5";
vector<int> chords7 = {FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, 
                        FOURTH_DEGREE, FIFTH_DEGREE, FOURTH_DEGREE, FIFTH_DEGREE, FIRST_DEGREE, FIRST_DEGREE, FIFTH_DEGREE, 
                        FIRST_DEGREE};
vector<int> chords_qualities_major7 = {MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, DOMINANT_SEVENTH_CHORD, MAJOR_CHORD, MAJOR_CHORD, 
                        MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, MAJOR_CHORD, 
                        DOMINANT_SEVENTH_CHORD, MAJOR_CHORD};
vector<int> chords_qualities_minor7 = {MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, DOMINANT_SEVENTH_CHORD, MINOR_CHORD, MINOR_CHORD,
                        MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MAJOR_CHORD, MINOR_CHORD, MINOR_CHORD, 
                        DOMINANT_SEVENTH_CHORD, MINOR_CHORD};
vector<int> states7 = {FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE, THIRD_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE,
                        FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE, 
                        FUNDAMENTAL_STATE, SECOND_INVERSION, FUNDAMENTAL_STATE, FUNDAMENTAL_STATE};
vector<vector<int>> testCase7 = {chords7, chords_qualities_major7, chords_qualities_minor7, states7};


vector<vector<vector<int>>> testCases = {testCase1, testCase2, testCase3, testCase4, testCase5, testCase6};

vector<string> testCasesNames = {testCase1Name, testCase2Name, testCase3Name, testCase4Name, testCase5Name, testCase6Name};