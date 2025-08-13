/**
 * @file Entity.h
 * @brief Define la clase base `Entity` para todos los objetos que poseen componentes en el motor.
 *
 * @details
 * La clase `Entity` es una interfaz abstracta que:
 * - Obliga a implementar los métodos de ciclo de vida (`start`, `update`, `render`, `destroy`).
 * - Permite agregar y obtener componentes de forma genérica mediante `TSharedPointer`.
 * - Mantiene una lista interna de componentes asociados.
 */

#pragma once
#include "../Prerequisites.h"
#include "Component.h"

class Window;

/**
 * @class Entity
 * @brief Clase base abstracta para todos los objetos del motor que pueden tener componentes.
 *
 * @details
 * - Define la interfaz para inicializar, actualizar, renderizar y destruir.
 * - Gestiona un contenedor de componentes (`Component`) que extienden la funcionalidad de la entidad.
 * - Provee métodos genéricos para agregar y obtener componentes.
 *
 * @note Se utiliza `EngineUtilities::TSharedPointer` para la gestión de memoria inteligente.
 */
class Entity {
public:
  /**
   * @brief Destructor virtual por defecto.
   *
   * Esto garantiza que la destrucción sea correcta incluso al usar punteros a la clase base.
   */
  virtual ~Entity() = default;

  /**
   * @brief Inicializa la entidad y sus componentes.
   * @return `true` si la inicialización fue exitosa, `false` en caso contrario.
   *
   * @note Este método debe ser implementado por las clases derivadas.
   */
  virtual void start() = 0;

  /**
   * @brief Actualiza la lógica de la entidad.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   *
   * @note Debe ser implementado por las clases derivadas.
   */
  virtual void update(float deltaTime) = 0;

  /**
   * @brief Renderiza la entidad.
   * @param window Puntero compartido a la ventana donde se dibujará.
   *
   * @note Debe ser implementado por las clases derivadas.
   */
  virtual void render(const EngineUtilities::TSharedPointer<Window>& window) = 0;

  /**
   * @brief Libera recursos asociados a la entidad.
   *
   * @note Debe ser implementado por las clases derivadas.
   */
  virtual void destroy() = 0;

  /**
   * @brief Agrega un componente a la entidad.
   * @tparam T Tipo del componente. Debe derivar de `Component`.
   * @param component Puntero compartido al componente a agregar.
   *
   * @warning Si `component` no es del tipo correcto, no será agregado.
   */
  template<typename T>
  void addComponent(EngineUtilities::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
    components.push_back(component.template dynamic_pointer_cast<Component>());
  }

  /**
   * @brief Obtiene un componente del tipo especificado.
   * @tparam T Tipo del componente a obtener.
   * @return `TSharedPointer` al componente, o un puntero vacío si no existe.
   *
   * @note Busca de forma secuencial en la lista de componentes.
   */
  template<typename T>
  EngineUtilities::TSharedPointer<T> getComponent() {
    for (auto& component : components) {
      EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }

protected:
  bool isActive = true;  ///< Indica si la entidad está activa en el juego.
  uint32_t id = 0;       ///< Identificador único de la entidad.
  std::vector<EngineUtilities::TSharedPointer<Component>> components; ///< Lista de componentes de la entidad.
};
