#include "BaseApp.h"
#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
BaseApp::~BaseApp() {}

//------------------------------------------------------------------------------
// Run loop
//------------------------------------------------------------------------------
int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp", "run", "Initializes result on a false statemente, check method validations");
  }
  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents(m_engineGUI);
    update();
    render();
  }
  destroy();
  return 0;
}

//------------------------------------------------------------------------------
// Init
//------------------------------------------------------------------------------
bool BaseApp::init() {
  ResourceManager& resourceMan = ResourceManager::getInstance();

  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Reaver Engine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
    return false;
  }

  // ImGui
  m_engineGUI.init(m_windowPtr);

  // ---------------- Waypoints (cargar ANTES que NPCs) -------------------------
  if (!loadWaypointsCSV("assets/tracks/oval.waypoints.csv")) {
    initWaypoints_DefaultTrack();
  }

  // ---------------- Actor "pista" (actor original) ----------------------------
  m_ACirlce = EngineUtilities::MakeShared<Actor>("Track");
  if (!m_ACirlce) {
    ERROR("BaseApp", "init", "Failed to create Actor, check memory allocation");
    return false;
  }
  m_ACirlce->getComponent<CShape>()->createShape(RECTANGLE);
  m_ACirlce->getComponent<CShape>()->setFillColor(sf::Color::White);

  // Ajusta a tu textura real (posición/escala)
  m_ACirlce->getComponent<Transform>()->setPosition(sf::Vector2f(21.f, 19.f));
  m_ACirlce->getComponent<Transform>()->setScale(sf::Vector2f(12.f, 18.f));

  // Carga textura de pista igual que siempre
  if (!resourceMan.loadTexture("Sprites/Track", "png")) {
    ERROR("BaseApp", "texture", "No pude cargar: Sprites/Track.png (revisa Working Directory)");
  }
  m_ACirlce->setTexture(resourceMan.getTexture("Sprites/Track"));
  m_actors.push_back(m_ACirlce);

  // ---------------- Marcadores de waypoints (encima de la pista) --------------
  buildWaypointMarkers();

  // ---------------- NPCs que siguen el path (TRIPLICAR pista) -----------------
  // Ahora los NPCs se crean EXACTAMENTE igual que la pista (RECTANGLE + textura)
  struct NpcCfg {
    const char* name;          // nombres exactos: "MP", "BP", "TP"
    const char* texKey;        // Sprites/<name>
    sf::Vector2f scale;        // puedes ajustar si quedan muy chicos/grandes
    sf::Vector2f startOffset;  // para no encimarlos en el primer waypoint
  };

  std::vector<NpcCfg> cfgs;
  cfgs.push_back({ "MP", "Sprites/MP", sf::Vector2f(1.5f, 1.5f), sf::Vector2f(0.f,   0.f) });
  cfgs.push_back({ "BP", "Sprites/BP", sf::Vector2f(1.5f, 1.5f), sf::Vector2f(-18.f, -18.f) });
  cfgs.push_back({ "TP", "Sprites/TP", sf::Vector2f(1.5f, 1.5f), sf::Vector2f(-18.f,  18.f) });

  for (const auto& cfg : cfgs) {
    // Se mantienen como A_Racer para que sigan waypoints,
    // pero su setup visual es EL MISMO que el actor pista:
    auto npc = EngineUtilities::MakeShared<A_Racer>(cfg.name);
    if (npc.isNull()) continue;

    // --- MISMO pipeline que la pista ---
    npc->getComponent<CShape>()->createShape(RECTANGLE);
    npc->getComponent<CShape>()->setFillColor(sf::Color::White);

    // posición inicial: primer waypoint, y pequeña separación
    npc->setPath(m_waypoints);
    if (auto xf = npc->getComponent<Transform>()) {
      sf::Vector2f p = xf->getPosition();      // setPath ya puso path.front()
      xf->setPosition(p + cfg.startOffset);
      xf->setScale(cfg.scale);                 // escala estilo sprite rectangular
    }

    // velocidad (ajústalas a gusto)
    npc->setMaxSpeed(240.f);

    // textura EXACTAMENTE como la pista (mismo ResourceManager y método)
    if (!resourceMan.loadTexture(cfg.texKey, "png")) {
      ERROR("BaseApp", "texture", std::string("No pude cargar: ") + cfg.texKey + ".png");
    }
    else {
      npc->setTexture(resourceMan.getTexture(cfg.texKey));
    }

    // Guardamos en ambas listas (para HUD/orden y para jerarquía/inspector)
    m_racers.push_back(npc);
    m_actors.push_back(npc);
  }

  return true;
}

//------------------------------------------------------------------------------
// Update
//------------------------------------------------------------------------------
void BaseApp::update() {
  if (!m_windowPtr.isNull()) m_windowPtr->update();

  m_engineGUI.update(m_windowPtr, m_windowPtr->deltaTime);
  m_engineGUI.outliner(m_actors);
  m_engineGUI.inspector(m_actors);
  // ImGui::ShowDemoWindow();

  const float dt = m_windowPtr->deltaTime.asSeconds();
  for (auto& actor : m_actors) {
    if (!actor.isNull()) actor->update(dt);
  }
}

//------------------------------------------------------------------------------
// Render  (orden: pista -> waypoints -> NPCs encima)
//------------------------------------------------------------------------------
void BaseApp::render() {
  if (!m_windowPtr) return;
  m_windowPtr->clear();

  // 1) Pista
  if (!m_ACirlce.isNull()) {
    if (auto shape = m_ACirlce->getComponent<CShape>()) shape->render(m_windowPtr);
    else m_ACirlce->render(m_windowPtr);
  }

  // 2) Waypoints
  for (auto& marker : m_waypointMarkers) {
    if (marker.isNull()) continue;
    if (auto shape = marker->getComponent<CShape>()) shape->render(m_windowPtr);
    else marker->render(m_windowPtr);
  }

  // 3) NPCs (ahora son “copias” del pipeline de la pista)
  for (auto& npc : m_racers) {
    if (npc.isNull()) continue;
    if (auto shape = npc->getComponent<CShape>()) shape->render(m_windowPtr);
    else npc->render(m_windowPtr);
  }

  m_windowPtr->render();
  m_engineGUI.render(m_windowPtr);
  m_windowPtr->display();
}

//------------------------------------------------------------------------------
// Destroy
//------------------------------------------------------------------------------
void BaseApp::destroy() {
  m_engineGUI.destroy();
}

// ==============================================================================
// =====================  WAYPOINTS: CSV / DEFAULT / MARKERS  ===================
// ==============================================================================

bool BaseApp::loadWaypointsCSV(const std::string& path) {
  std::ifstream in(path);
  if (!in.is_open()) {
    MESSAGE("BaseApp", "Waypoints", "CSV not found: " + path);
    return false;
  }

  std::vector<sf::Vector2f> parsed;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream ss(line);
    std::string sx, sy;
    if (std::getline(ss, sx, ',') && std::getline(ss, sy, ',')) {
      try {
        float x = std::stof(sx);
        float y = std::stof(sy);
        parsed.push_back(sf::Vector2f(x, y));
      }
      catch (...) {
        MESSAGE("BaseApp", "Waypoints", "Invalid line in CSV: " + line);
      }
    }
  }

  if (parsed.empty()) {
    MESSAGE("BaseApp", "Waypoints", "CSV loaded but no valid rows");
    return false;
  }

  m_waypoints = parsed;
  m_currentWaypointIndex = 0;
  MESSAGE("BaseApp", "Waypoints", "Loaded " + std::to_string(m_waypoints.size()) + " points from CSV");
  return true;
}

void BaseApp::initWaypoints_DefaultTrack() {
  m_waypoints.clear();
  m_waypoints.push_back(sf::Vector2f(32.f, 483.f));
  m_waypoints.push_back(sf::Vector2f(30.f, 214.f));
  m_waypoints.push_back(sf::Vector2f(35.f, 110.f));
  m_waypoints.push_back(sf::Vector2f(72.f, 66.f));
  m_waypoints.push_back(sf::Vector2f(113.f, 37.f));
  m_waypoints.push_back(sf::Vector2f(195.f, 26.f));
  m_waypoints.push_back(sf::Vector2f(310.f, 38.f));
  m_waypoints.push_back(sf::Vector2f(354.f, 118.f));
  m_waypoints.push_back(sf::Vector2f(388.f, 212.f));
  m_waypoints.push_back(sf::Vector2f(424.f, 268.f));
  m_waypoints.push_back(sf::Vector2f(465.f, 261.f));
  m_waypoints.push_back(sf::Vector2f(523.f, 261.f));
  m_waypoints.push_back(sf::Vector2f(586.f, 253.f));
  m_waypoints.push_back(sf::Vector2f(622.f, 165.f));
  m_waypoints.push_back(sf::Vector2f(639.f, 92.f));
  m_waypoints.push_back(sf::Vector2f(665.f, 55.f));
  m_waypoints.push_back(sf::Vector2f(723.f, 32.f));
  m_waypoints.push_back(sf::Vector2f(864.f, 121.f));
  m_waypoints.push_back(sf::Vector2f(894.f, 183.f));
  m_waypoints.push_back(sf::Vector2f(908.f, 243.f));
  m_waypoints.push_back(sf::Vector2f(913.f, 295.f));
  m_waypoints.push_back(sf::Vector2f(894.f, 363.f));
  m_waypoints.push_back(sf::Vector2f(858.f, 405.f));
  m_waypoints.push_back(sf::Vector2f(792.f, 434.f));
  m_waypoints.push_back(sf::Vector2f(609.f, 478.f));
  m_waypoints.push_back(sf::Vector2f(412.f, 524.f));
  m_waypoints.push_back(sf::Vector2f(366.f, 551.f));
  m_waypoints.push_back(sf::Vector2f(367.f, 589.f));
  m_waypoints.push_back(sf::Vector2f(432.f, 643.f));
  m_waypoints.push_back(sf::Vector2f(530.f, 641.f));
  m_waypoints.push_back(sf::Vector2f(678.f, 633.f));
  m_waypoints.push_back(sf::Vector2f(745.f, 633.f));
  m_waypoints.push_back(sf::Vector2f(882.f, 648.f));
  m_waypoints.push_back(sf::Vector2f(899.f, 675.f));
  m_waypoints.push_back(sf::Vector2f(919.f, 717.f));
  m_waypoints.push_back(sf::Vector2f(912.f, 780.f));
  m_waypoints.push_back(sf::Vector2f(888.f, 858.f));
  m_waypoints.push_back(sf::Vector2f(802.f, 886.f));
  m_waypoints.push_back(sf::Vector2f(695.f, 897.f));
  m_waypoints.push_back(sf::Vector2f(649.f, 802.f));
  m_waypoints.push_back(sf::Vector2f(548.f, 793.f));
  m_waypoints.push_back(sf::Vector2f(468.f, 801.f));
  m_waypoints.push_back(sf::Vector2f(189.f, 931.f));
  m_waypoints.push_back(sf::Vector2f(35.f, 839.f));
  m_waypoints.push_back(sf::Vector2f(64.f, 558.f));
  m_currentWaypointIndex = 0;
}

void BaseApp::buildWaypointMarkers() {
  m_waypointMarkers.clear();
  if (!m_showWaypointMarkers) return;

  int idx = 0;
  for (const auto& p : m_waypoints) {
    auto marker = EngineUtilities::MakeShared<Actor>(std::string("WP_") + std::to_string(idx++));
    if (marker.isNull()) continue;

    marker->getComponent<CShape>()->createShape(CIRCLE);
    marker->getComponent<CShape>()->setFillColor(sf::Color(0, 255, 255, 200));
    marker->getComponent<Transform>()->setPosition(p);
    marker->getComponent<Transform>()->setScale(sf::Vector2f(0.4f, 0.4f));

    if (idx == 1) marker->getComponent<CShape>()->setFillColor(sf::Color(0, 255, 0, 220));
    else if (idx == (int)m_waypoints.size()) marker->getComponent<CShape>()->setFillColor(sf::Color(255, 0, 0, 220));

    m_waypointMarkers.push_back(marker);
    m_actors.push_back(marker);
  }
}