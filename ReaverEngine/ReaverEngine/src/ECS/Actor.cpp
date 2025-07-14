#include "ECS/Actor.h" 

Actor::Actor(const std::string& actorName) : Entity() { // Llama al constructor base de Entity si lo necesitas
	m_name = actorName;

	EngineUtilities::TSharedPointer<CShape> shape = EngineUtilities::MakeShared<CShape>(); // Asumo constructor por defecto
	addComponent(shape);

	// Setup Transform
	EngineUtilities::TSharedPointer<Transform> transform = EngineUtilities::MakeShared<Transform>();
	addComponent(transform);
}

void
Actor::update(float deltaTime) {
	// Corrección para la advertencia 'auto' y uso de const auto& para eficiencia y corrección
	const auto& transform = getComponent<Transform>(); // Llama al getComponent de Entity
	const auto& shape = getComponent<CShape>(); // Llama al getComponent de Entity

	// Ahora, 'if (transform && shape)' funcionará gracias al operator bool() en TSharedPointer
	if (transform && shape) { //
		shape->setPosition(transform->getPosition());
		// Asumo que getRotation().x es el ángulo que necesitas para setRotation.
		// Si getRotation() devuelve un Vector2f con X e Y para algo más, ajusta según tu lógica.
		shape->setRotation(transform->getRotation().x);
		shape->setScale(transform->getScale());
	}
}

void
Actor::render(const EngineUtilities::TSharedPointer<Window>& window) {
	for (unsigned int i = 0; i < components.size(); i++) {
		// Ahora usamos dynamic_pointer_cast del TSharedPointer para obtener el tipo correcto
		EngineUtilities::TSharedPointer<CShape> shape = components[i].dynamic_pointer_cast<CShape>();
		if (shape) { // Esto funcionará con el operator bool()
			// CShape::render espera Window&, por lo que desreferenciamos window.get()
			shape->render(*window.get());
		}
	}
}
