#include "Menu.hpp"
#include "Match.hpp"
#include "Binary.hpp"
#include "InputText.hpp"
#include "Game.hpp"
/*
*   Constructor Menu:
*       Carga las fuentes mediante el metodo loadFont de la clase Interface
*       E instancia objetos dinamicos de la clase Sf::Text para almacenar en m_vectorTexts
*/
Menu::Menu() : m_vectorTexts() {

	m_vectorTexts.loadFont("fonts/arial.ttf");
	m_vectorTexts.loadFont("fonts/highspeed.ttf");
	m_vectorTexts.loadFont("fonts/urial.ttf");
	m_vectorTexts.loadFont("fonts/minecraft.ttf");
    m_vectorTexts.selectFont(1); 

    std::vector<sf::String> titles = { "Proyecto Final","Jugar", "Salir", "Cargar","",""};
    for (const auto& obj : titles) {
        sf::Text* aux = new sf::Text();
        aux->setString(obj);
        m_vectorTexts.push(aux);
        m_vectorTexts.selectFont(2);
    }
    m_vectorTexts.selectFont(0);
    
}

/*
*  Funcion encargada de actualizar la memoria dinamica del programa, gran
*  parte recibiendo eventos de la clase MenuEvent.
*/
void Menu::Update(Game& game) {
    m_vectorTexts.Update(); 
    m_binaryName = m_vectorTexts.inputText_getInput();
    while(!m_queueEvents.empty()){
        switch (m_queueEvents.front())
        {
        case MenuEvent::Play:
            game.SetScene(new Match);
            break;
        case MenuEvent::Quit:
            game.Close();
            break;
        case MenuEvent::Load:{
            Binary file(m_binaryName, out); 
            if (file.open(out)) game.SetScene(new Match(file)); 
            else m_vectorTexts.setError("archivo inexistente"); 
            break;
        }
        default:
            break;
        }
        m_queueEvents.pop();
    }
}

/*
* Funcion encargada de recibir los eventos y procesarlos.
* Parte van derivados al Update y otros son procesados directamente en esta funcion.
*/
void Menu::Event(sf::Event &e){
    switch(e.type){
        
        //Eventos de Mouse
        case sf::Event::MouseButtonPressed:{
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                MenuEvent interfaceTouched = m_vectorTexts.IsMouseOver(m_mouse_actualPosition);
                switch(interfaceTouched){
                    case MenuEvent::Play:
                        m_queueEvents.push(MenuEvent::Play);
                        break;
                    
                    case MenuEvent::Quit:
                        m_queueEvents.push(MenuEvent::Quit);
                        break;
                    
                    case MenuEvent::Load:
                        m_queueEvents.push(MenuEvent::Load);
                        break;
                    default:
                        break;
                }
            }
            break;
        } 
        //Eventos de teclado
        case sf::Event::KeyPressed:{
            switch(e.key.code){

                case sf::Keyboard::Delete:
                    m_vectorTexts.inputText_reset();
                    break;

                case sf::Keyboard::Escape:{
                    m_queueEvents.push(MenuEvent::Quit);
                    break;
                }

                case sf::Keyboard::Return:
                    m_queueEvents.push(MenuEvent::Load);
                    break;
            }
        }
        default:{
            m_vectorTexts.processEvent(e);
            break;                    
        }
    }
}

/* Draw
*       Se encarga de dibujar los textos en pantalla.
*       llamando al metodo fixTexts para organizar sus posiciones
*       y tamaños.
*       
*/
void Menu::Draw(sf::RenderWindow& win) {
	if(!m_textsFixed){
        this->fixTexts(float(win.getSize().x), float(win.getSize().y));
    }
    m_vectorTexts.Draw(win);    
    m_mouse_actualPosition = sf::Vector2f(sf::Mouse::getPosition(win));
}


/* fixTexts
*       Su funcion es recibir el tamaño de la ventana, y asignar las posiciones 
*       a los distintos textos adaptandose a cada resolucion.
*       el metodo fixTexts de la clase Interface, permite recibir un enum que indicara
*       el indice donde se encuentra el texto buscado.
*       
*/
void Menu::fixTexts(float x, float y) {
    m_vectorTexts.fixPosition(MenuEvent::Title, sf::Vector2f(x * 0.5f, y * 0.08f),80);
    m_vectorTexts.fixPosition(MenuEvent::Play, sf::Vector2f(x * 0.1f, y * 0.3f),50);
    m_vectorTexts.fixPosition(MenuEvent::Quit, sf::Vector2f(x * 0.1f, y * 0.5f),50);
    m_vectorTexts.fixPosition(MenuEvent::Load, sf::Vector2f(x * 0.1f, y * 0.7f),50);
    m_vectorTexts.fixPosition(MenuEvent::Error, sf::Vector2f(x * 0.4f, y * 0.8f),50);
    
    //Se incializa el inputText (con los metodos de sf::Text) y se le asigna su posicion
    m_vectorTexts.selectFont(3);
    m_vectorTexts.inputText_inicializate(sf::Vector2f(x * 0.25f, y * 0.7f));
    m_vectorTexts.inputText_setSize(30);
}



