//Assignment 1:The Game of Life
//hint:generate a new grid to store the original version of the grid because coordinates in the grid are updated one by one. We should avoid the updated points affecting the results of the following ones.
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;


//function prototype declarations
void intro();
void fileinput();
void displaygrid(Grid<char> &grid);
bool wrap();
void nextgeneration(Grid<char> &originalgrid,Grid<char> &grid,int x,int y,bool wrap);
int neicount_nowrap(Grid<char> &grid,int row,int col);
int neicount_wrap(Grid<char> &grid,int row,int col);
void menu(Grid<char> &grid,bool wrap);
void animate(Grid<char> &grid,int num,bool wrap);
int main() {
    //print the introductory welcome message
    cout<<"Welcome to the CS 106B Game of Life,"<<endl;
    cout<<"a simulation of the lifecycle of a bacteria colony."<<endl;
    cout<<"Cells (X) live and die by the following rules:"<<endl;
    cout<<"- A cell with 1 or fewer neighbors dies."<<endl;
    cout<<"- Locations with 2 neighbors remain stable."<<endl;
    cout<<"- Locations with 3 neighbors will create life."<<endl;
    cout<<"- A cell with 4 or more neighbors dies."<<endl;
    cout<<endl;
    //variables declarations
    string filename;
    string request;
    string stringnumber;
    string line;
    ifstream stream;
    int row;
    int column;
    int i;
    int j;
    //read in the name of the file
    request="Grid input file name ";
    filename=getLine(request);
    //check whether the file exists
   while(!fileExists(filename)){
        cout<<"Unable to open that file.  Try again."<<endl;
        filename=getLine(request);
     }
    //store the content of the file in the empty stream.
    openFile(stream, filename);
    //The First line:number of rows tall
    getLine(stream, stringnumber);
    row=stringToInteger(stringnumber);
    //The Second line:number of columns wide
    getLine(stream, stringnumber);
    column=stringToInteger(stringnumber);

    //read in the initial setting.
    Grid<char> grid(row, column);
    for(i=0;i<row;i++){
        //read by rows
        getLine(stream, line);
        for(j=0;j<column;j++){
            grid[i][j]=line[j];
        }
    }
    stream.close();
    //ask wrap
    bool a=wrap();
    displaygrid(grid);
    menu(grid,a);
    cout<<"Have a nice life!"<<endl;
    return 0;
}


void displaygrid(Grid<char> &grid){
    int row;
    int col;
    int i;
    int j;
    row=grid.numRows();
    col=grid.numCols();
    for(i=0;i<row;i++){
        for(j=0;j<col;j++){
            cout<<grid[i][j];
        }
    cout<<endl;
    }

}
bool wrap(){
    string answer;
    //read in the answer;
    getLine("Should the simulation wrap around the grid(y/n)? ",answer);
    //transform to lowercase
    answer=toLowerCase(answer);
    if(answer=="y"){
        return true;
    }else if (answer=="n"){
        return false;
    }else{
        cout<<"Please enter 'y' or 'n'."<<endl;
        return wrap();
    }
}
int neicount_wrap(Grid<char> &grid,int x,int y){
    int i;
    int j;
    int col=grid.numCols();
    int row=grid.numRows();
    int count=0;
    for(i=-1;i<2;i++){
        for(j=-1;j<2;j++){
            if(i!=0||j!=0){
                if(grid.inBounds(x+i,y+j)){
                    if(grid[x+i][y+j]=='X'){
                        count++;
                }
             }else{
                    if(grid[(x+i+row)%row][(y+j+col)%col]=='X'){
                        count++;
                    }
                 }
            }
         }
     }
    return count;
}
int neicount_nowrap(Grid<char> &grid,int x,int y){
    int i;
    int j;
    int count=0;

    for(i=-1;i<2;i++){
        for(j=-1;j<2;j++){
            //exclude the cell itself
            if(i!=0||j!=0){
                if(grid.inBounds(x+i,y+j)){
                    if(grid[x+i][y+j]=='X'){
                        count=count+1;
                     }
                 }
             }
         }
    }
    return count;
}
void nextgeneration(Grid<char> &originalgrid,Grid<char> &grid,int x,int y,bool wrap){

    if(wrap){
    //wrap case
        int cell=neicount_wrap(originalgrid,x,y);
        //Case 1: A cell with 1 or fewer neighbors or with 4 or more neighbors dies
        if(cell<=1||cell>=4){
            grid[x][y]='-';
        //Case 2:Locations with 2 neighbors remain stable.
        }else if(cell==2){
            //do nothing
        //Case 3:Locations with 3 neighbors will create life.
        }else{
            grid[x][y]='X';
        }
    }else{
    //unwrap case
        int cell=neicount_nowrap(originalgrid,x,y);
        //Case 1: A cell with 1 or fewer neighbors or with 4 or more neighbors dies
        if(cell<=1||cell>=4){
            grid[x][y]='-';
        //Case 2:Locations with 2 neighbors remain stable.
        }else if(cell==2){
            //do nothing
        //Case 3:Locations with 3 neighbors will create life.
        }else{
            grid[x][y]='X';
        }
    }
}
void menu(Grid<char> &grid,bool wrap){
    string answer;
    int num;
    getLine("a)nimate, t)ick, q)uit? ",answer);
    answer=toLowerCase(answer);
    if(answer=="a"){
        getLine("How many frames? ",answer);
        num=stringToInteger(answer);
        animate(grid,num,wrap);
        return menu(grid,wrap);
    }else if (answer=="t"){
        animate(grid,1,wrap);
        return menu(grid,wrap);
    }else if (answer=="q"){
     //return to the main
    }else{
        cout<<"Illegal integer format. Try again."<<endl;
        return menu(grid,wrap);
    }

}
void animate(Grid<char> &grid,int num,bool wrap){
    for(int c=num;c>0;c--){
        int row;
        int col;
        int i;
        int j;
        row=grid.numRows();
        col=grid.numCols();
        //generate a new grid to store the orginial version of the grid.Becase cordinates in the grid are updated one by one. We should avoid the updated points affect the results of the following ones.
        Grid<char> originalgrid=grid;
        for(i=0;i<row;i++){
            for(j=0;j<col;j++){
                nextgeneration(originalgrid,grid,i,j,wrap);
            }
        }
    displaygrid(grid);
    pause(50);
    }
}
