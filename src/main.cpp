#include <iostream>
#include <SDL.h>

#include "board.h"

int main(){
    /*SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
        "Testing...",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyWindow(window);
    SDL_Quit();*/

    NGBoard testBoard;

    testBoard.makeRandom();
    testBoard.print(true);

    string testStr = "The quick brown fox jumps over the lazy turtle";
    string b64 = to_base_64((byte_t*)testStr.c_str(),testStr.size()+1);
    using std::cout;
    using std::endl;
    cout << b64 << endl;
    char buff[48];
    from_base_64((byte_t*)buff, 48, b64);
    cout << buff <<endl;

    return 0;
}