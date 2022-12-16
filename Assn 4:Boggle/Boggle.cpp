//reference:https://github.com/efeacer/Stanford_CS106B_Assignments/tree/master/Assignment4
#include "Boggle.h"
#include "bogglegui.h"
#include "simpio.h"
#include "grid.h"
#include "random.h"
#include "shuffle.h"
#include "string.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
const int DELAY=100;

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Implementation notes:
 * If the string is empty, generate a random shuffled board.
 * Use the given 16-letter string to initialize the 16 board cubes.
 */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dimension=4;
    humanScore=0;
    computerScore=0;
    this->dictionary = dictionary;
    board.resize(dimension,dimension);
    if(boardText.empty()==false){
        //case1:manully entered
        int count=0;
        for(int i=0;i<dimension;i++){
            for(int j=0;j<dimension;j++){
                board.set(i,j,toUpperCase(boardText[count]));
                count++;
            }
        }
    }else{
        //case2:randomly generate one
        shuffle(CUBES,16);
        int count=0;
        for(int i=0;i<dimension;i++){
            for(int j=0;j<dimension;j++){
                int random=randomInteger(0,5);
                board.set(i,j,CUBES[count][random]);
                count++;
            }
        }
    }

}
//Boggle Method 1: return the character that is stored in your Boggle board at the given 0-based row and column. If the row and/or column are out of bounds, you should throw an int exception.
char Boggle::getLetter(int row, int col) {
    if( board.inBounds(row,col)==false){
        throw("Row and/or column are out of bounds.");
    }
        return board[row][col];

}

bool Boggle::checkWord(string word) {
    //convert to lower level
    word=toLowerCase(word);
    //whether the word is long enough to be a valid Boggle word
    if(word.size()>=4&&dictionary.contains(word)==true&&foundwords.contains(word)==false){
        return true;
    }
    return false;
}

//这个是找到这个单词的helper function，所以一个个删字母就可以了！！！而不是说把单词都保存下来。
//即使是private，也要在前面套Boggle::,而且后面括号里的要完全一致

bool Boggle::HumanWordSearch(int &row, int &col, Grid<bool>&isUsed,string word){
    BoggleGUI::setAnimationDelay(DELAY);
    BoggleGUI::setHighlighted(row,col,true);
    isUsed[row][col]=true;
    if(toLowerCase(board[row][col])==word.back()){
        char currentchar=word.back();
        word.pop_back();//delete the matched char
        //base case
        if(word.size()==0){
            return true;
        }else{
            //continue searching
            for(int i=row-1;i<=row+1;i++){
                for(int j=col-1;j<=col+1;j++){
                    if(board.inBounds(i,j)==true){
                        if(isUsed[i][j]==false&&HumanWordSearch(i,j,isUsed,word)==true){
                           return true;
                           }
                        }
                    }
                }
            }
        word.push_back(currentchar);
    }
    isUsed[row][col]=false;
    BoggleGUI::setAnimationDelay(DELAY);
    BoggleGUI::setHighlighted(row,col,false);
    return false;
    }


//我不明白为什么要把isUsed放在外面，我可以放在里面吗
bool Boggle::humanWordSearch(string word) {
     word=toLowerCase(word);
     //at the beginnning of each searching round, the previous highlighting should be cleared;
     BoggleGUI::clearHighlighting();
    //examine whether it is a valid word (has a proper size,a valid English word,has not entered before)
    if(checkWord(word)!=false){
     //examine whether it can be found in the Boggle Board
     //iterate 16 times to try different starting points
        for(int i=0;i<dimension;i++){
            for(int j=0;j<dimension;j++){
                Grid<bool> isUsed(dimension,dimension,false);
                if(HumanWordSearch(i,j,isUsed,word)==true){
                    humanScore=humanScore+word.size()-3;
                    foundwords.add(toLowerCase(word));
                    return true;
                }
            }
        }
   }
   return false;
}
//word其实不用choose unchoose,因为word前面并没有&，每个里面都自己有一个了
void Boggle::ComputerWordSearch(Set<string> &result,Grid<bool> &isUsed,int row,int col,string word){
    word.push_back(board[row][col]);
    isUsed[row][col]=true;
    if(checkWord(word)){
        result.add(toUpperCase(word));
        foundwords.add(toLowerCase(word));
        computerScore=computerScore+word.size()-3;
    }
    for(int i=row-1;i<=row+1;i++){
        for(int j=col-1;j<=col+1;j++){
            if(board.inBounds(i,j)==true&&isUsed[i][j]==false&&dictionary.containsPrefix(toLowerCase(word))){
                ComputerWordSearch(result,isUsed,i,j,word);
            }
        }
    }
    isUsed[row][col]=false;
}


int Boggle::getScoreHuman() {
    return humanScore;
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    Grid<bool> isUsed(dimension,dimension,false);
    string word;
    //iterate 16 times to try different starting points
    for(int i=0;i<dimension;i++){
        for(int j=0;j<dimension;j++){
                string word="";
                ComputerWordSearch(result,isUsed,i,j,word);
            }
        }
    return result;
}



int Boggle::getScoreComputer() {
    return computerScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int r=0;r<boggle.board.numRows();r++){
        for(int c=0;c<boggle.board.numCols();c++){
            out<<toUpperCase(charToString(boggle.getLetter(r,c)));
        }
        out<<"\n";
    }
    return out;
}
