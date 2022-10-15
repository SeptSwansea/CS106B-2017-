/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains grammar generating code for CS106B.
 */

#include "grammarsolver.h"


using namespace std;
void helpfunction(string &word,Map<string,Vector<string>> &map,string &output);

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    string line;
    Vector<string> raw;
    //read in the file by lines
    while(getline(input,line)){
        raw.add(line);
    }
    Map<string,Vector<string>> map;
    //spilt the non-terminal and terminal words first
    for(int i=0;i<raw.size();i++){
        Vector<string> Split=stringSplit(raw[i], "::=");
    //store rules. The values in the map are rules and keys are non-terminal words
        Vector<string> Rules=stringSplit(Split[1], "|");
        map.add(Split[0],Rules);
    }
    //output is the vector to store the final output
    Vector<string> output={};
    for(int j=0;j<times;j++){
        string part=" ";
        //recursion function.
        helpfunction(symbol,map,part);
        output.add(part);
    }

    return output;
}

void helpfunction(string &word,Map<string,Vector<string>> &map,string &part){
    if(map.containsKey(word)==false){
        //base case: the word is not non-terminal word
        //add terminal word to the string 
        part=part+" "+word;
    }else{

        Vector<string> terminal=map.get(word);
        //pick a random rule;
        int random=randomInteger(0,terminal.size()-1);
        //words in each rule are seperated by space.
        Vector<string> tem=stringSplit(terminal[random], " ");
        //recursion starts. enter next loop to find the terminal words.
        for(int j=0;j<tem.size();j++){
            helpfunction(tem[j],map,part);
        }
    }
}



