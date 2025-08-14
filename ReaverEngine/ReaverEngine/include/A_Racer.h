#pragma once

#include "ECS/Actor.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>

/**
 * @brief Racer (NPC/Jugador). Steering + vueltas (sin FloatRect).
 */
class A_Racer : public Actor {
public:
  enum class Steering : int { PathFollow = 0, Seek = 1, Arrive = 2 };

  explicit A_Racer(const std::string& name, int playerId = 0);

  void start() override {}
  void update(float deltaTime) override;

  // Path / vueltas
  void setPath(const std::vector<sf::Vector2f>& pathPoints);
  void reset();

  void setTotalLaps(int laps) { m_totalLaps = laps; }
  int  getCurrentLap() const { return m_currentLap; }
  int  getTotalLaps()  const { return m_totalLaps; }
  bool isFinished()    const { return m_currentLap >= m_totalLaps; }

  // Steering
  void  setMaxSpeed(float s) { m_maxSpeed = s; }
  float getMaxSpeed() const { return m_maxSpeed; }

  void setBehavior(Steering s) { m_behavior = s; }
  Steering getBehavior() const { return m_behavior; }

  // Para ranking/hud
  float getProgress() const;

private:
  // Behaviors
  void doPathFollowing(float deltaTime);
  void doSeek(const sf::Vector2f& target, float dt);
  void doArrive(const sf::Vector2f& target, float dt);

  std::vector<sf::Vector2f> path;
  int   currentWaypointIndex = 0;

  // Conteo de vueltas sin Rect: detecta "wrap" de índice
  int   m_lastIndex = 0;

  // Parámetros steering
  Steering m_behavior = Steering::PathFollow;
  float lookaheadDistance = 140.f;
  float arriveRadius = 26.f;   // para PathFollow
  float m_maxSpeed = 240.f;

  // Arrive específico
  float m_arriveSlowRadius = 120.f;
  float m_arriveStopRadius = 20.f;

  // Vueltas
  int  m_currentLap = 0;
  int  m_totalLaps = 3;

  // Target temporal (Seek/Arrive): siguiente punto
  sf::Vector2f m_debugTarget = sf::Vector2f(0.f, 0.f);
};
