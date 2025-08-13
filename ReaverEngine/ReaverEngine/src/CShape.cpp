/**
 * @file CShape.cpp
 * @brief Implementación del componente CShape para manejar figuras geométricas en SFML.
 *
 * @details
 * Este componente permite:
 * - Crear diferentes tipos de figuras (`ShapeType`).
 * - Configurar posición, color, rotación, escala y textura.
 * - Renderizar la figura en una ventana.
 *
 * Estilo aplicado:
 * - Nombres de métodos en lowerCamelCase.
 * - Variables miembro con prefijo `m_`.
 * - Código identado a 2 espacios.
 */

#include "CShape.h"
#include "Window.h"
#include "ECS/Texture.h"

 /**
  * @brief Crea una figura del tipo especificado.
  * @param type Tipo de figura (`CIRCLE`, `RECTANGLE`, `TRIANGLE`, `POLYGON`).
  *
  * @details
  * - Cada figura se inicializa en blanco y con un tamaño predeterminado.
  * - Se usa `dynamic_pointer_cast` para guardarla como `sf::Shape`.
  * - Si el tipo no se reconoce, se resetea el puntero y se lanza un error.
  */
void
CShape::createShape(ShapeType type) {
  m_shapeType = type;
  switch (type) {
  case ShapeType::CIRCLE: {
    auto circleSP = EngineUtilities::MakeShared<sf::CircleShape>(10.f);
    circleSP->setFillColor(sf::Color::White);
    m_shapePtr = circleSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::RECTANGLE: {
    auto rectSP = EngineUtilities::MakeShared<sf::RectangleShape>(sf::Vector2f(100.f, 50.f));
    rectSP->setFillColor(sf::Color::White);
    m_shapePtr = rectSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::TRIANGLE: {
    auto triSP = EngineUtilities::MakeShared<sf::ConvexShape>(3);
    triSP->setPoint(0, { 0,0 });
    triSP->setPoint(1, { 50,100 });
    triSP->setPoint(2, { 100,0 });
    triSP->setFillColor(sf::Color::White);
    m_shapePtr = triSP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  case ShapeType::POLYGON: {
    auto polySP = EngineUtilities::MakeShared<sf::ConvexShape>(5);
    polySP->setPoint(0, { 0,0 });
    polySP->setPoint(1, { 50,100 });
    polySP->setPoint(2, { 100,0 });
    polySP->setPoint(3, { 75,-50 });
    polySP->setPoint(4, { -25,-50 });
    polySP->setFillColor(sf::Color::White);
    m_shapePtr = polySP.dynamic_pointer_cast<sf::Shape>();
    break;
  }
  default:
    m_shapePtr.reset();
    ERROR("CShape", "createShape", "Tipo desconocido");
    return;
  }
}

/**
 * @brief Método llamado al iniciar el componente.
 * @note Actualmente vacío, pero se puede usar para inicializaciones.
 */
void
CShape::start() {
}

/**
 * @brief Actualiza la lógica del componente cada frame.
 * @param deltaTime Tiempo transcurrido desde el último frame.
 * @note Actualmente vacío.
 */
void
CShape::update(float deltaTime) {
}

/**
 * @brief Renderiza la figura en la ventana.
 * @param window Puntero compartido a la ventana donde se dibujará.
 */
void
CShape::render(const EngineUtilities::TSharedPointer<Window>& window) {
  if (m_shapePtr) {
    window->draw(*m_shapePtr);
  }
}

/**
 * @brief Libera recursos asociados a la figura.
 * @note Actualmente no realiza operaciones.
 */
void
CShape::destroy() {
}

/**
 * @brief Establece la posición de la figura usando coordenadas flotantes.
 */
void
CShape::setPosition(float x, float y) {
  if (m_shapePtr) {
    m_shapePtr->setPosition({ x, y });
  }
  else {
    ERROR("CShape", "setPosition", "Shape no inicializado");
  }
}

/**
 * @brief Establece la posición de la figura usando un `sf::Vector2f`.
 */
void
CShape::setPosition(const sf::Vector2f& position) {
  if (m_shapePtr) {
    m_shapePtr->setPosition(position);
  }
  else {
    ERROR("CShape", "setPosition", "Shape no inicializado");
  }
}

/**
 * @brief Cambia el color de relleno de la figura.
 */
void
CShape::setFillColor(const sf::Color& color) {
  if (m_shapePtr) {
    m_shapePtr->setFillColor(color);
  }
  else {
    ERROR("CShape", "setFillColor", "Shape no inicializado");
  }
}

/**
 * @brief Rota la figura en grados.
 */
void
CShape::setRotation(float angle) {
  if (m_shapePtr) {
    m_shapePtr->setRotation(sf::degrees(angle));
  }
  else {
    ERROR("CShape", "setRotation", "Shape no inicializado");
  }
}

/**
 * @brief Cambia la escala de la figura.
 */
void
CShape::setScale(const sf::Vector2f& scale) {
  if (m_shapePtr) {
    m_shapePtr->setScale(scale);
  }
  else {
    ERROR("CShape", "setScale", "Shape no inicializado");
  }
}

/**
 * @brief Asigna una textura a la figura.
 * @param texture Textura que se aplicará.
 */
void
CShape::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  if (!texture.isNull()) {
    m_shapePtr->setTexture(&texture->getTexture());
  }
}
