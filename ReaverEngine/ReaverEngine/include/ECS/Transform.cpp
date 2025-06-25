#include "Transform.h"

//Constructor
Transform::Transform()
  : Component(ComponentType::TRANSFORM), // Llama al constructor de la clase base con el tipo TRANSFORM
  m_position(0.0f, 0.0f),              // Inicializa la posición por defecto en (0,0)
  m_rotation(0.0f, 0.0f),              // Inicializa la rotación por defecto en (0,0)
  m_scale(1.0f, 1.0f)                  // Inicializa la escala por defecto en (1,1)
{
  // Puedes añadir lógica adicional aquí si es necesario al inicializar.
}

// --- Implementación de los métodos de la clase base Component ---
// Por ahora, estos métodos están vacíos ya que la clase Transform
// se encarga principalmente de gestionar datos. Su lógica de "actualización"
// o "renderizado" puede ser manejada por otros componentes que utilicen
// la información de Transform, como un SpriteRenderer.
void Transform::start()
{
  
}

void Transform::update(float deltaTime)
{
  
}

void Transform::render(const EngineUtilities::TSharedPointer<Window>& window)
{
  
}

void Transform::destroy()
{
  
}

//Getters y Setters para Position
const sf::Vector2f& Transform::getPosition() const
{
  return m_position;
}

void Transform::setPosition(const sf::Vector2f& newPosition)
{
  m_position = newPosition;
}

void Transform::setPosition(float x, float y)
{
  m_position.x = x;
  m_position.y = y;
}

//Getters y Setters para Rotation
const sf::Vector2f& Transform::getRotation() const
{
  return m_rotation;
}

void Transform::setRotation(const sf::Vector2f& newRotation)
{
  m_rotation = newRotation;
}

void Transform::setRotation(float x, float y)
{
  m_rotation.x = x;
  m_rotation.y = y;
}

//Getters y Setters para Scale
const sf::Vector2f& Transform::getScale() const
{
  return m_scale;
}

void Transform::setScale(const sf::Vector2f& newScale)
{
  m_scale = newScale;
}

void Transform::setScale(float x, float y)
{
  m_scale.x = x;
  m_scale.y = y;
}