#include <SFML/Graphics.hpp>
#include "game/Game.hpp"
#include "game/Menu.hpp"

int main(int argc, char *argv[]){
    Game juego(new Menu); 
    juego.Run();
    return 0;
}
 
 
