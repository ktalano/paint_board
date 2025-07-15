#include "Match.hpp"
#include "Binary.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "algorithm"
#include<iostream>
#include<iomanip>
#include<math.h>
using namespace std;
using namespace sf;
/*
* Match():
* Inicializa m_colorSelected y m_vectorTexts.
* Llama a los metodos para cargar las fuentes, incializar la guia
* y acomodar sus posiciones.
*/
Match::Match():m_colorSelected(sf::Color::Red),m_vectorTexts(){
	this->loadFonts();
	m_vectorTexts.selectFont(3);
	this->guide();
	this->fixTexts();
	this->updatePencil();
}

/*
* Match(Binary):
* Inicializa m_colorSelected y m_vectorTexts.
* Llama a los metodos para cargar las fuentes, incializar la guia
* y acomodar sus posiciones.
* Procede a utilizar el metodo read de Binary para leer 
* todos los objetos guardados en el archivo e ir guardandolos en
* la memoria dinamica de m_vectorShapes
*/
Match::Match(Binary& f_shapes) :m_vectorTexts(),m_colorSelected(sf::Color::Red) {
	this->loadFonts(); m_vectorTexts.selectFont(3);
	this->guide();
	this->fixTexts();
	this->updatePencil();
	while (!f_shapes.end()) {

		auto obj = f_shapes.read();

		switch (obj.type) {

			case ShapeType::Circl: {
				sf::CircleShape* circle = new sf::CircleShape(obj.rad);
				circle->setFillColor(obj.col);
				circle->setPosition(obj.pos);
				circle->setOrigin(sf::Vector2f(circle->getLocalBounds().width / 2.0f, circle->getLocalBounds().height / 2.0f));
				m_vectorShapes.push(circle);
				break;
			}

			case ShapeType::Rect: {	 
				sf::RectangleShape* rectangle = new sf::RectangleShape(obj.dim);
				rectangle->setFillColor(obj.col);
				rectangle->setPosition(obj.pos);
				rectangle->setOrigin(sf::Vector2f(rectangle->getLocalBounds().width / 2.0f, rectangle->getLocalBounds().height / 2.0f));
				m_vectorShapes.push(rectangle);
				break;
			}
		}
	}
}

/* Carga las fuentes de manera general para ambos constructores
*/
void Match::loadFonts() {
	m_vectorTexts.loadFont("fonts/arial.ttf");
	m_vectorTexts.loadFont("fonts/highspeed.ttf");
	m_vectorTexts.loadFont("fonts/urial.ttf");
	m_vectorTexts.loadFont("fonts/minecraft.ttf");
}

/* Draw:
*	Se encarga de incializar m_windowSize una unica vez y de
*	llamar a los metodos draw de las clases Interface y Shapes
*/
void Match::Draw(RenderWindow& win) {
	m_mouse_actualPosition = sf::Vector2f(sf::Mouse::getPosition(win));
	if(m_pencil) win.draw(*m_pencil);
	if (!m_windowSized) {
		m_windowSize = sf::RectangleShape(sf::Vector2f(win.getSize()));
		m_windowSize.setPosition(sf::Vector2f(win.getSize()) / 2.0f);
		m_windowSize.setOrigin(sf::Vector2f(m_windowSize.getLocalBounds().width / 2.0f, m_windowSize.getLocalBounds().height / 2.0f));
		m_windowSized = !m_windowSized;
	}
	m_vectorShapes.Draw(win);
	m_vectorTexts.Draw(win);
}

/* Update()
*		Se encarga de primero, manejar los estados en el que se encuentra el juego,
*		y procesar los eventos generales del juego.
*/
void Match::Update(Game& game) {
	m_vectorTexts.Update();
	if(!m_savemode){
		
		this->updatePencil();
		
		if(m_guideDrawled){
			this->guide();
			this->fixTexts();
			m_vectorTexts.inputText_reset();
			m_vectorTexts.inputText_setColor(sf::Color(0,0,0,0));
		}

		if (m_mousePressed) drawLine();
		
		while(!m_queueEvents.empty()){
			switch(m_queueEvents.front()){
				case MatchEvent::Exit:
					game.SetScene(new Menu);
					break;
				default:
					break;
			}
			m_queueEvents.pop();
		}
	}else{
		m_vectorTexts.inputText_setColor(sf::Color(0,255,0,255));
	}
}


/* 
* El metodo Event se encarga de procesar los eventos recibidos de la clase sf::Event.
* Determinando en que estado de juego se encuentra (guardado/normal).
*/

void Match::Event(sf::Event &e){

	if (!m_savemode){

		switch (e.type){
			
		//Eventos de teclado 
		case Event::KeyPressed: {
				
				switch (e.key.code) {

				case sf::Keyboard::Up:
					m_vectorShapes.Move(Vector2f(0.0, 10.0));
					break;

				case sf::Keyboard::Down:
					m_vectorShapes.Move(Vector2f(0.0, -10.0));
					break;

				case sf::Keyboard::Left:
					m_vectorShapes.Move(Vector2f(10.0, 0.0));
					break;

				case sf::Keyboard::Right:
					m_vectorShapes.Move(Vector2f(-10.0, 0.0));
					break;

				case sf::Keyboard::Escape:
					m_queueEvents.push(MatchEvent::Exit);
					break;
					
				default:
					break;
					}
				break;
			}

		//Evento de Soltar Mouse
		case Event::MouseButtonReleased:{
			
			if(e.mouseButton.button == sf::Mouse::Left){
				m_mousePressed = false;
				m_guideDrawled = true;
			}
			break;
		}
		
		//Evento rueda del mouse
		case Event::MouseWheelScrolled:{
			delete(m_pencil);
			m_pencil = nullptr;
			if (e.mouseWheelScroll.delta > 0) {
				m_shapeSize+=5;
			}else{
				if(!(m_shapeSize < 5)){
					m_shapeSize-=5;
				}
			}
			break;
		}


		//Evento de presionar Mouse
		case Event::MouseButtonPressed:{

			//Devuelve si un texto ligado a un evento fue seleccionado
			MatchEvent interfaceTouched = m_vectorTexts.IsMouseOver(m_mouse_actualPosition);

			switch (interfaceTouched){

			case MatchEvent::Rectangle:{
				m_shapeSwitch = false;
				delete(m_pencil);
				m_pencil = nullptr;
				break;
			}
			case MatchEvent::Circle:{
				m_shapeSwitch = true;
				delete(m_pencil);
				m_pencil = nullptr;
				break;
			}

			case MatchEvent::Save:
				m_savemode = true;
				break;

			case MatchEvent::Exit:
				m_queueEvents.push(MatchEvent::Exit);
				break;

			case MatchEvent::Red:
				m_colorSelected = sf::Color::Red;
				break;

			case MatchEvent::Green:
				m_colorSelected = sf::Color::Green;
				break;

			case MatchEvent::Blue:
				m_colorSelected = sf::Color::Blue;
				break;

			case MatchEvent::Yellow:
				m_colorSelected = sf::Color::Yellow;
				break;

			case MatchEvent::White:
				m_colorSelected = sf::Color::White;
				break;

			default:
				m_mousePressed = true;
				m_mouse_previousPosition = m_mouse_actualPosition;
				break;
			}
			break;
		}

		default:
			break;
		}
	}
	//Condicional que maneja el modo guardado
	else if (e.key.code == sf::Keyboard::Return){
		m_binaryName = m_vectorTexts.inputText_getInput();
		m_savemode = false;
		Binary file(m_binaryName, inOut::in);
		if (file.open(in)) {
			results(m_vectorShapes.Save(file));
			m_vectorTexts.inputText_setColor(sf::Color(0,200,0,255));
		}else {
			m_vectorTexts.inputText_setColor(sf::Color(200,0,0,255));
			m_vectorTexts.setError("error al escribir archivo");
		}
	}else if(e.key.code == sf::Keyboard::Escape){
		m_savemode = false;
	}else{
		m_vectorTexts.processEvent(e); 
	}
}


/* 
* Metodo encargado de actualizar el Puntero del lapiz
*/
void Match::updatePencil(){
	if(!m_pencil){
		if (m_shapeSwitch) {
            m_pencil = new sf::CircleShape(m_shapeSize);
        } else {
            m_pencil = new sf::RectangleShape(sf::Vector2f(m_shapeSize, m_shapeSize));
        }
        m_pencil->setOrigin(sf::Vector2f(m_pencil->getLocalBounds().width / 2.0f, m_pencil->getLocalBounds().height / 2.0f));
	}
	m_pencil->setFillColor(m_colorSelected);
    m_pencil->setPosition(m_mouse_actualPosition);
}


/*
* Metodo encargado de crear una linea recta entre los distintos puntos del mouse leidos por el programa
* La propiedad matemematica utilizada es la interpolacion lineal.
*/
void Match::drawLine() {

    // Calcular la distancia total entre los puntos
    float dX = m_mouse_actualPosition.x - m_mouse_previousPosition.x;
    float dY = m_mouse_actualPosition.y - m_mouse_previousPosition.y;
    float distance = std::sqrt(std::pow(dX,2) + std::pow(dY,2));

    float step = 1.0f;
	int numSteps = static_cast<int>(distance / step);

    for (int i = 0; i <= numSteps; ++i) {
        float t = static_cast<float>(i) / numSteps;
        sf::Shape* shape;
        
        if (m_shapeSwitch) {
            shape = new sf::CircleShape(m_shapeSize);
        } else {
            shape = new sf::RectangleShape(sf::Vector2f(m_shapeSize, m_shapeSize));
        }
        shape->setFillColor(m_colorSelected);
        shape->setOrigin(sf::Vector2f(shape->getLocalBounds().width / 2.0f, shape->getLocalBounds().height / 2.0f));
        shape->setPosition(
            (1 - t) * m_mouse_previousPosition.x + t * m_mouse_actualPosition.x,
            (1 - t) * m_mouse_previousPosition.y + t * m_mouse_actualPosition.y
        );

        m_vectorShapes.push(shape);
    }

    // Actualizar la posición previa del ratón
    m_mouse_previousPosition = m_mouse_actualPosition;
}

/* results(sf::Vector2i)
*	Este metodo se encarga de mostrar los resultados de los objetos guardados
*	en el archivo binario y calcula el tamanio de los objetos.
*	Guarda todo en un string y pushea a m_vectorTexts un sf::Text con ese contenido.
*/
void Match::results(sf::Vector2i totales){

	int circulos(totales.x*sizeof(sf::CircleShape));
	int rectangulos(totales.y*sizeof(sf::RectangleShape));

	std::string s = { 
		std::to_string(totales.x+totales.y) + " Objetos en pantalla\n\n" +

		std::to_string(totales.x) + " Circulos\n" +
		std::to_string(circulos) + " Bytes\n" +
		std::to_string(circulos / 1024) + " Kilobytes\n" +
		std::to_string(circulos / 1024 / 1024) + " Megabytes\n\n" +

		std::to_string(totales.y) + " Rectangulos\n" +
		std::to_string(rectangulos) + " Bytes\n" +
		std::to_string(rectangulos / 1024) + " Kilobytes\n" +
		std::to_string(rectangulos / 1024 / 1024) + " Megabytes\n\n" +

		"Tamanio total del archivo : " + std::to_string(totales.x*sizeof(dataShape)/1024 + totales.y * sizeof(dataShape) / 1024) + " kB\n"
	};

	sf::Vector2f pos = sf::Vector2f(m_windowSize.getSize().x * 0.60f, m_windowSize.getSize().y * 0.5f);

	sf::Text* aux = new sf::Text();
	aux->setCharacterSize(35);
	aux->setString(s);
	aux->setFillColor(sf::Color::Green);
	aux->setPosition(pos);

	m_vectorTexts.selectFont(3);
	m_vectorTexts.Clear();
	m_vectorTexts.push(aux);
	m_guideDrawled = false;
	
}

/*
*	Inicializa los textos en m_vectorTexts, encargado de la interfaz.
*/
void Match::guide() {
	m_vectorTexts.Clear();
	
	std::vector<std::string> titles = {"Rectangulo","Circulo","Guardar","Salir",
	"Rojo","Verde","Azul","Amarillo","Blanco",""};
	
	for (auto& str : titles) {
		sf::Text* aux = new sf::Text();
		if (str == "Rojo") {
			aux->setFillColor(sf::Color::Red);
		}
		else if (str == "Verde") {
			aux->setFillColor(sf::Color::Green);
		}
		else if (str == "Azul") {
			aux->setFillColor(sf::Color::Blue);
		}
		else if (str == "Amarillo") {
			aux->setFillColor(sf::Color::Yellow);
		}
		else if (str == "Blanco") {
			aux->setFillColor(sf::Color::White);
		}
		else {
			aux->setFillColor(sf::Color(255, 255, 255));
		}
		aux->setString(str);
		m_vectorTexts.push(aux);
	}

}

/*
* Acomoda todas las posiciones de los textos y sus respectivos tamaños.
* El inputText tambien es un texto, pero para identificarlo mejor tiene su propio metodo.
*/
void Match::fixTexts() {
	float x = float(m_windowSize.getSize().x), y = float(m_windowSize.getSize().y);
	int dim = 35;

	m_vectorTexts.fixPosition(MatchEvent::Rectangle,sf::Vector2f(x * 0.20f, y * 0.05f),dim);
	m_vectorTexts.fixPosition(MatchEvent::Circle,sf::Vector2f(x * 0.35f, y * 0.05f),dim);
	m_vectorTexts.fixPosition(MatchEvent::Save,sf::Vector2f(x * 0.50f, y * 0.05f),dim);
	m_vectorTexts.fixPosition(MatchEvent::Exit,sf::Vector2f(x * 0.60f, y * 0.05f),dim);
	m_vectorTexts.fixPosition(MatchEvent::Red,sf::Vector2f(x * 0.9f, y * 0.2f),dim);
	m_vectorTexts.fixPosition(MatchEvent::Green,sf::Vector2f(x * 0.9f, y * 0.3f),dim);
	m_vectorTexts.fixPosition(MatchEvent::Blue,sf::Vector2f(x * 0.9f, y * 0.4f),dim);
	m_vectorTexts.fixPosition(MatchEvent::Yellow,sf::Vector2f(x * 0.9f, y * 0.5f),dim);
	m_vectorTexts.fixPosition(MatchEvent::White,sf::Vector2f(x * 0.9f, y * 0.6f),dim);
	

	m_vectorTexts.inputText_inicializate(sf::Vector2f(0.0f, 0.0f));
	m_vectorTexts.inputText_setSize(25);

}

Match::~Match()
{
	m_vectorShapes.~Shapes();
	m_vectorTexts.Clear();
}
