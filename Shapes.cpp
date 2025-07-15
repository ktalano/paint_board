#include "Shapes.hpp"
#include <iostream>
#include <cmath>
/* Draw:
*	Recorre el vector de sf::Shapes* y llama al metodo draw de RenderWindow
*/
void Shapes::Draw(sf::RenderWindow& window){
	for (auto& obj : m_vectorShapes) {
		window.draw(*obj);
	}
}

/* Push:
*	Agrega un puntero Shape al final del vector de la clase
*/
void Shapes::push(sf::Shape* shape){
	m_vectorShapes.push_back(shape);
}

/* Move:
*	Recorre el vector de Shapes y suma un diferencial pasado por parametro
*	a las posiciones de los objetos
*/
void Shapes::Move(sf::Vector2f diff) { 
	for (auto& obj : m_vectorShapes) {
		obj->move(diff);
	}
}

//Retorna el tamanio del vector
size_t Shapes::getSize() {
	return m_vectorShapes.size();
}

//Elimina la memoria solicitada al sistema
Shapes::~Shapes(){
	for (auto& obj : m_vectorShapes) {
		delete obj;
	}
}

/* Save
* Devuelve un sf::Vector2i que es la cantidad de objetos guardados (x = Circulos e y = Rectangulos);
* Recibe una clase Binary, la cual ira escribiendo sobre el archivo los distitnos objetos.
*/
sf::Vector2i Shapes::Save(Binary& file) {
	int c = 0, r = 0; 
	for (auto& obj : m_vectorShapes) { 
		if (dynamic_cast<sf::RectangleShape*>(obj)) { 
			file.write(*obj, ShapeType::Rect); 
			r++;
		}
		else { 
			file.write(*obj, ShapeType::Circl);
			c++;
		}
	}
	return sf::Vector2i(c, r);
}
