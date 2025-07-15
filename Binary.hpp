#ifndef BINARY_H
#define BINARY_H
#include<SFML/Graphics.hpp>
#include<fstream>
#include<iostream>


//Enum que representa las clases heredadas de Sf::Shape
enum ShapeType {
    Rect,
    Circl
};

//Enum para representar el flujo de dato
enum inOut {
	in,
	out
};

/*
Clase dataShape donde se almacenan los datos mas importante de una instancia *Shape;
*/
struct dataShape {
	ShapeType type;
	sf::Vector2f pos;
	sf::Color col;
	float rad = 0;
	sf::Vector2f dim = sf::Vector2f(0,0);
};

/*
Binary
Clase encargada de los flujos tanto de entrada como de salida del Programa.
En cada instancia de Binary solo va a utilizar uno de estos flujos, por lo
que su funcionalidad es con fines practicos de manejar con una sola Clase 
todos los requerimentos de trabajar con archivos binarios.
*/
class Binary {
	std::ifstream m_file_in;
	std::ofstream m_file_out;
	inOut m_stream;
public:
	/*
		Binary:
			El constructor es el que incializa los archivos 
		&dir = Direccion del archivo a abrir.
		inOut& = flujo de entrada o de salida 
	*/
	Binary(const std::string&dir,const inOut&);

	/*
		write:
			Es el encargado de instanciar dataShape, copiar los atributos
			mas importantes de un sf::Shape, y guardar en el archivo 
		sf::Shape& = La direccion de memoria heredada de sf::Shape
		ShapeTyep& = Tipo de clase
	*/
	void write(const sf::Shape&, const ShapeType&);
	
	/*
		read:
			Metodo que se encarga leer y devolver cada instancia 
			guardada de dataShape
	*/
	dataShape read();

	//Devuelve el tipo de flujo con el que trabajamos
	inOut getStream();
	
	//Si el archivo esta al final (para el metodo read)
	bool end();

	//Si el archivo esta abierto (con su respectivo flujo)
	bool open(const inOut& stream);
	~Binary();
};

#endif