#ifndef MENU_H
#define MENU_H

#include "Scene.hpp"
#include "Interface.hpp" 
#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

/*
* Enum que representa los eventos que tendra la escena Menu
*/
enum class MenuEvent {
    Title,
    Play,
    Quit,
    Load,
    Error,
    None,
};



class Menu : public Scene { 

public:
	Menu();

    //Metodos heredados de la clase Scene
	void Update(Game&) override;
    void Event(sf::Event&);
	void Draw(sf::RenderWindow&) override;

    //Metodo encargado de organizar las posiciones de los textos en pantalla
    void fixTexts(float,float);
    
private:
    std::queue<MenuEvent> m_queueEvents;
    Interface<MenuEvent> m_vectorTexts;

    bool m_textsFixed = false;   //Determina si ya se han acomodado las posiciones o no.
    std::string m_binaryName; 
    sf::Vector2f m_mouse_actualPosition = sf::Vector2f(0, 0);
};



#endif
