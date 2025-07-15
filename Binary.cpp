#include "Binary.hpp"


Binary::Binary(const std::string& dir, const inOut& stream) :m_stream(stream) {

    //Depende con que flujo trabajemos, se abrira cada archivo.
    if (m_stream) {
        m_file_in.open(dir, std::ios::binary | std::ios::in);
    }
    else {
        m_file_out.open(dir, std::ios::binary | std::ios::trunc);
    }
}

void Binary::write(const sf::Shape& shape, const ShapeType& type) {

    //Inicializamos un struct dataShape y guardamos los atributos mas genericos
    dataShape aux;
    aux.type = type;
    aux.pos = shape.getPosition();
    aux.col = shape.getFillColor();

    //Dependiendo del tipo de dato, se guarda en el struct su dimensiono o su radio.
    switch (type) {
    case ShapeType::Rect:
        aux.dim = sf::Vector2f(shape.getGlobalBounds().width, shape.getGlobalBounds().height);
        break;
    case ShapeType::Circl:
        aux.rad = shape.getGlobalBounds().width / 2.0f;
        break;
    }

    //Procede a guardarse
    m_file_out.write(reinterpret_cast<const char*>(&aux), sizeof(dataShape));
}



dataShape Binary::read() {

    //Instanciamos una clase dataShape y leemos el archivo.
    dataShape aux;
    if (m_file_in.read(reinterpret_cast<char*>(&aux), sizeof(dataShape))) {
        return aux;
    }
    return aux;
}


bool Binary::open(const inOut& stream) {
    if (stream) {
        if (m_file_in.is_open()) return true;
    }
    else {
        if (m_file_out.is_open()) return true;
    }
    return false;
}

Binary::~Binary(){
    if (m_file_in) {
        if (m_file_in.is_open()) m_file_in.close();
    }
    if (m_file_out) {
        if (m_file_out.is_open()) m_file_out.close();
    }
}

inOut Binary::getStream()
{
    return m_stream;
}

bool Binary::end()
{
    return m_file_in.eof();
}



