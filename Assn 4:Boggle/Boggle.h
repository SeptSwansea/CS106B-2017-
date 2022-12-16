//reference:https://github.com/efeacer/Stanford_CS106B_Assignments/tree/master/Assignment4
//参考了efeacer的代码
#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "string.h"
#include "hashset.h"
using namespace std;

class Boggle {
public:
    /*Constructor:Boggle.Create a Boggle object.*/
    Boggle(Lexicon& dictionary, string boardText = "");

    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    friend ostream& operator<<(ostream& out, Boggle& boggle);



private:
    Grid<char> board;
    Lexicon dictionary;
    int dimension;
    int humanScore;
    int computerScore;
    HashSet<string> foundwords;
    //helperfunctions
    bool HumanWordSearch(int &row, int &col, Grid<bool>&isUsed,string word);
    void ComputerWordSearch(Set<string> &result,Grid<bool> &isUsed,int row,int col,string word);
};

#endif // _boggle_h
