#pragma once
#include "Component.h"
#include <SFML/System/Vector2.hpp>

class
  Transform : public Component {
public:
  
  Transform();

  ~Transform() override = default;

  void
    start() override;

  void
    update(float deltaTime) override;

  void
    render(const EngineUtilities::TSharedPointer<Window>& window) override;

  void
    destroy() override;

  // Getters y Setters para Position
  const sf::Vector2f&
    getPosition() const;

  void
    setPosition(const sf::Vector2f& newPosition);

  void
    setPosition(float x, float y);

  // Getters y Setters para Rotation
  const sf::Vector2f&
    getRotation() const;

  void
    setRotation(const sf::Vector2f& newRotation);

  void
    setRotation(float x, float y);

  // Getters y Setters para Scale
  const sf::Vector2f&
    getScale() const;

  void
    setScale(const sf::Vector2f& newScale);

  void
    setScale(float x, float y);

private:
  sf::Vector2f m_position; 
  sf::Vector2f m_rotation; 
  sf::Vector2f m_scale;    
};
