#ifndef INTERFACE_H
#define INTERFACE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "InputText.hpp"

/*Clase Interface:
*       Es una clase contenedor de los textos a mostrar en pantalla.
*       Encargandose de las fuentes, y tambien del inputText.
*       A su vez es un template, ya que el orden del vector de textos 
*       tambien esta asociado al orden de los eventos de la clase que instancia a Interface.
*/

template <typename EventType>
class Interface {

public:
    Interface() :m_inputText() {};

    /*
    * loadFont:
    *       Se encarga de ir guardando las fuentes
    * std::string& fontDir          direccion de la fuente
    */
    void loadFont(const std::string&fontDir){
        Font fuente;
        fuente.loadFromFile(fontDir);
        m_vector_fonts.push_back(fuente);
    }
    /*
    * push:
    *       El metodo push funciona como en cualquier clase contenedor, 
    *       Asignandole la fuente que va a utilizar 
    */
    void push(sf::Text* t) {
        t->setFont(m_vector_fonts[m_actual_font]);
        m_vectorTexts.push_back(t); 
    }

    /*
    * selectFont:
    *       Metodo para elegir un indice del vector de fuentes
    */
    void selectFont(int newFont){
        m_actual_font = static_cast<size_t>(newFont);
    }

    /*Metodo Draw
            Metodo para dibujar, se recorre el vector de textos sf::Text, y se llama al
            metodo draw de la clase sf::RenderWindow.
            InputText tambien requiere dibujarse
    */
    void Draw(sf::RenderWindow& window) {
        for (auto& text : m_vectorTexts) {
            window.draw(*text);
        }
        window.draw(m_inputText); //Tambien se tiene que dibujar el input
    }

    //Funcion Update de la clase Interface (Necesario para el InputText)
    void Update() {
        m_inputText.update();
    }

    /*IsMouseOver:
    *   Es un metodo que recorre el vector de textos, y dadas las coordenadas 
    *   recibidas por parametro, puede identificar que Texto esta siendo
    *   seleccionado.
    *   Devuelve un EventType que:
    *   En Interface representa el indice del vector de textos.
    *   En Clase que lo instancia representa uno de los eventos.
    *   (Ejem, presionar sobre jugar, salir, cargar, que tambien son textos en pantalla)
    */
    EventType IsMouseOver(sf::Vector2f& mousePosition) {
        EventType e = static_cast<EventType>(0);
        int i = 0;

        if (!m_vectorTexts.empty()) {
            for (auto& text : m_vectorTexts) {
                if (text->getGlobalBounds().contains(mousePosition)) {
                    e = static_cast<EventType>(i);
                    return e;
                }
                i++;
            }
        }
        return EventType::None;
    }



    //Funcion para pasar los eventos al InputText
    void processEvent(sf::Event& e) {
        m_inputText.processEvent(e);
    }

    /*
    *   Metodo para localizar el ultimo elemento del vector y crear un texto con el string pasado como parametro. 
    */
    void setError(std::string error) {
        m_vectorTexts.back()->setString(error);
        m_vectorTexts.back()->setFillColor(sf::Color::Red);
    }

    /* Metodo que recibe un evento que representa un indice en m_vectorTexts
    *  Siempre y cuando no quiera acceder a una posicion no inializada del vector
    *  Van a asignarse tanto la posicion como la dimension pasadas por parametros
    */
    void fixPosition(EventType evento, sf::Vector2f posicion, int dimension) {
        size_t indice = size_t(evento);
        if (indice < m_vectorTexts.size() ) {
            m_vectorTexts[indice]->setPosition(posicion);
            m_vectorTexts[indice]->setCharacterSize(dimension);
            m_vectorTexts[indice]->setOrigin(Vector2f(m_vectorTexts[indice]->getLocalBounds().width * 0.5f, m_vectorTexts[indice]->getLocalBounds().height * 0.5f));
        }
    }

    /*
    * Metodo que se encarga de incializar la parte que hereda inputText de sf::Text.
    */
    void inputText_inicializate(sf::Vector2f pos) {
        m_inputText.setFont(m_vector_fonts[m_actual_font]);
        m_inputText.setFillColor(sf::Color::Green);
        m_inputText.setPosition(pos);
    }

    /*
    *   Metodo que se encarga de cambiarle el tamaño al InputText
    */
    void inputText_setSize(int n) {
        m_inputText.setCharacterSize(n);
    }

    //Devuelve la cantidad de textos en el vector
    size_t Size() {
        return m_vectorTexts.size();
    }

    //Metodo que llama a borrar el InputText
    void inputText_reset() {
        m_inputText.reset();
    }

    //Metodo para liberar la memoria y eliminar los textos en pantalla
    void Clear() {
        for (auto& obj : m_vectorTexts) {
            delete obj;
        }
        m_vectorTexts.clear();
    }

    void inputText_setColor(sf::Color col){
        m_inputText.setFillColor(col);
    }

    //Metodo que llama al atributo de inputText, que devuelve el string ingresado por teclado
    std::string inputText_getInput(){
        return m_inputText.getValue();
    }


    private:
        /*
         *  Vector para guardar las fuentes.
         */
        std::vector<sf::Font> m_vector_fonts;

        /*
        *   Vector de punteros de sf::Text para guardar los textos.
        */
        std::vector<sf::Text*> m_vectorTexts;

        /*
        *   Un objeto tipo InputText para la entrada de texto
        */
        InputText m_inputText;

        //Indice de fuente seleccionada
        size_t m_actual_font = 0;
};

#endif
 
