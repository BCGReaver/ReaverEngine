#include "A_Racer.h"

#if __has_include("ECS/Transform.h")
#include "ECS/Transform.h"
#elif __has_include("Transform.h")
#include "Transform.h"
#else
#error "No se encontró Transform.h (ajusta la ruta a 'ECS/Transform.h' o 'Transform.h')"
#endif

#include <cmath>
#include <algorithm>
#include <functional> // std::hash

static inline float vlen(const sf::Vector2f& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}
static inline sf::Vector2f norm(const sf::Vector2f& v) {
  float l = vlen(v); if (l <= 1e-6f) return sf::Vector2f(0.f, 0.f);
  return sf::Vector2f(v.x / l, v.y / l);
}
static inline float clamp01(float x) { return std::max(0.f, std::min(1.f, x)); }

A_Racer::A_Racer(const std::string& name, int /*playerId*/)
  : Actor(name) {
  // Fases únicas por nombre para que no “oscilen igual”
  std::size_t h = std::hash<std::string>{}(name);
  m_phase1 = float((h & 0xFFFF)) / 65535.f;  // 0..1
  m_phase2 = float((h & 0xFFFF0000) >> 16) / 65535.f;
}

void A_Racer::setPath(const std::vector<sf::Vector2f>& pathPoints) {
  path = pathPoints;
  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
    m_debugTarget = path.front();
  }
  currentWaypointIndex = (path.size() > 1 ? 1 : 0);
  m_lastIndex = currentWaypointIndex;

  // Sincroniza drawable con transform
  Actor::update(0.f);
}

void A_Racer::reset() {
  m_currentLap = 0;

  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
    m_debugTarget = path.front();
  }
  currentWaypointIndex = (path.size() > 1 ? 1 : 0);
  m_lastIndex = currentWaypointIndex;
  m_noiseT = 0.f;

  Actor::update(0.f);
}

float A_Racer::getProgress() const {
  const int N = (int)path.size();
  if (N < 2) return 0.f;

  auto xf = const_cast<A_Racer*>(this)->getComponent<Transform>();
  sf::Vector2f pos = xf ? xf->getPosition() : path.front();

  const int cur = (currentWaypointIndex % N);
  const int prev = (cur + N - 1) % N;

  const sf::Vector2f A = path[prev];
  const sf::Vector2f B = path[cur];

  const float ABx = B.x - A.x, ABy = B.y - A.y;
  const float segLen = std::max(1e-4f, std::sqrt(ABx * ABx + ABy * ABy));
  const float dx = B.x - pos.x, dy = B.y - pos.y;
  const float t = 1.f - clamp01(std::sqrt(dx * dx + dy * dy) / segLen);

  return clamp01((prev + t) / float(N));
}

void A_Racer::update(float dt) {
  if (isFinished()) { Actor::update(dt); return; }

  const int prevIndex = currentWaypointIndex;

  // Avanza tiempo de ruido
  if (m_wanderFreq > 0.f) m_noiseT += dt * m_wanderFreq;

  // Comportamiento actual
  if (path.size() >= 2) {
    switch (m_behavior) {
    case Steering::PathFollow: doPathFollowing(dt); break;
    case Steering::Seek:       doSeek(m_debugTarget, dt); break;
    case Steering::Arrive:     doArrive(m_debugTarget, dt); break;
    }
  }

  // Vuelta cuando el índice hace wrap (e.g., pasa de 41 -> 0)
  if (currentWaypointIndex < prevIndex) ++m_currentLap;
  m_lastIndex = currentWaypointIndex;

  Actor::update(dt);
}

void A_Racer::doPathFollowing(float dt) {
  auto xf = getComponent<Transform>();
  if (!xf || path.size() < 2) return;

  sf::Vector2f pos = xf->getPosition();

  const int N = (int)path.size();
  int i = currentWaypointIndex;
  sf::Vector2f A = path[i];
  sf::Vector2f B = path[(i + 1) % N];
  sf::Vector2f AB = B - A;
  float abLen2 = AB.x * AB.x + AB.y * AB.y;
  if (abLen2 < 1e-6f) { currentWaypointIndex = (i + 1) % N; return; }
  float abLen = std::sqrt(abLen2);

  // Tangente y normal (izquierda)
  sf::Vector2f T = sf::Vector2f(AB.x / abLen, AB.y / abLen);
  sf::Vector2f Nrm(-T.y, T.x);

  sf::Vector2f AP = pos - A;
  float t = (AP.x * AB.x + AP.y * AB.y) / abLen2;

  float distToB = std::hypot(pos.x - B.x, pos.y - B.y);
  if (t > 1.f || distToB < arriveRadius) {
    currentWaypointIndex = (i + 1) % N;
    i = currentWaypointIndex;
    A = path[i];
    B = path[(i + 1) % N];
    AB = B - A;
    abLen2 = AB.x * AB.x + AB.y * AB.y;
    abLen = std::sqrt(abLen2);
    if (abLen2 < 1e-6f) return;
    T = sf::Vector2f(AB.x / abLen, AB.y / abLen);
    Nrm = sf::Vector2f(-T.y, T.x);
    AP = pos - A;
    t = (AP.x * AB.x + AP.y * AB.y) / abLen2;
  }

  // Punto de persecución
  float s = std::clamp(t + (lookaheadDistance / std::max(abLen, 1e-3f)), 0.f, 1.f);
  sf::Vector2f pursue = A + AB * s;

  // Variaciones: offset lateral fijo + wander senoidal + pequeño jitter de velocidad
  pursue += Nrm * m_lateralOffset;
  if (m_wanderStrength > 0.f) {
    const float twoPi = 6.2831853f;
    float wiggle = std::sin((m_noiseT + m_phase1) * twoPi) * m_wanderStrength;
    pursue += Nrm * wiggle;
  }

  // Dirigirse hacia pursue
  sf::Vector2f to = pursue - pos;
  float d = std::hypot(to.x, to.y);
  if (d > 1e-4f) {
    sf::Vector2f dir = sf::Vector2f(to.x / d, to.y / d);

    float brakeRadius = lookaheadDistance * 1.2f;
    float baseSpeed = (d < brakeRadius) ? (m_maxSpeed * (d / brakeRadius)) : m_maxSpeed;

    // Jitter de velocidad (±)
    if (m_speedJitter > 0.f) {
      const float twoPi = 6.2831853f;
      float jitterMul = 1.f + m_speedJitter * std::sin((m_noiseT + m_phase2) * twoPi * 0.7f);
      baseSpeed *= std::max(0.6f, jitterMul); // evita caer demasiado
    }

    pos += dir * baseSpeed * dt;

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    xf->setRotation(angleDeg);
    xf->setPosition(pos);
  }

  // Para demo de Seek/Arrive: target = siguiente punto
  m_debugTarget = B;
}

void A_Racer::doSeek(const sf::Vector2f& target, float dt) {
  auto xf = getComponent<Transform>();
  if (!xf) return;
  sf::Vector2f pos = xf->getPosition();
  sf::Vector2f desired = target - pos;
  float d = vlen(desired);
  if (d < 1e-4f) return;

  sf::Vector2f vel = norm(desired) * m_maxSpeed;
  pos += vel * dt;

  float angleDeg = std::atan2(vel.y, vel.x) * 180.f / 3.14159265f;
  xf->setRotation(angleDeg);
  xf->setPosition(pos);
}

void A_Racer::doArrive(const sf::Vector2f& target, float dt) {
  auto xf = getComponent<Transform>();
  if (!xf) return;

  sf::Vector2f pos = xf->getPosition();
  sf::Vector2f to = target - pos;
  float d = vlen(to);
  if (d < m_arriveStopRadius) return; // ya llegó

  float speed = m_maxSpeed;
  if (d < m_arriveSlowRadius) {
    speed = m_maxSpeed * (d / m_arriveSlowRadius); // desacelera
  }

  sf::Vector2f vel = norm(to) * speed;
  pos += vel * dt;

  float angleDeg = std::atan2(vel.y, vel.x) * 180.f / 3.14159265f;
  xf->setRotation(angleDeg);
  xf->setPosition(pos);
}
