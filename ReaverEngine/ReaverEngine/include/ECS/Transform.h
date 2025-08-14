/**
 * @file Transform.h
 * @brief Declara la clase Transform para manejar posición, rotación y escala de entidades.
 *
 * @details
 * El componente Transform es esencial para representar la posición,
 * orientación y tamaño de un Actor dentro del motor.
 * También implementa un método `seek` para movimiento hacia un objetivo.
 */

#pragma once
#include "Prerequisites.h"
#include "Component.h"
#include "Window.h"
#include <cmath> // <- necesario para std::sqrt

 /**
  * @class Transform
  * @brief Componente encargado de almacenar y manipular transformaciones 2D.
  *
  * @details
  * Este componente:
  * - Almacena la posición, rotación y escala como `sf::Vector2f`.
  * - Permite el acceso y modificación directa de estos valores.
  * - Incluye utilidades para interpolar la posición hacia un objetivo (`seek`).
  */
class Transform : public Component {
public:
  /**
   * @brief Constructor por defecto.
   *
   * Inicializa:
   * - Posición en (0, 0)
   * - Rotación en (0, 0)
   * - Escala en (1, 1)
   * y asigna el tipo de componente a `TRANSFORM`.
   */
  Transform()
    : position(0.0f, 0.0f),
    rotation(0.0f, 0.0f),
    scale(1.0f, 1.0f),
    Component(ComponentType::TRANSFORM) {
  }

  /** @brief Destructor por defecto. */
  virtual ~Transform() = default;

  /** @brief Método de inicio del componente (no utilizado en esta clase). */
  void start() override {}

  /**
   * @brief Actualización por frame (no utilizada en esta clase).
   * @param deltaTime Tiempo transcurrido desde el último frame.
   */
  void update(float /*deltaTime*/) override {}

  /**
   * @brief Método de renderizado (no utilizado en esta clase).
   * @param window Ventana donde potencialmente se podría renderizar.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& /*window*/) override {}

  /** @brief Método para liberar recursos (no utilizado en esta clase). */
  void destroy() {}

  /**
   * @brief Mueve la posición hacia un objetivo a velocidad constante.
   *
   * @param targetPosition Posición destino.
   * @param speed Velocidad de movimiento en unidades por segundo.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   * @param range Distancia mínima para detener el movimiento.
   */
  void seek(const sf::Vector2f& targetPosition, float speed, float deltaTime, float range) {
    sf::Vector2f direction = targetPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > range) {
      direction /= length;  // Normaliza el vector
      position += direction * speed * deltaTime;
    }
  }

  /** @brief Asigna la posición. */
  void setPosition(const sf::Vector2f& _position) { position = _position; }

  /**
   * @brief Asigna la rotación desde un vector (compatibilidad).
   * Usado si alguien almacena dos componentes de rotación; no es lo común.
   */
  void setRotation(const sf::Vector2f& _rotation) { rotation = _rotation; }

  /**
   * @brief Asigna la rotación como ángulo en grados (lo esperado por A_Racer).
   * Guarda el ángulo en rotation.x y pone rotation.y = 0 por coherencia.
   */
  void setRotation(float angleDegrees) { rotation = sf::Vector2f(angleDegrees, 0.0f); }

  /** @brief Asigna la escala. */
  void setScale(const sf::Vector2f& _scale) { scale = _scale; }

  /** @brief Obtiene la posición. */
  sf::Vector2f& getPosition() { return position; }
  const sf::Vector2f& getPosition() const { return position; }

  /** @brief Obtiene la rotación como vector. */
  sf::Vector2f& getRotation() { return rotation; }
  const sf::Vector2f& getRotation() const { return rotation; }

  /** @brief Obtiene el ángulo de rotación (en grados) almacenado en rotation.x. */
  float getRotationAngle() const { return rotation.x; }

  /** @brief Obtiene la escala. */
  sf::Vector2f& getScale() { return scale; }
  const sf::Vector2f& getScale() const { return scale; }

  /**
   * @brief Devuelve un puntero crudo a los datos de posición.
   * @return Puntero a `float` que apunta a `position.x`.
   * @warning El puntero es válido mientras el objeto exista.
   */
  float* getPosData() { return &position.x; }

  /** @brief Devuelve un puntero crudo a los datos de rotación (rotation.x). */
  float* getRotData() { return &rotation.x; }

  /** @brief Devuelve un puntero crudo a los datos de escala. */
  float* getScaData() { return &scale.x; }

private:
  sf::Vector2f position;  ///< Posición del objeto en coordenadas 2D.
  sf::Vector2f rotation;  ///< Rotación del objeto (en grados; ángulo en rotation.x).
  sf::Vector2f scale;     ///< Escala del objeto.
};
