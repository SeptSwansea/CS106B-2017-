//Random Writter
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "map.h"
#include "strlib.h"
#include "vector.h"
#include "random.h"


using namespace std;
//function prototype declarations
bool CheckN(string &N);
Vector<string> output(Map<Vector<string>,Vector<string>> &map,int &length);
void menu(Map<Vector<string>,Vector<string>> &map);
int main() {
    // print the introductory message
    cout<<"Welcome to CS 106B Random Writer ('N-Grams').\nThis program makes random text based on a document.\nGive me an input file and an 'N' value for groups\nof words, and I'll create random text for you.\n";
    cout<<endl;
    //get the name of the input file
    string FileName;
    getLine("Input file?", FileName);
    //examine whether the file exists
    while(!fileExists(FileName)){
        cout<<"Unable to open that file.  Try again."<<endl;
        getLine("Input file?", FileName);
    }
    //open the text file.All words are stored in the vecotr content
    ifstream file;
    openFile(file, FileName);
    Vector<string> content;
    string word;
    while (file >> word) {
        content.add(word);
    }
  //read the value of N
    string N;
    getLine("Value of N?",N);
    while(!CheckN(N)){
        getLine("Value of N?",N);
    }
    //N which has been examined and transformed to interger type
    int number=stringToInteger(N);
    //create the map
    Map<Vector<string>,Vector<string>> map;
    //perform 2 more iterations to connect the last 2 prefixes in the end of the file to the first 2 words at the start of the file
    int OriginalSize=content.size();
    for(int i=0;i<number-1;i++){
        //add the last N-1 words in the file at the front
        //when you insert content[a] at the front, the index will change.It will automatically return the content[a-1] in the next loop
        content.insert(0,content[OriginalSize-1]);
    }

    for(int i=0;i<content.size()-number+1;i++){
        //generate the vector for keys
        Vector<string> tem;
        for(int n=0;n<number-1;n++){
            tem.push_back(content[i+n]);
        }
        //if the key already exists, add the word to the corresponding value Vector. For example:{to, be} : { just, or}->{to, be} : {or, just, or}
         if(map.containsKey(tem)==true){
             Vector<string> newvalue=map.get(tem);
             newvalue.add(content[i+number-1]);
             //remove the original one first
             map.remove(tem);
            //add the new one
             map.add(tem,newvalue);
         }else{
          //if it is a new key, add the key and the corresponding value
             map.add(tem,{content[i+number-1]});
         }

    }

    menu(map);
    cout << "Exiting." << endl;
    return 0;
}

/*function:chech whether N is an interger*/
bool CheckN(string &N){
    try{
       //check whether it is an double
       for(int i=0;i<N.length();i++){
           if(isdigit(N[i])==false){
               cout<<"Illegal integer format. Try again."<<endl;
               return false;
           }
       }
       int number=stringToInteger(N);
       if(number<=1){
           cout<<"N must be 2 or greater."<<endl;
           return false;
       }else{
           return true;
       }
    }catch(string N){
        cout<<"Illegal integer format. Try again."<<endl;
        return false;
    }
}
/*function:read in the length, pass the value to the output function,print the output,*/
void menu(Map<Vector<string>,Vector<string>> &map){
    string Strlength;
    getLine("# of random words to generate (0 to quit)?",Strlength);
    int length=stringToInteger(Strlength);
    while(length!=0){
        Vector<string> result=output(map,length);
        cout<<"...";
        //print the output by looping
        for(int count=0;count<result.size();count++){
            cout<<result[count]<<" ";
        }
        cout<<"..."<<endl;
        getLine("# of random words to generate (0 to quit)?",Strlength);
        length=stringToInteger(Strlength);
    }
    //quit
    return;

}
/*function:generate the output*/
Vector<string> output(Map<Vector<string>,Vector<string>> &map,int &length){

    Vector<string> output;
    Vector<string> chosen;
    Vector<Vector<string>> key = map.keys();
    //choose the first N-1 words
    int random=randomInteger(0,map.size()-1);
    int size=key[random].size();
    //store the first N-1 words in the key
    for(int n=0;n<size;n++){
     output.add(key[random][n]);
     chosen.add(key[random][n]);
    }

    for(int i=length;i>size;i--){
        //choose the next word 
        //1.first asking the map to return the value of the key "chosen"
        Vector<string> newvector=map[chosen];
        //2.Markov chain, the weighted probability is related to the frequency of the occurrence of the word
        string newword=newvector[randomInteger(0,newvector.size()-1)];
        output.add(newword);
        //slide the current "window" of N-1 words by discarding the first word in the window and appending the new suffix.
        chosen.remove(0);
        chosen.add(newword);
    }
    return output;
}
