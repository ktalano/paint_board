#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include "Scene.hpp"
using namespace sf;

//Clase Game proporcionadapor la Catedra para el manejo de ventanas.

class Scene;

class Game{
    RenderWindow window; 
    Scene *scene,*nextscene; 
public:
    Game(Scene *first_s);
    void Run(); 
    void SetScene(Scene *s); 
    void Close();
    ~Game();
};

#endif

//Ir a Game.cpp
