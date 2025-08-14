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

/**
 * @class BaseApp
 * @brief Clase base que implementa el flujo principal de la aplicación.
 */
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
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors;   // todos los actores
  EngineUtilities::TSharedPointer<Window> m_windowPtr;
  EngineUtilities::TSharedPointer<Actor>  m_ACirlce;              // pista (texturizada)
  EngineGUI m_engineGUI;

  // --- Waypoints
  std::vector<sf::Vector2f> m_waypoints;
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_waypointMarkers;
  size_t m_currentWaypointIndex = 0;
  bool   m_showWaypointMarkers = true;

  // --- NPCs (A_Racer) para gestionar y dibujar arriba de todo
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;

  // --- Helpers
  bool loadWaypointsCSV(const std::string& path);
  void initWaypoints_DefaultTrack();
  void buildWaypointMarkers();

  // util para cargar textura con log de error
  bool loadTextureOrLog(const std::string& keyNoExt);
};
