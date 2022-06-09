#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "board.h"

using std::string;
using std::vector;

NGTile::NGTile(bool val, bool explored, bool guessVal){
    this->val = val;
    this->explored = explored;
    this->guessVal = guessVal;
}

void NGBoard::init(){
    this->board_ = new NGTile[this->rowNum_ * this->colNum_];
    this->nums_ = new vector<int>[this->rowNum_ + this->colNum_];
}
NGBoard::NGBoard(){
    this->rowNum_ = 5;
    this->colNum_ = 5;
    this->mistakes_ = 0;
    this->init();
}
NGBoard::NGBoard(int size){
    this->rowNum_ = size;
    this->colNum_ = size;
    this->mistakes_ = 0;
    this->init();
}
NGBoard::NGBoard(int row, int col){
    this->rowNum_ = row;
    this->colNum_ = col;
    this->mistakes_ = 0;
    this->init();
}
NGBoard::NGBoard(string seed){
    this->makeFromSeed(seed);
}
NGBoard::~NGBoard(){
    delete[] this->board_;
    delete[] this->nums_;
}

void NGBoard::buildNumLists(){
    int row, col, toAdd;
    for(row = 0; row < this->rowNum_; row++){
        col = 0;
        while(col < this->colNum_){
            toAdd = 0;
            while(col < this->colNum_ && !this->board_[this->rowNum_ * row + col].val) col++;
            while(col < this->colNum_ && this->board_[this->rowNum_ * row + col].val){
                toAdd++;
                col++;
            }
            if(this->nums_[row].empty() || toAdd) this->nums_[row].push_back(toAdd);
        }
    }

    for(col = 0; col < this->colNum_; col++){
        row = 0;
        while(row < this->rowNum_){
            toAdd = 0;
            while(row < this->rowNum_ && !this->board_[this->rowNum_ * row + col].val) row++;
            while(row < this->rowNum_ && this->board_[this->rowNum_ * row + col].val){
                toAdd++;
                row++;
            }
            if(this->nums_[this->rowNum_ + col].empty() || toAdd) this->nums_[this->rowNum_ + col].push_back(toAdd);
        }
    }
}

void NGBoard::print(bool revealed){
    using std::cout;
    using std::endl;
    NGTile tile;
    for(int row = 0; row < this->rowNum_; row++){
        for(int col = 0; col < this->colNum_; col++){
            tile = this->getTile(row,col);

            if(!revealed && !tile.explored) cout << ' ';
            else{
                if(tile.val) cout << (tile.guessVal ? '@' : 'X');
                else cout << (tile.guessVal ? 'O' : '0');
            }
        }
        cout << endl;
    }
    for(int ind = 0; ind < this->rowNum_ + this->colNum_; ind++){
        for(int val : this->nums_[ind]) cout << val << ' ';
        cout << endl;
    }
    cout << "Mistakes: " << this->mistakes_ << endl;
}

//TODO: create seeds using base64
string NGBoard::getSeed(){
    return string();
}
int NGBoard::rowNum(){
    return this->rowNum_;
}
int NGBoard::colNum(){
    return this->colNum_;
}
int NGBoard::mistakes(){
    return this->mistakes_;
}
bool NGBoard::solved(){
    return false;
}
NGTile NGBoard::getTile(int row, int col){
    return this->board_[row * this->rowNum_ + col];
}

//False for row, true for column
vector<int> NGBoard::getNumList(int val, bool rowOrCol){
    return this->nums_[val + rowOrCol * this->rowNum_];
}

void NGBoard::makeRandom(){
    int ind;
    std::srand(time(nullptr));
    for(int row = 0; row < this->rowNum_; row++){
        for(int col = 0; col < this->colNum_; col++){
            ind = row * this->rowNum_ + col;
            this->board_[ind].explored = false;
            this->board_[ind].val = rand() & 1;
        }
    }

    this->buildNumLists();
}
void NGBoard::makeFromSeed(string seed){
    return;
}
//string NGBoard::getSeed();

void NGBoard::guess(int row, int col, bool guess){
    int ind = row * this->rowNum_ + col;
    if(this->board_[ind].explored) return;
    this->board_[ind].explored = true;
    this->board_[ind].guessVal = (guess == this->board_[ind].val);
    if(!this->board_[ind].guessVal) this->mistakes_++;
}