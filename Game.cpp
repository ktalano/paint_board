#include "Game.hpp" 
#include "Scene.hpp"
using namespace sf; 

Game::Game(Scene* first_s) :window(VideoMode::getFullscreenModes()[0], "Sabaté_Catalano",Style::Fullscreen) {
    window.setFramerateLimit(60); 
    scene = first_s;
    nextscene = nullptr;
}



void Game::Run(){
    while(window.isOpen()) { 
        Event e;
        while(window.pollEvent(e)){
            scene->Event(e);
        }
		scene->Update(*this);
        if (!nextscene && !scene) { 
            window.close();
			
        }
        else { 
            window.clear(); 
            scene->Draw(window);
            window.display(); 
        } 
        if (nextscene) { 
            delete scene;
            scene = nextscene;
            nextscene = nullptr;
        }
	}
}


void Game::SetScene(Scene *scene){
    nextscene = scene;
}

void Game::Close() {
    delete scene;
    scene = nullptr;
}

Game::~Game(){
    delete scene;
    delete nextscene;
}
