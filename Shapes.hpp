#ifndef SHAPES_H
#define SHAPES_H
#include<list>
#include<vector>
#include<SFML/Graphics.hpp>
#include"Binary.hpp"

using namespace std;
using namespace sf;

/*
* Clase encargada del almacenamiento de instancias heredadas de sf::Shape
*/
class Shapes {
	//Puntero de sf::Shapes donde se guardan sf::RectangleShape y sf::CircleShape.
	vector<sf::Shape*> m_vectorShapes;

public:
	void Draw(RenderWindow& w);

	//Metodo push
	void push(Shape* shape);

	//Metodo encargado de sumarle a las posiciones un diferencial pasado por parametro.
	void Move(Vector2f diff);

	//Metodo que guarda en un archivo binario los objetos almacenados en m_vectorShapes.
	Vector2i Save(Binary& file);

	//Dimension del vector
	size_t getSize();

	~Shapes();
};
#endif
