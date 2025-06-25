#pragma once
#include "../Prerequisites.h"
#include "Entity.h"
#include "Cshape.h"

class 
Actor : Entity {
public:
	Actor() = default;

	Actor(const std::string & actorName);

	virtual
	~Actor() = default;

	void
	start() override;

	void
	update(float deltaTime) override;

	void
	render(const EngineUtilities::TSharedPointer<Window>& window) override;

	void
	destroy() override;

	/**
   * @brief Obtiene un componente específico del actor.
   * @tparam T Tipo del componente que se va a obtener.
   * @return Puntero compartido al componente, o nullptr si no se encuentra.
   */
  template <typename T>
  EngineUtilities::TSharedPointer<T> 
  getComponent();

private:
	std::string m_name = "Actor";
};

/**
 * El propósito de esta función es buscar y devolver un componente específico de un actor,
 * utilizando el tipo de componente especificado como argumento de la plantilla.
 * Si el componente no se encuentra, la función devuelve nullptr.
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
