/**
 * @file BaseApp.cpp
 * @brief Loop principal de la app: init → update → render → destroy, con sistema de carrera y edición de waypoints.
 *
 * @details
 * Este módulo se encarga de:
 * - Inicializar ventana, GUI, pista, waypoints y NPCs.
 * - Cargar/guardar waypoints desde/para CSV.
 * - Controlar el estado de la carrera (Ready → Countdown → Running → Finished).
 * - Dibujar HUD (laps, ranking, controles) y un mini editor de waypoints in-app.
 *
 * Estilo:
 * - 2 espacios de indentación.
 * - Métodos/variables locales en lowerCamelCase.
 * - Miembros privados con prefijo m_.
 */

#include "BaseApp.h"
#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

 //------------------------------------------------------------------------------
 // Destructor
 //------------------------------------------------------------------------------

 /**
  * @brief Destructor por defecto.
  */
BaseApp::~BaseApp() {}

//------------------------------------------------------------------------------
// Run loop
//------------------------------------------------------------------------------

/**
 * @brief Punto de arranque del ciclo principal de la app.
 * @return 0 si todo ok.
 *
 * @details
 * - Valida `init()`.
 * - Mientras la ventana esté abierta: procesa eventos, actualiza y renderiza.
 * - Al salir: `destroy()`.
 */
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

/**
 * @brief Inicializa ventana, GUI, pista, waypoints, marcadores, NPCs y estado de carrera.
 * @return true si todo ok; false si falla algo crítico (como crear la ventana).
 */
bool BaseApp::init() {
  ResourceManager& resourceMan = ResourceManager::getInstance();

  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Reaver Engine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create Window");
    return false;
  }

  // ImGui
  m_engineGUI.init(m_windowPtr);

  // --- Waypoints (cargar ANTES que NPCs)
  if (!loadWaypointsCSV("assets/tracks/oval.waypoints.csv")) {
    initWaypoints_DefaultTrack();
  }

  // --- Actor pista (rectángulo texturizado con el track)
  m_ACirlce = EngineUtilities::MakeShared<Actor>("Track");
  if (!m_ACirlce) {
    ERROR("BaseApp", "init", "Failed to create Track actor");
    return false;
  }
  m_ACirlce->getComponent<CShape>()->createShape(RECTANGLE);
  m_ACirlce->getComponent<CShape>()->setFillColor(sf::Color::White);
  m_ACirlce->getComponent<Transform>()->setPosition(sf::Vector2f(21.f, 19.f));
  m_ACirlce->getComponent<Transform>()->setScale(sf::Vector2f(12.f, 18.f));

  resourceMan.loadTexture("Sprites/Track", "png");
  m_ACirlce->setTexture(resourceMan.getTexture("Sprites/Track"));
  m_actors.push_back(m_ACirlce);

  // --- Marcadores visibles de WPs (si están habilitados)
  buildWaypointMarkers();

  // --- NPCs (racers): configuración base + parrilla sobre el primer segmento del path
  struct NpcCfg {
    const char* name;
    const char* texKey;
    sf::Vector2f scale;
    float lateral;
    float wanderPx;
    float wanderHz;
    float speedJitter;
  };
  std::vector<NpcCfg> cfgs{
    { "MP", "Sprites/MP", {1.5f,1.5f},  0.f,  8.f, 0.8f, 0.08f },
    { "BP", "Sprites/BP", {1.5f,1.5f}, +9.f, 10.f, 1.0f, 0.10f },
    { "TP", "Sprites/TP", {1.5f,1.5f}, -9.f,  7.f, 0.7f, 0.07f }
  };

  // Vector director del 1er tramo para colocar parrilla “en fila”
  sf::Vector2f A = m_waypoints.front();
  sf::Vector2f B = (m_waypoints.size() > 1 ? m_waypoints[1] : sf::Vector2f(A.x + 1.f, A.y));
  sf::Vector2f T = B - A;
  float tLen = std::sqrt(T.x * T.x + T.y * T.y);
  if (tLen < 1e-4f) tLen = 1.f;
  T = sf::Vector2f(T.x / tLen, T.y / tLen);
  const float slotGap = 42.f;

  m_gridPositions.clear();

  for (size_t idx = 0; idx < cfgs.size(); ++idx) {
    const auto& cfg = cfgs[idx];

    auto npc = EngineUtilities::MakeShared<A_Racer>(cfg.name);
    if (npc.isNull()) continue;

    npc->getComponent<CShape>()->createShape(RECTANGLE);
    npc->getComponent<CShape>()->setFillColor(sf::Color::White);

    npc->setPath(m_waypoints);

    // Slot de parrilla hacia atrás sobre el tramo AB
    sf::Vector2f startPos = A - T * (slotGap * float(idx));
    m_gridPositions.push_back(startPos);

    if (auto xf = npc->getComponent<Transform>()) {
      xf->setPosition(startPos);
      xf->setScale(cfg.scale);
    }

    npc->setMaxSpeed(240.f);
    npc->setBehavior(A_Racer::Steering::PathFollow);
    npc->setLateralOffset(cfg.lateral);
    npc->setJitter(cfg.wanderPx, cfg.wanderHz, cfg.speedJitter);

    resourceMan.loadTexture(cfg.texKey, "png");
    npc->setTexture(resourceMan.getTexture(cfg.texKey));

    m_racers.push_back(npc);
    m_actors.push_back(npc);
  }

  applyRaceConfigToRacers();
  resetRace(true);   ///< Deja en Ready y coloca en parrilla
  return true;
}

//------------------------------------------------------------------------------
// Update
//------------------------------------------------------------------------------

/**
 * @brief Lógica por frame: tiempos, actores y GUI/HUD.
 *
 * @details
 * - Actualiza deltaTime de la ventana.
 * - Avanza timers de carrera (countdown, race time).
 * - Actualiza actores según estado (Running o pausado).
 * - Actualiza GUI (outliner/inspector) y dibuja HUD.
 */
void BaseApp::update() {
  if (!m_windowPtr.isNull()) m_windowPtr->update();
  const float dt = m_windowPtr->deltaTime.asSeconds();

  updateRaceTimers(dt);

  if (m_state == RaceState::Running) {
    for (auto& a : m_actors) if (!a.isNull()) a->update(dt);
  }
  else {
    for (auto& a : m_actors) if (!a.isNull()) a->update(0.f);
  }

  m_engineGUI.update(m_windowPtr, m_windowPtr->deltaTime);
  m_engineGUI.outliner(m_actors);
  m_engineGUI.inspector(m_actors);
  drawHUD();
}

//------------------------------------------------------------------------------
// Render
//------------------------------------------------------------------------------

/**
 * @brief Dibuja pista, marcadores, racers, GUI y presenta en pantalla.
 */
void BaseApp::render() {
  if (!m_windowPtr) return;
  m_windowPtr->clear();

  if (!m_ACirlce.isNull()) {
    if (auto s = m_ACirlce->getComponent<CShape>()) s->render(m_windowPtr);
    else m_ACirlce->render(m_windowPtr);
  }
  for (auto& m : m_waypointMarkers) {
    if (m.isNull()) continue;
    if (auto s = m->getComponent<CShape>()) s->render(m_windowPtr);
    else m->render(m_windowPtr);
  }
  for (auto& n : m_racers) {
    if (n.isNull()) continue;
    if (auto s = n->getComponent<CShape>()) s->render(m_windowPtr);
    else n->render(m_windowPtr);
  }

  m_windowPtr->render();
  m_engineGUI.render(m_windowPtr);
  m_windowPtr->display();
}

//------------------------------------------------------------------------------
// Destroy
//------------------------------------------------------------------------------

/**
 * @brief Limpieza de subsistemas (GUI, etc.).
 */
void BaseApp::destroy() {
  m_engineGUI.destroy();
}

// ============================================================================
// Waypoints load/save
// ============================================================================

/**
 * @brief Carga waypoints desde un CSV (x,y por línea, admite comentarios con '#').
 * @param path Ruta al CSV.
 * @return true si se cargó al menos un punto válido.
 *
 * @note Limpia `m_waypoints` y resetea `m_currentWaypointIndex` a 0.
 */
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
      try { parsed.emplace_back(std::stof(sx), std::stof(sy)); }
      catch (...) { MESSAGE("BaseApp", "Waypoints", "Invalid line: " + line); }
    }
  }
  if (parsed.empty()) return false;
  m_waypoints = std::move(parsed);
  m_currentWaypointIndex = 0;
  return true;
}

/**
 * @brief Guarda los waypoints actuales a CSV como "x,y" por línea.
 * @param path Ruta destino.
 * @return true si pudo abrir y escribir el archivo.
 */
bool BaseApp::saveWaypointsCSV(const std::string& path) {
  std::ofstream out(path, std::ios::trunc);
  if (!out.is_open()) return false;
  out << "# x,y\n";
  for (auto& p : m_waypoints) out << p.x << "," << p.y << "\n";
  return true;
}

/**
 * @brief Inicializa un track por defecto hardcodeado (fallback si no hay CSV).
 */
void BaseApp::initWaypoints_DefaultTrack() {
  // (mismos puntos de antes; ajusta aquí si quieres hardcodear uno nuevo)
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

/**
 * @brief Crea actores marcadores de los waypoints (círculos) y los agrega a la escena si cumplen flag.
 *
 * @note Primer WP en verde, último en rojo, resto en cian semi-transparente.
 */
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

/**
 * @brief Sincroniza la posición de los marcadores con `m_waypoints`.
 *
 * @details
 * Si el número de puntos cambió, reconstruye por completo los marcadores.
 */
void BaseApp::refreshMarkersFromWaypoints() {
  // Asume mismo tamaño. Si cambia el número, vuelve a buildWaypointMarkers().
  if (m_waypointMarkers.size() != m_waypoints.size()) {
    // elimina marcadores viejos de m_actors
    for (auto& mk : m_waypointMarkers) {
      auto it = std::find(m_actors.begin(), m_actors.end(), mk);
      if (it != m_actors.end()) m_actors.erase(it);
    }
    buildWaypointMarkers();
    return;
  }
  for (size_t i = 0; i < m_waypointMarkers.size(); ++i) {
    if (m_waypointMarkers[i].isNull()) continue;
    m_waypointMarkers[i]->getComponent<Transform>()->setPosition(m_waypoints[i]);
  }
}

/**
 * @brief Reasigna el path a todos los racers y opcionalmente los devuelve a parrilla.
 */
void BaseApp::rebuildRacersPath() {
  for (auto& r : m_racers) {
    if (r.isNull()) continue;
    r->setPath(m_waypoints); // también resetea su posición al primer WP
  }
  // Vuelve a parrilla (opcional)
  for (size_t i = 0; i < m_racers.size() && i < m_gridPositions.size(); ++i) {
    if (auto xf = m_racers[i]->getComponent<Transform>()) xf->setPosition(m_gridPositions[i]);
  }
}

// ============================================================================
// Helpers HUD / carrera
// ============================================================================

/**
 * @brief Helper para cargar una textura y loguear en caso de error.
 * @param keyNoExt Clave/ruta sin extensión.
 * @return true si cargó ok.
 */
bool BaseApp::loadTextureOrLog(const std::string& keyNoExt) {
  ResourceManager& rm = ResourceManager::getInstance();
  if (!rm.loadTexture(keyNoExt, "png")) {
    ERROR("BaseApp", "texture", std::string("No pude cargar: ") + keyNoExt + ".png");
    return false;
  }
  return true;
}

/**
 * @brief Aplica configuración de carrera (laps) a todos los racers.
 */
void BaseApp::applyRaceConfigToRacers() {
  for (auto& r : m_racers) if (!r.isNull()) r->setTotalLaps(m_totalLaps);
}

/**
 * @brief Resetea el estado de carrera y devuelve racers a parrilla.
 * @param hardResetSprites Si true, fuerza un update(0) para sincronizar visuals.
 */
void BaseApp::resetRace(bool hardResetSprites) {
  m_state = RaceState::Ready;
  m_raceTime = 0.f;
  m_countLeft = 0.f;

  for (size_t i = 0; i < m_racers.size(); ++i) {
    auto& r = m_racers[i];
    if (r.isNull()) continue;
    r->reset();
    // volver a parrilla
    if (i < m_gridPositions.size()) {
      if (auto xf = r->getComponent<Transform>()) {
        xf->setPosition(m_gridPositions[i]);
      }
    }
    if (hardResetSprites) r->update(0.f);
  }
}

/**
 * @brief Inicia el conteo regresivo para pasar a estado Running.
 */
void BaseApp::startCountdown() {
  m_state = RaceState::Countdown;
  m_countLeft = m_countdown;
  m_clock.restart();
}

/**
 * @brief Actualiza conteo regresivo, tiempo de carrera y detecta si todos terminaron.
 * @param dt Delta time en segundos.
 */
void BaseApp::updateRaceTimers(float dt) {
  if (m_state == RaceState::Countdown) {
    m_countLeft -= dt;
    if (m_countLeft <= 0.f) {
      m_state = RaceState::Running;
      m_raceTime = 0.f;
      m_clock.restart();
    }
  }
  else if (m_state == RaceState::Running) {
    m_raceTime += dt;

    bool allFinished = true;
    for (auto& r : m_racers) {
      if (r.isNull()) continue;
      if (!r->isFinished()) { allFinished = false; break; }
    }
    if (allFinished) m_state = RaceState::Finished;
  }
}

/**
 * @brief Calcula y muestra el ranking actual en la UI (ImGui).
 *
 * @details
 * - Score = lap actual + progreso dentro de la vuelta.
 * - Ordena descendentemente y lista con `BulletText`.
 */
void BaseApp::computeAndShowRanking() {
  struct Entry { EngineUtilities::TSharedPointer<A_Racer> r; float score; };
  std::vector<Entry> list;
  for (auto& r : m_racers) {
    if (r.isNull()) continue;
    float sc = r->getCurrentLap() + r->getProgress();
    list.push_back({ r, sc });
  }
  std::sort(list.begin(), list.end(),
    [](const Entry& a, const Entry& b) { return a.score > b.score; });

  int pos = 1;
  for (auto& e : list) {
    ImGui::BulletText("#%d  %s  L:%d/%d  prog:%.2f",
      pos++, e.r->getName().c_str(),
      e.r->getCurrentLap(), m_totalLaps, e.r->getProgress());
  }
}

// ---------------------------------------------------------------------------
// HUD principal + Editor de Waypoints
// ---------------------------------------------------------------------------

/**
 * @brief Dibuja el subpanel para editar waypoints en caliente.
 *
 * @details
 * - Muestra el total y un índice seleccionable.
 * - Permite arrastrar float2 y hacer “nudges” ±1/±5 por eje.
 * - Botones para guardar CSV, reconstruir marcadores y rearmar paths.
 */
void BaseApp::drawWaypointEditor() {
  if (!m_wpEditorOpen || m_waypoints.empty()) return;

  if (ImGui::CollapsingHeader("Waypoints (editor)")) {
    ImGui::Text("Total: %d", (int)m_waypoints.size());
    if (m_wpEditIndex < 0) m_wpEditIndex = 0;
    if (m_wpEditIndex >= (int)m_waypoints.size()) m_wpEditIndex = (int)m_waypoints.size() - 1;

    ImGui::SliderInt("Selected", &m_wpEditIndex, 0, (int)m_waypoints.size() - 1);
    sf::Vector2f& p = m_waypoints[(size_t)m_wpEditIndex];
    float xy[2] = { p.x, p.y };
    if (ImGui::DragFloat2("Position (x,y)", xy, 0.5f)) {
      p.x = xy[0]; p.y = xy[1];
      refreshMarkersFromWaypoints();
      rebuildRacersPath();
    }

    // Nudge rápido
    if (ImGui::Button("X -5")) { p.x -= 5; refreshMarkersFromWaypoints(); rebuildRacersPath(); }
    ImGui::SameLine();
    if (ImGui::Button("X -1")) { p.x -= 1; refreshMarkersFromWaypoints(); rebuildRacersPath(); }
    ImGui::SameLine();
    if (ImGui::Button("X +1")) { p.x += 1; refreshMarkersFromWaypoints(); rebuildRacersPath(); }
    ImGui::SameLine();
    if (ImGui::Button("X +5")) { p.x += 5; refreshMarkersFromWaypoints(); rebuildRacersPath(); }

    if (ImGui::Button("Y -5")) { p.y -= 5; refreshMarkersFromWaypoints(); rebuildRacersPath(); }
    ImGui::SameLine();
    if (ImGui::Button("Y -1")) { p.y -= 1; refreshMarkersFromWaypoints(); rebuildRacersPath(); }
    ImGui::SameLine();
    if (ImGui::Button("Y +1")) { p.y += 1; refreshMarkersFromWaypoints(); rebuildRacersPath(); }
    ImGui::SameLine();
    if (ImGui::Button("Y +5")) { p.y += 5; refreshMarkersFromWaypoints(); rebuildRacersPath(); }

    ImGui::Separator();
    if (ImGui::Button("Save CSV")) {
      if (saveWaypointsCSV("assets/tracks/oval.waypoints.csv")) {
        MESSAGE("BaseApp", "Waypoints", "Saved to assets/tracks/oval.waypoints.csv");
      }
      else {
        ERROR("BaseApp", "Waypoints", "Failed to save CSV");
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Rebuild markers")) {
      buildWaypointMarkers();
      rebuildRacersPath();
    }
    ImGui::TextDisabled("Tip: ajusta con DragFloat o 'nudge' y presiona Save.");
  }
}

/**
 * @brief Ventana HUD principal: estado de carrera, controles, laps, modos de steering y ranking.
 */
void BaseApp::drawHUD() {
  ImGui::SetNextWindowBgAlpha(0.35f);
  if (ImGui::Begin("HUD Carrera", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Estado: %s",
      (m_state == RaceState::Ready ? "Ready" :
        m_state == RaceState::Countdown ? "Countdown" :
        m_state == RaceState::Running ? "Running" : "Finished"));
    if (m_state == RaceState::Countdown) {
      ImGui::Text("Comienza en: %.1f", m_countLeft);
    }
    else if (m_state == RaceState::Running) {
      ImGui::Text("Tiempo: %.2f s", m_raceTime);
    }

    if (ImGui::Button("Start / Countdown") && m_state == RaceState::Ready) startCountdown();
    ImGui::SameLine();
    if (ImGui::Button("Reset")) { resetRace(true); }

    ImGui::Separator();
    if (ImGui::SliderInt("Laps", &m_totalLaps, 1, 10)) applyRaceConfigToRacers();

    ImGui::Separator();
    for (auto& r : m_racers) {
      if (r.isNull()) continue;
      int mode = (int)r->getBehavior();
      ImGui::Text("%s", r->getName().c_str());
      ImGui::SameLine();
      if (ImGui::RadioButton("PathFollow", mode == 0)) mode = 0;
      ImGui::SameLine();
      if (ImGui::RadioButton("Seek", mode == 1)) mode = 1;
      ImGui::SameLine();
      if (ImGui::RadioButton("Arrive", mode == 2)) mode = 2;
      r->setBehavior((A_Racer::Steering)mode);
    }

    ImGui::Separator();
    ImGui::TextUnformatted("Ranking:");
    computeAndShowRanking();

    // Editor de WPs
    ImGui::Separator();
    drawWaypointEditor();
  }
  ImGui::End();
}
