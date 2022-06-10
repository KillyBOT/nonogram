#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include <string>

using byte_t = unsigned char;
using std::vector;
using std::string;

struct NGTile{
    bool val:1;
    bool explored:1;
    bool guessVal:1;

    NGTile(bool val = false, bool explored = false, bool guessVal = true);
};

class NGBoard{
    NGTile *board_;
    int rowNum_, colNum_, mistakes_;
    vector<int> *nums_;

    void init();
    void buildNumLists();

    public:
    NGBoard();
    NGBoard(int size);
    NGBoard(int row, int col);
    NGBoard(string seed);
    ~NGBoard();

    void print(bool revealed = false);

    int rowNum();
    int colNum();
    int mistakes();
    bool solved();
    NGTile getTile(int row, int col);
    vector<int> getNumList(int val, bool isCol);

    void makeRandom();
    void makeFromSeed(string seed);
    string getSeed();

    void guess(int row, int col, bool guess);
};

string to_base_64(const byte_t *data, size_t size);
void from_base_64(byte_t *data, size_t size, string str);

#endif