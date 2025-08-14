#pragma once

#include "ECS/Actor.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <string>

/**
 * @brief Racer (NPC/Jugador). Path following + vueltas.
 */
class A_Racer : public Actor {
public:
  explicit A_Racer(const std::string& name, int playerId = 0);

  void start() override {}
  void update(float deltaTime) override;

  void setPath(const std::vector<sf::Vector2f>& pathPoints);
  void reset();

  void setTotalLaps(int laps) { m_totalLaps = laps; }
  int  getCurrentLap() const { return m_currentLap; }
  int  getTotalLaps()  const { return m_totalLaps; }
  bool isFinished()    const { return m_currentLap >= m_totalLaps; }

  void  setMaxSpeed(float s) { m_maxSpeed = s; }
  float getMaxSpeed() const { return m_maxSpeed; }

  // (Opcional) Progreso 0..1 dentro de la vuelta
  float getProgress() const;

private:
  void doPathFollowing(float deltaTime);

  std::vector<sf::Vector2f> path;
  int   currentWaypointIndex = 0;

  float lookaheadDistance = 140.f;
  float arriveRadius = 26.f;
  float m_maxSpeed = 240.f;

  // Vueltas: si luego quieres línea de meta, reponla aquí
  int  m_currentLap = 0;
  int  m_totalLaps = 3;
  bool m_crossedLastFrame = false;
};
