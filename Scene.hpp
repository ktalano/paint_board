#ifndef SCENE_HPP
#define SCENE_HPP
#include <SFML/Graphics.hpp>
#include "Game.hpp"

//Clase Scene proporcionada por la catedra para el manejo de Escenas

class Game;

class Scene{
public:
    virtual void Update(Game &) = 0; 
    virtual void Event(sf::Event &) = 0;
    virtual void Draw(sf::RenderWindow &) = 0; 
};

#endif
