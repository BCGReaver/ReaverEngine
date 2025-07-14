#pragma once

#include <string>
#include <vector>

#include "ECS/Entity.h" // Actor hereda de Entity
#include "ECS/Transform.h" // Asegúrate de que esta ruta sea correcta
#include "CShape.h"    // Asegúrate de que esta ruta sea correcta
#include <Memory/TSharedPointer.h> // Para TSharedPointer<Window> en render

class Actor : public Entity { // Actor hereda de Entity
public:
  Actor(const std::string& actorName);

  // Override de los métodos virtuales de Entity
  void update(float deltaTime) override;
  void render(const EngineUtilities::TSharedPointer<class Window>& window) override;

};
