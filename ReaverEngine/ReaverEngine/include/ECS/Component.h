/**
 * @file Component.h
 * @brief Define la clase base `Component` y el enumerador `ComponentType`.
 *
 * @details
 * - `Component` es la clase abstracta base que define la interfaz para todos los componentes del motor.
 * - Cada componente tiene un tipo (`ComponentType`) que lo identifica.
 * - Los componentes son gestionados por entidades (`Entity`) y pueden ser de varios tipos:
 *   Transform, Sprite, Renderer, Physics, AudioSource, Shape, Texture, etc.
 */

#pragma once
#include "../Prerequisites.h"

class Window;

/**
 * @enum ComponentType
 * @brief Lista de tipos de componentes disponibles en el motor.
 *
 * @details
 * Este enumerador define los tipos reconocidos de componentes que pueden existir.
 * Cada componente hereda de `Component` y especifica su tipo en el constructor.
 */
enum ComponentType {
  NONE = 0,       ///< Componente sin tipo definido.
  TRANSFORM = 1,  ///< Componente de transformación (posición, rotación, escala).
  SPRITE = 2,     ///< Componente para renderizar sprites.
  RENDERER = 3,   ///< Componente de renderizado genérico.
  PHYSICS = 4,    ///< Componente para simulación física.
  AUDIOSOURCE = 5,///< Componente de reproducción de audio.
  SHAPE = 6,      ///< Componente para formas geométricas.
  TEXTURE = 7     ///< Componente que gestiona texturas.
};

/**
 * @class Component
 * @brief Clase base abstracta para todos los componentes del motor.
 *
 * @details
 * - Proporciona la interfaz que deben implementar todos los componentes.
 * - Permite inicializar, actualizar, renderizar y destruir un componente.
 * - Incluye un identificador `ComponentType` para reconocer su tipo en tiempo de ejecución.
 *
 * @note Los componentes son agregados a las entidades (`Entity`) y gestionados mediante `TSharedPointer`.
 */
class Component {
public:
  /**
   * @brief Constructor por defecto.
   */
  Component() = default;

  /**
   * @brief Constructor que establece el tipo de componente.
   * @param type Tipo del componente (`ComponentType`).
   */
  Component(const ComponentType type) : m_type(type) {}

  /**
   * @brief Destructor virtual para permitir herencia segura.
   */
  virtual ~Component() = default;

  /**
   * @brief Inicializa el componente.
   * @return `true` si la inicialización fue exitosa, `false` en caso contrario.
   *
   * @note Debe ser implementado por todas las clases derivadas.
   */
  virtual void start() = 0;

  /**
   * @brief Actualiza la lógica del componente.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   *
   * @note Debe ser implementado por todas las clases derivadas.
   */
  virtual void update(float deltaTime) = 0;

  /**
   * @brief Renderiza el componente.
   * @param window Puntero compartido a la ventana en la que se dibujará.
   *
   * @note Debe ser implementado por todas las clases derivadas.
   */
  virtual void render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

  /**
   * @brief Libera los recursos asociados al componente.
   *
   * @note Debe ser implementado por todas las clases derivadas.
   */
  virtual void destroy() = 0;

  /**
   * @brief Obtiene el tipo del componente.
   * @return Valor de `ComponentType` que identifica el tipo.
   */
  ComponentType getType() const { return m_type; }

protected:
  ComponentType m_type = NONE; ///< Tipo del componente.
};
