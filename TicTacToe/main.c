#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>

#define SCREEN_HEIGHT 480
#define SCREEN_WIDTH 640
//Button constants
#define  BUTTON_WIDTH 213
#define  BUTTON_HEIGHT 160
#define  TOTAL_BUTTONS 9

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

typedef struct Game{

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* background;
    SDL_Texture* cross;
    SDL_Texture* circle;

    int turn;


}Game;

typedef struct LButton{
    SDL_Point mPosition;
    
    enum LButtonSprite mCurrentSprite;
}LButton;

char choiseArr[9];

Game game;

LButton gButton[TOTAL_BUTTONS];
LButton gPlay;

void init();
void shutdown();
void loadMedia();
SDL_Texture* loadTexture(char* path);
SDL_Texture* loadBackground(char* path);

void checkWin();
void initLButton(LButton* button);
void handleEvent(LButton* button, SDL_Event* e, int i);
void render();
void win(char winner);


int main(int argc , char** argv)
{
    game.turn = 0;
    init();

    bool quit;   
    SDL_Event e;

    for(int i = 0; i < TOTAL_BUTTONS-1; i++)
    {
        initLButton(&gButton[i]);
    }

    loadMedia();



    while(!quit)
    {

        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }

            for(int i = 0; i< TOTAL_BUTTONS; i++)
            {
                handleEvent(&gButton[i], &e, i);
            }
        } 

        SDL_RenderClear( game.renderer );
        SDL_RenderCopy(game.renderer, game.background, NULL, NULL);

        render();

        SDL_RenderPresent( game.renderer );
        checkWin();

    }
}


void init()
{
    game.window = NULL;
    game.renderer = NULL;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("failed to initialize SDL %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    game.window = SDL_CreateWindow("I MADE A GAME", SDL_WINDOWPOS_UNDEFINED ,SDL_WINDOWPOS_UNDEFINED , SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(game.window == NULL)
    {
        printf("failed to create window %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(game.renderer == NULL)
    {
        printf("failed to create renderer %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("failed to initialize IMG %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if(TTF_Init() != 0)
    {
        printf("failed to initialize TTF %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}



void shutdown()
{

    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);
    game.window = NULL;
    game.renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void loadMedia()
{
    game.background = loadBackground("background.png");
    game.circle = loadTexture("O.png");
    game.cross = loadTexture("X.png");
 
    gButton[0].mPosition.x = 0;
    gButton[0].mPosition.y = 0;

    gButton[1].mPosition.x = BUTTON_WIDTH;
    gButton[1].mPosition.y = 0;

    gButton[2].mPosition.x = 2*BUTTON_WIDTH;
    gButton[2].mPosition.y = 0;

    gButton[3].mPosition.x = 0;
    gButton[3].mPosition.y = BUTTON_HEIGHT;
    
    gButton[4].mPosition.x = BUTTON_WIDTH;
    gButton[4].mPosition.y = BUTTON_HEIGHT;
    
    gButton[5].mPosition.x = 2*BUTTON_WIDTH;
    gButton[5].mPosition.y = BUTTON_HEIGHT;
    
    gButton[6].mPosition.x = 0;
    gButton[6].mPosition.y = 2*BUTTON_HEIGHT;
    
    gButton[7].mPosition.x = BUTTON_WIDTH;
    gButton[7].mPosition.y = 2*BUTTON_HEIGHT;

    gButton[8].mPosition.x = 2*BUTTON_WIDTH;
    gButton[8].mPosition.y = 2*BUTTON_HEIGHT;
    
} 


void initLButton(LButton* button)
{ 
    button->mPosition.y = 0;
    button->mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

SDL_Texture* loadBackground(char* path)
{ 
    SDL_Texture *newTexture;
   
    newTexture = IMG_LoadTexture(game.renderer ,path);

    return newTexture;
}
SDL_Texture* loadTexture(char* path)
{ 
    SDL_Texture *newTexture;

    SDL_Surface* loadedSurface = IMG_Load( path );
    SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format,0xFF, 0xFF, 0xFF ) );        
    newTexture = SDL_CreateTextureFromSurface( game.renderer, loadedSurface );


    return newTexture;
}
void render()
{  
    for(int i = 0; i < 9; i++)
    {
        if(choiseArr[i] == 'X')
        { 
            SDL_Rect renderQuad = {(i%3)*BUTTON_WIDTH, (i/3)*BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
            SDL_RenderCopy(game.renderer, game.cross, NULL, &renderQuad);           
        }
        else if(choiseArr[i] == 'O' )
        { 
            SDL_Rect renderQuad = {(i%3)*BUTTON_WIDTH, (i/3)*BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
            SDL_RenderCopy(game.renderer, game.circle, NULL, &renderQuad);
        }
    }
}


void handleEvent(LButton* button, SDL_Event* e, int i)
{
    //if mouse event happened
    if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        //get mouse position            //MOUSE POSITION IS FUCKED
        int x, y;
        SDL_GetMouseState(&x, &y);
        //check if mouse is in button

        if(x > (button->mPosition.x) && x < (button->mPosition.x + BUTTON_WIDTH) &&  y > (button->mPosition.y) && y < (button->mPosition.y + BUTTON_HEIGHT ))
        {
            //Set mouse over sprite
            if( e->type == SDL_MOUSEBUTTONDOWN)
            {
                if(choiseArr[i] != 'X' && choiseArr[i] != 'O')
                {
                    if(game.turn % 2 == 0 )
                    {
                        choiseArr[i] = 'X';
                        printf("we are in square %d \n also even \n", i);
                    }
                    else if (game.turn % 2 == 1)
                    {
                        choiseArr[i] = 'O';
                        printf("we are in square %d \n also odd \n", i);
                    } 
                    printf("turn is %d \n", game.turn);
                    game.turn++;
                }
            }
        }
    }
}
 

void checkWin()
{
    //straight horizontal wins
    if(choiseArr[0] == 'X' && choiseArr[1] == 'X' && choiseArr[2] == 'X' )
    {
        win('X');
    }
    if(choiseArr[0] == 'O' && choiseArr[1] == 'O' && choiseArr[2] == 'O' )
    {
        win('O');
    }
    if(choiseArr[3] == 'X' && choiseArr[4] == 'X' && choiseArr[5] == 'X' )
    {
        win('X');
    }
    if(choiseArr[3] == 'O' && choiseArr[4] == 'O' && choiseArr[5] == 'O' )
    {
        win('O');
    }
    if(choiseArr[6] == 'X' && choiseArr[7] == 'X' && choiseArr[8] == 'X' )
    {
        win('X');
    }
    if(choiseArr[6] == 'O' && choiseArr[7] == 'O' && choiseArr[8] == 'O' )
    {
        win('O');
    }
    //vertical wins
    if(choiseArr[0] == 'X' && choiseArr[3] == 'X' && choiseArr[6] == 'X' )
    {
        win('X');
    }
    if(choiseArr[0] == 'O' && choiseArr[3] == 'O' && choiseArr[6] == 'O' )
    {
        win('O');
    }
    if(choiseArr[1] == 'X' && choiseArr[4] == 'X' && choiseArr[7] == 'X' )
    {
        win('X');
    }
    if(choiseArr[1] == 'O' && choiseArr[4] == 'O' && choiseArr[7] == 'O' )
    {
        win('O');
    }
    if(choiseArr[2] == 'X' && choiseArr[5] == 'X' && choiseArr[8] == 'X' )
    {
        win('X');
    }
    if(choiseArr[2] == 'O' && choiseArr[5] == 'O' && choiseArr[8] == 'O' )
    {
        win('O');
    }
    //diagonal wins
    if(choiseArr[0] == 'X' && choiseArr[4] == 'X' && choiseArr[8] == 'X' )
    {
        win('X');
    }
    if(choiseArr[0] == 'O' && choiseArr[4] == 'O' && choiseArr[8] == 'O' )
    {
        win('O');
    }
    if(choiseArr[6] == 'X' && choiseArr[4] == 'X' && choiseArr[2] == 'X' )
    {
        win('X');
    }
    if(choiseArr[6] == 'O' && choiseArr[4] == 'O' && choiseArr[2] == 'O' )
    {
        win('O');
    }
    if(game.turn == 9)
    {
        win('n');
    }

}

void win(char winner)
{   
    printf("winner is %c \n", winner);
    for(int i = 0; i < TOTAL_BUTTONS; i++)
    {
        choiseArr[i] = 0;
    }
    game.turn = 0;
    // exit(EXIT_SUCCESS);
}