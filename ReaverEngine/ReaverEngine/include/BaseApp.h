#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "CShape.h"
#include "ECS/Actor.h"
#include "EngineGUI.h"
#include "A_Racer.h"

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class BaseApp {
public:
  BaseApp() = default;
  ~BaseApp();

  int  run();
  bool init();
  void update();
  void render();
  void destroy();

private:
  // --- Ventana / GUI / Actores
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors;   // jerarquía
  EngineUtilities::TSharedPointer<Window> m_windowPtr;
  EngineUtilities::TSharedPointer<Actor>  m_ACirlce;              // pista
  EngineGUI m_engineGUI;

  // --- Waypoints
  std::vector<sf::Vector2f> m_waypoints;
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_waypointMarkers;
  size_t m_currentWaypointIndex = 0;
  bool   m_showWaypointMarkers = true;

  // Editor de waypoints (HUD)
  int  m_wpEditIndex = 0;
  bool m_wpEditorOpen = true;

  // --- NPCs
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;

  // --- Parrilla de salida (posiciones para Reset/Countdown)
  std::vector<sf::Vector2f> m_gridPositions;

  // --- Estado de carrera + cronómetro
  enum class RaceState { Ready, Countdown, Running, Finished };
  RaceState  m_state = RaceState::Ready;
  float      m_countdown = 3.0f;
  float      m_countLeft = 0.0f;
  sf::Clock  m_clock;
  float      m_raceTime = 0.0f;
  int        m_totalLaps = 3;

  // --- Helpers
  bool loadWaypointsCSV(const std::string& path);
  bool saveWaypointsCSV(const std::string& path);
  void initWaypoints_DefaultTrack();
  void buildWaypointMarkers();
  void refreshMarkersFromWaypoints();   // mueve marcadores existentes
  void rebuildRacersPath();             // aplica m_waypoints a todos los NPCs
  bool loadTextureOrLog(const std::string& keyNoExt);

  // HUD / lógica carrera
  void drawHUD();
  void drawWaypointEditor();            // panel de edición
  void resetRace(bool hardResetSprites);
  void startCountdown();
  void updateRaceTimers(float dt);
  void applyRaceConfigToRacers();
  void computeAndShowRanking();
};
