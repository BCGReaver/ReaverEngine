/**
 * @file Actor.h
 * @brief Definición de la clase Actor, que representa una entidad con componentes en la escena.
 *
 * @details
 * Un Actor es una entidad que:
 * - Puede tener múltiples componentes (Transform, CShape, etc.).
 * - Gestiona su propia lógica de actualización y renderizado.
 * - Sirve como unidad básica para representar objetos en la escena.
 *
 * Estilo aplicado:
 * - Métodos en lowerCamelCase.
 * - Variables miembro privadas con prefijo `m_`.
 */

#pragma once
#include "../Prerequisites.h"
#include "Entity.h"
#include "CShape.h"
#include "Transform.h"

 /**
  * @class Actor
  * @brief Representa una entidad compuesta por uno o más componentes.
  *
  * @details
  * Cada actor:
  * - Tiene un nombre único (`m_name`).
  * - Puede contener componentes que determinan su comportamiento o apariencia.
  * - Se actualiza y renderiza en cada frame según sus componentes.
  *
  * Ejemplo de uso:
  * @code
  * Actor player("Player");
  * player.getComponent<CShape>()->createShape(CIRCLE);
  * @endcode
  */
class Actor : public Entity {
public:
  /**
   * @brief Constructor por defecto.
   */
  Actor() = default;

  /**
   * @brief Constructor que asigna un nombre al actor y añade componentes básicos.
   * @param actorName Nombre del actor.
   */
  Actor(const std::string& actorName);

  /**
   * @brief Destructor por defecto.
   */
  virtual ~Actor() = default;

  /**
   * @brief Método de inicialización (actualmente sin uso).
   */
  void start() override {};

  /**
   * @brief Actualiza la lógica del actor.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   *
   * @details
   * Sincroniza la posición, rotación y escala del componente `CShape`
   * con los valores de su `Transform`.
   */
  void update(float deltaTime) override;

  /**
   * @brief Renderiza el actor.
   * @param window Puntero compartido a la ventana donde se dibujará.
   *
   * @details
   * Recorre los componentes y dibuja cualquier `CShape` asociado.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /**
   * @brief Método de destrucción (actualmente sin uso).
   */
  void destroy() override {};

  /**
   * @brief Asigna una textura al componente `CShape` del actor.
   * @param texture Puntero compartido a la textura a aplicar.
   */
  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

  /**
   * @brief Obtiene el nombre del actor.
   * @return Nombre del actor.
   */
  std::string getName() {
    return m_name;
  }

  /**
   * @brief Obtiene un componente específico del actor.
   * @tparam T Tipo del componente que se desea obtener.
   * @return Puntero compartido al componente, o nullptr si no existe.
   *
   * @details
   * Busca en la lista de componentes el primero que coincida con el tipo solicitado.
   */
  template <typename T>
  EngineUtilities::TSharedPointer<T> getComponent();

private:
  std::string m_name = "Actor"; ///< Nombre del actor (por defecto "Actor").
};

/**
 * @brief Implementación del método genérico para obtener un componente por tipo.
 * @tparam T Tipo del componente que se desea buscar.
 * @return Puntero compartido al componente encontrado, o vacío si no existe.
 */
template<typename T>
inline EngineUtilities::TSharedPointer<T>
Actor::getComponent() {
  for (auto& component : components) {
    EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
    if (specificComponent) {
      return specificComponent;
    }
  }
  // Devuelve un TSharedPointer vacío si no se encuentra el componente
  return EngineUtilities::TSharedPointer<T>();
}
