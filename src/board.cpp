#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "board.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

static const string cBase64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//Convert to base64
string to_base_64(const byte_t *data, size_t size){
    size_t ind = 0;
    string str;

    while(ind < size){
        str += cBase64Chars[data[ind] >> 2]; //First character

        if(ind < size - 2){ //Second, third, and fourth characters
            str += cBase64Chars[((data[ind] & 0b11) << 4) | (data[ind+1] >> 4)];
            str += cBase64Chars[((data[ind+1] & 0b1111) << 2) | (data[ind+2] >> 6)];
            str += cBase64Chars[data[ind+2] & 0b111111];
        } else if (ind < size - 1) {
            str += cBase64Chars[((data[ind] & 3) << 4) | (data[ind+1] >> 4)];
            str += cBase64Chars[(data[ind+1] & 0b1111) << 2];
            str += '=';
        } else {
            str += cBase64Chars[(data[ind] & 3) << 4];
            str += "==";
        }

        ind += 3;
    }

    return str;
    
}
//Convert base64 string into data field
void from_base_64(byte_t *data, size_t size, string str){
    if(str.empty()) return;
    if(str.size() * 3 > size * 4) return;

    uint32_t buff;
    int toWrite = 3;
    size_t dataInd = 0;
    for(size_t ind = 0; ind < str.size(); ind+=4){
        buff = 0;
        for(size_t n = 0; n < 4; n++){
            buff <<= 6;
            if(str[ind+n] != '=') buff |= (cBase64Chars.find_first_of(str[ind+n]) & 0b111111);
            else toWrite--;
        }
        for(size_t n = 0; n < toWrite; n++){
            data[dataInd] = (buff >> (8 * (2-n))) & 0xFF;
            dataInd++;
        }

    }
}

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