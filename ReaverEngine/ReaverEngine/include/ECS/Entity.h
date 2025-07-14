#pragma once
#include "../Prerequisites.h"


#include <vector>
#include <string>
#include <type_traits> // Para std::is_base_of
#include <memory/TSharedPointer.h> // Asegúrate de que esta ruta sea correcta a tu TSharedPointer

// Asumiendo que Component.h define la clase Component base
#include "ECS/Component.h" // Ajusta la ruta si es diferente

// Otras inclusiones necesarias para tus componentes si Entity las usa directamente
// #include "Transform.h" // Si Entity necesita conocer Transform
// #include "CShape.h"    // Si Entity necesita conocer CShape

class Entity {
public:
  // Constructor
  Entity() = default; // O tu constructor si tiene lógica específica

  // Destructor
  virtual ~Entity() = default;

  /**
   * @brief Añade un componente a la entidad.
   * @tparam T Tipo del componente a añadir (debe derivar de Component).
   * @param component El TSharedPointer al componente a añadir.
   */
  template<typename T>
  void addComponent(EngineUtilities::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
    components.push_back(component.static_pointer_cast<Component>()); // Siempre almacenar como TSharedPointer<Component>
  }

  /**
   * @brief Obtiene un componente de la entidad por su tipo.
   * @tparam T Tipo del componente a obtener (debe derivar de Component).
   * @return Un TSharedPointer al componente si se encuentra, o un TSharedPointer nulo.
   */
  template<typename T>
  EngineUtilities::TSharedPointer<T>
    getComponent() {
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");

    for (const auto& component : components) { // Usa const auto& para eficiencia
      // Usa dynamic_pointer_cast del TSharedPointer para realizar el cast seguro
      EngineUtilities::TSharedPointer<T> specificComponent = component.dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EngineUtilities::TSharedPointer<T>(); // Retorna un puntero compartido nulo si no se encuentra
  }

  /**
   * @brief Inicia la entidad y sus componentes.
   * @param deltaTime El tiempo transcurrido desde el último frame.
   */
  virtual void start(float deltaTime) {}

  /**
   * @brief Actualiza la lógica de la entidad y sus componentes.
   * @param deltaTime El tiempo transcurrido desde el último frame.
   */
  virtual void update(float deltaTime) {}

  /**
   * @brief Renderiza la entidad y sus componentes.
   * @param window El puntero compartido a la ventana de renderizado.
   */
  virtual void render(const EngineUtilities::TSharedPointer<class Window>& window) {}


  /**
   * @brief Devuelve el nombre de la entidad.
   * @return El nombre de la entidad.
   */
  const std::string& getName() const { return m_name; }

  /**
   * @brief Establece el nombre de la entidad.
   * @param name El nuevo nombre de la entidad.
   */
  void setName(const std::string& name) { m_name = name; }


protected:
  std::string m_name;
  // Aquí es donde Entity almacena todos sus componentes
  std::vector<EngineUtilities::TSharedPointer<Component>> components;
};