/**
 * @file Actor.cpp
 * @brief Implementación de la clase Actor, que representa una entidad con componentes.
 *
 * @details
 * Un Actor es una entidad genérica que:
 * - Puede tener múltiples componentes (ej. CShape, Transform, etc.).
 * - Gestiona su actualización y renderizado.
 * - Permite asignar texturas u otras propiedades a través de sus componentes.
 */

#include "ECS/Actor.h"

 /**
  * @brief Constructor que inicializa un Actor con nombre y componentes básicos.
  * @param actorName Nombre del actor.
  *
  * @details
  * - Asigna el nombre del actor.
  * - Agrega un componente `CShape` (forma geométrica).
  * - Agrega un componente `Transform` (posición, rotación, escala).
  */
Actor::Actor(const std::string& actorName) {
  // Nombre del Actor
  m_name = actorName;

  // Componente Shape
  EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>();
  addComponent(shape);

  // Componente Transform
  EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
  addComponent(transform);
}

/**
 * @brief Actualiza la lógica del actor.
 * @param deltaTime Tiempo transcurrido desde el último frame.
 *
 * @details
 * - Sincroniza la posición, rotación y escala del `CShape` con los datos de `Transform`.
 */
void Actor::update(float deltaTime) {
  auto transform = getComponent<Transform>();
  auto shape = getComponent<CShape>();

  if (transform && shape) {
    shape->setPosition(transform->getPosition());
    shape->setRotation(transform->getRotation().x);
    shape->setScale(transform->getScale());
  }
}

/**
 * @brief Renderiza el actor.
 * @param window Puntero compartido a la ventana donde se dibujará.
 *
 * @details
 * - Busca en la lista de componentes alguno que sea `CShape` y lo renderiza.
 */
void Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
  for (unsigned int i = 0; i < components.size(); i++) {
    auto shape = components[i].dynamic_pointer_cast<CShape>();
    if (shape) {
      shape->render(window);
    }
  }
}

/**
 * @brief Asigna una textura al `CShape` del actor.
 * @param texture Puntero compartido a la textura que se aplicará.
 *
 * @details
 * - Si el actor tiene un `CShape` y la textura no es nula, se aplica y se agrega como componente.
 */
void Actor::setTexture(const EngineUtilities::TSharedPointer<Texture>& texture) {
  auto shape = getComponent<CShape>();
  if (shape) {
    if (!texture.isNull()) {
      shape->setTexture(texture);
      addComponent(texture);
    }
  }
}
