#include "A_Racer.h"

// Resuelve la ruta de Transform automáticamente
#if __has_include("ECS/Transform.h")
#include "ECS/Transform.h"
#elif __has_include("Transform.h")
#include "Transform.h"
#else
#error "No se encontró Transform.h (ajusta la ruta a 'ECS/Transform.h' o 'Transform.h')"
#endif

#include <cmath>
#include <algorithm>

static inline float vlen(const sf::Vector2f& v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}
static inline float clamp01(float x) { return std::max(0.f, std::min(1.f, x)); }

A_Racer::A_Racer(const std::string& name, int /*playerId*/)
  : Actor(name) {
}

void A_Racer::setPath(const std::vector<sf::Vector2f>& pathPoints) {
  path = pathPoints;
  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
  }
  currentWaypointIndex = (path.size() > 1 ? 1 : 0);

  // **Clave**: sincroniza el drawable con el Transform inicial
  Actor::update(0.f);
}

void A_Racer::reset() {
  m_currentLap = 0;
  m_crossedLastFrame = false;

  if (!path.empty()) {
    if (auto xf = getComponent<Transform>()) {
      xf->setPosition(path.front());
      xf->setRotation(0.f);
    }
  }
  currentWaypointIndex = (path.size() > 1 ? 1 : 0);

  // **Clave**: sincroniza tras el reset
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

  const float segLen = std::max(1e-4f, vlen(B - A));
  const float t = 1.f - clamp01(vlen(B - pos) / segLen);

  return clamp01((prev + t) / float(N));
}

void A_Racer::update(float dt) {
  if (path.size() >= 2) {
    doPathFollowing(dt);
  }

  // **MUY IMPORTANTE**: sincroniza el drawable con el Transform cada frame
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
    AP = pos - A;
    t = (AP.x * AB.x + AP.y * AB.y) / abLen2;
  }

  float s = std::clamp(t + (lookaheadDistance / std::max(abLen, 1e-3f)), 0.f, 1.f);
  sf::Vector2f pursue = A + AB * s;

  sf::Vector2f to = pursue - pos;
  float d = std::hypot(to.x, to.y);
  if (d > 1e-4f) {
    sf::Vector2f dir = { to.x / d, to.y / d };
    float brakeRadius = lookaheadDistance * 1.2f;
    float speed = (d < brakeRadius) ? (m_maxSpeed * (d / brakeRadius)) : m_maxSpeed;

    pos += dir * speed * dt;

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    xf->setRotation(angleDeg);
    xf->setPosition(pos);
  }
}
