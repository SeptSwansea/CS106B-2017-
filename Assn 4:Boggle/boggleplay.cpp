//reference:https://github.com/efeacer/Stanford_CS106B_Assignments/tree/master/Assignment4

#include "lexicon.h"
#include"bogglegui.h"
#include"Boggle.h"
#include"simpio.h"
#include"console.h"

const int dimension=4;
//function decleration
bool isValidboardtext(string &boardtext);
void printboardtext(string &boardtext);


void playOneGame(Lexicon& dictionary) {
    //set up GUI
    BoggleGUI::initialize(dimension,dimension);
    //enter boardText
    string boardText;
    if(getYesOrNo("Do you want a random board?")){
        boardText="";
    }else{
        boardText=getLine("Please enter 16 letters");
        boardText=toLowerCase(boardText);
        while(isValidboardtext(boardText)!=true){
            cout<<"Invalid input. Please try again."<<endl;
            boardText=getLine("Please enter 16 letters");
            boardText=toLowerCase(boardText);
        }
    }
    //initilize a boggle game
    Boggle game(dictionary,boardText);
    //make all 16 letters appear on the GUI
    if(boardText==""){
        string tem;
        for(int i=0;i<dimension;i++){
            for(int j=0;j<dimension;j++){
                tem.push_back(game.getLetter(i,j));
            }
        }
        tem=toUpperCase(tem);
        BoggleGUI::labelAllCubes(tem);
    }else{
        BoggleGUI::labelAllCubes(toUpperCase(boardText));
    }
   //human's turn
   clearConsole();
   BoggleGUI::setStatusMessage("It's your turn!");
   Vector<string> result;
   cout<<"It's your turn!"<<endl;
   printboardtext(boardText);
   cout<<"Your words ("<<result.size()<<"):"<<result.toString()<<endl;
   cout<<"Your score:"<<game.getScoreHuman()<<endl;
   string word=getLine("Type a word (or enter to stop):");
   word=toUpperCase(word);
   while(word!=""){
       if(game.humanWordSearch(word)==true){
           clearConsole();
           cout<<"You found a new word! "<<word<<endl;
           result.add(word);
           printboardtext(boardText);
           cout<<"Your words ("<<result.size()<<"):"<<result.toString()<<endl;
           cout<<"Your score:"<<game.getScoreHuman()<<endl;
           BoggleGUI::setStatusMessage("You found a new word!");
           BoggleGUI::recordWord(word,BoggleGUI::HUMAN);
           BoggleGUI::setScore(game.getScoreHuman(),BoggleGUI::HUMAN);
           word=getLine("Type a word (or enter to stop):");
       }else{
           clearConsole();
           cout<<"This is not a valid answer. Please try agian"<<endl;
           printboardtext(boardText);
           cout<<"Your words ("<<result.size()<<"):"<<result.toString()<<endl;
           cout<<"Your score:"<<game.getScoreHuman()<<endl;
           BoggleGUI::setStatusMessage("This is not a valid answer. Please try agian");
           word=getLine("Type a word (or enter to stop):");
       }
   }
   //computer's turn
   cout<<endl;
   BoggleGUI::clearHighlighting();
   cout<<"It's my turn"<<endl;
   Set<string> ComputerSearch=game.computerWordSearch();
   cout<<"My words ("<<ComputerSearch.size()<<"):"<<ComputerSearch.toString()<<endl;
   cout<<"My score "<<game.getScoreComputer()<<endl;
   for(string word:ComputerSearch){
       BoggleGUI::recordWord(word,BoggleGUI::COMPUTER);
   }
   BoggleGUI::setScore(game.getScoreComputer(),BoggleGUI::COMPUTER);
   //annouce winner
   if(game.getScoreComputer()>game.getScoreHuman()){
       BoggleGUI::setStatusMessage("I win!!!! Best luck next time.");
       cout<<"I win!!!! Best luck next time."<<endl;
   }else{
       BoggleGUI::setStatusMessage("Nice work! You win. Congratulations!");
       cout<<"Nice work! You win. Congratulations!"<<endl;
   }




}

bool isValidboardtext(string &boardtext){
    if(boardtext.size()==16){
        for(int i=0;i<boardtext.size();i++){
            if(isalpha(!boardtext[i])){
                return false;
            }
        }
        return true;
    }
    return false;
}

void printboardtext(string &boardtext){
    int i=0;
    while(i<dimension*dimension){
        for(int j=i;j<i+dimension;j++){
            cout<<toUpperCase(boardtext[j]);
        }
        cout<<endl;
        i=i+dimension;
    }
}
