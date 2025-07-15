#ifndef MATCH_H
#define MATCH_H

#include "Scene.hpp"
#include "Shapes.hpp"
#include "Binary.hpp"
#include "Interface.hpp"
#include "InputText.hpp"
#include "Game.hpp"
#include <vector>
#include <queue>

/*
* Enum que representa los eventos de la clase
*/
enum class MatchEvent {
    Rectangle,
    Circle,
    Save,
    Exit,
    Red,
    Green,
    Blue,
    Yellow,
    White,
    None,
};

class Match : public Scene {
public:
    Match();

    /*Constructor que recibe un objeto Binary*/
    Match(Binary& f_shapes);

    //Sobrecarga de la clase Scene
    void Update(Game& game) override;
    void Event(sf::Event &e) override;
    void Draw(sf::RenderWindow& win) override;

    //Metodo encargado de mostrar los resultados de guardar el archivo binario.
    void results(sf::Vector2i);

    //Metodo encargado de acomodar las posiciones y tamaños de los textos en pantalla.
    void fixTexts();

    //Metodo que inicializa la interfaz mostrada en pantalla.
    void guide();

    //Metodo que carga las fuentes necesarias para que funcione m_vectorTexts.
    void loadFonts();

    //Metodo encargado de rellenar de puntos entre los espacios del mouse.
    void drawLine();

    //Actualiza la posicion, tamaño y forma del pincel.
    void updatePencil();

    ~Match();

private:
    std::queue<MatchEvent> m_queueEvents;
    Interface<MatchEvent> m_vectorTexts;
    Shapes m_vectorShapes;


    sf::RectangleShape m_windowSize;  //Variable que representa el tamaño de la ventana.
    sf::Color m_colorSelected = sf::Color::Red;        //Color seleccionado para Pincel
    sf::Color m_windowColor;          //Color de fondo de pantalla
    float m_shapeSize = 1;            //Tamanio del pincel.
    sf::Shape * m_pencil = nullptr;   //Puntero al Shape que representa el Pincel

    bool m_mousePressed = false;      //Lapso en el cual se presiona el click.
    bool m_windowSized = false;       //Si la variable m_windowSize se ha inicializado.
    bool m_shapeSwitch = false;       //Cambia entre dibujar con Rectangulos o Circulos.
    bool m_savemode = false;          //Lapso en el cual el juego entra en modo guardado (desaparece la interfaz, eventos pasados a InputText).
    bool m_guideDrawled = true;       //Si la guia en pantalla esta dibujada.       

    std::string m_binaryName;         //String donde se va a almacenar el valor del InputText.
    sf::Vector2f m_mouse_actualPosition = sf::Vector2f(0, 0);
    sf::Vector2f m_mouse_previousPosition = sf::Vector2f(0,0);
    
};

#endif
