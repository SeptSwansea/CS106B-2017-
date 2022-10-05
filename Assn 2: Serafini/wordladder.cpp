//Assignment 2: Serafini
//why we use stack and queue here?
//stack-LIFO(to extract the last word in the partial-ladder)
//queue-FIFO(return the first element of the queue. The first solution you find will be as short as any other)

#include <fstream>
#include <iostream>
#include <string>
#include "simpio.h"
#include "filelib.h"
#include "queue.h"
#include "stack.h"
#include "lexicon.h"
#include "strlib.h"
#include "console.h"

using namespace std;


//function prototype declarations
int validinput(Lexicon &dictionary,string &word1,string &word2);
void ladder(Lexicon &dictionary,Lexicon &ExistingWords,string &word1,string &word2);
void menu(Lexicon &dictionary,Lexicon &ExistingWords);
const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";
//main function
int main() {
    // print introductory message
    cout<<"Welcome to CS 106B Word Ladder."<<endl;
    cout<<"Please give me two English words, and I will change the"<<endl;
    cout<<"first into the second by changing one letter at a time."<<endl;
    cout<<endl;
    //read in the dictionary file name
    string filename;
    filename=getLine("Dictionary file name?");
    //examine whether the file exists
    while (!fileExists(filename)){
        cout<<"Unable to open that file.  Try again."<<endl;
        getLine("Dictionary file name?",filename);
    }
    cout<<endl;
    //store the dictionary
     Lexicon dictionary(filename);
     //lexicon used to store the used words, avoid reusing words
     Lexicon ExistingWords;
    menu(dictionary,ExistingWords);
    cout << "Have a nice day." << endl;
    return 0;

}

void menu(Lexicon &dictionary,Lexicon &ExistingWords){
    //read in the word#1 and word#2
    string word1;
    string word2;
    getLine("Word #1 (or Enter to quit):", word1);
    //if the input is "enter", return to the main function and exit
    if(word1.size()==0){
       return;
    }
    getLine("Word #2 (or Enter to quit):", word2);
    if(word2.size()==0){
        return;
    }
    //transform to lower case
    word1=toLowerCase(word1);
    word2=toLowerCase(word2);
    //examine the inputs
    while(validinput(dictionary,word1,word2)==0){
        getLine("Word #1 (or Enter to quit):", word1);
        getLine("Word #2 (or Enter to quit):", word2);
        //transform to lower case
        word1=toLowerCase(word1);
        word2=toLowerCase(word2);
    }
    //validinput==1 means two words are valid inputs and are ready for the next process
    if(validinput(dictionary,word1,word2)==1){
       ladder(dictionary,ExistingWords,word1,word2);
    }
}
/*description:examine whether word1 and word2 are valid input*/
int validinput(Lexicon &dictionary,string &word1,string &word2){

    //1.make sure those two words are not the same;
    if (word1==word2){
        cout<<"The two words must be different."<<endl;
        return 0;
    }
    //2.make sure those two words are at the same length
    if(word1.length()!=word2.length()){
        cout<<"The two words must be the same length."<<endl;
        return 0;
    }
    //3.make sure those two words can be found in the dictionary
    if(!dictionary.contains(word1)||!dictionary.contains(word2)){
        cout<<"The two words must be found in the dictionary."<<endl;
        return 0;
    }

    return 1;
}

void ladder(Lexicon &dictionary,Lexicon &ExistingWords,string &word1,string &word2){
    //generate a stack which stores the word
    Stack<string> stack;
    stack.push(word1);
    ExistingWords.add(word1);
    Queue<Stack<string>> queue;
    queue.enqueue(stack);
    while(queue.isEmpty()==false){
        //dequeue the partial-ladder stack
        Stack<string> temstack;
        temstack=queue.dequeue();
        //copy the word which is on top of the partial-ladder stack
        string tem=temstack.peek();
        //find neighbours
        int length=tem.length();
        //change the n the letter
        for(int n=0;n<=length;n++){
            //avoid changing the original word which is on top of the partial-ladder stack, we create a copy
            string copytem=tem;
            //loops through the letters of the alphabet from a-z
            for(int i=0;i<26;i++){
                copytem[n]=ALPHABET[i];
                //check whether it is a valid English word and whether it has been used before
                if(dictionary.contains(copytem)==true&&!ExistingWords.contains(copytem)){
                    //case 1:we find the solution,we print it out and it will be the shortest path because it is based on breadth-first search
                    if(copytem==word2){
                        cout<<"A ladder from "<<word2<< " back to "<< word1<<":"<<endl;
                        //reverse the order of the words in the stack.
                        //print the first word
                        temstack.push(word2);
                        ExistingWords.add(word2);
                        while(!temstack.isEmpty()){
                            cout<<temstack.pop()<<" ";
                        }
                        cout<<endl;
                        return menu(dictionary,ExistingWords);
                    //case2: we didn't find a solution so far. We add a new word in to the partial-ladder stack and continue our search.
                    }else{
                        Stack<string> copy;
                        copy=temstack;
                        copy.add(copytem);
                        //record the used word
                        ExistingWords.add(tem);
                        //add the partial-ladder stack to our queue
                        queue.enqueue(copy);
                        //why we need copy here?
                        //the temstack will be used again and again in the loops.If we didn't make the copy, it will store all the valid words in one loop.
                    }
                 }

             }

        }
  }
  // no solution case
  if(!ExistingWords.contains(word2)){
      cout<<"No word ladder found from "<<word2<<" back to "<<word1<<"."<<endl;
  }
  //clear the used words lexicons and make it ready for the next ladder search
  ExistingWords.clear();
  return menu(dictionary,ExistingWords);
}
