/**
 * @file BaseApp.h
 * @brief Clase que orquesta todo el flow del motor: init → update → render → destroy.
 *
 * @details
 * Este header define la clase **BaseApp**, que básicamente es “la jefa” del bucle principal.
 * Aquí se inicializa la ventana, la GUI, la pista, se cargan los waypoints y se crean los NPCs.
 * Además, maneja el estado de la carrera (Ready → Countdown → Running → Finished) y dibuja el HUD.
 *
 * ## Estilo de documentación (versión compa)
 * - **Variables**: usamos **camelCase** (primera palabra en minúscula, las siguientes con MayúsculaInicial).
 *   - Ej: `contarPalabras`, `aumentarNivelDificultad`.
 * - **Variables globales**: si algún día metes una global, que empiece con **g_** y en minúscula (y bien justificadas, pls).
 *   - Ej: `int g_time = 0;`
 * - **Variables locales**: descriptivas, sin prefijos raros.
 *   - Ej:
 *     @code
 *     void foo() {
 *       float time = 0.0f;
 *     }
 *     @endcode
 * - **Métodos y funciones**: igual que las variables, empiezan en minúscula y si hay más palabras, pegas con Mayúscula.
 *   - Ej: `sumaDeOperadores()`, `init()`, `update()`, `render()`, `destroy()`.
 * - **Clases/Estructuras**: con su tipo y salto de línea como en C++ clásico. Miembros con `m_` al inicio.
 * - **Tabulaciones**: 2 espacios para que se vea limpio (chécalo en las settings del IDE).
 */

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
  * @brief Clase principal que corre el juego/app y controla todo el ciclo de vida.
  *
  * @details
  * Lo que hace esta clase:
  * - Crea la **ventana** y arranca la **GUI** (ImGui).
  * - Carga **waypoints** desde CSV (o usa unos por defecto si no hay archivo).
  * - Arma la **pista** (un `Actor` con `CShape` y textura).
  * - Crea los **NPCs** (racers) y los pone en parrilla usando el primer tramo del path.
  * - Lleva el **estado de la carrera** y los **timers** (countdown, race time).
  * - Dibuja un **HUD** con controles y un mini **editor de waypoints** in-app.
  */
class BaseApp {
public:
  /**
   * @brief Constructor por defecto.
   */
  BaseApp() = default;

  /**
   * @brief Destructor (libera/termina subsistemas necesarios).
   */
  ~BaseApp();

  /**
   * @brief Ejecuta el loop principal (ideal llamarlo desde `main()`).
   * @return 0 si todo salió fine.
   *
   * @details
   * Flujo: `init()` → (loop: `update()` y `render()`) → `destroy()`.
   */
  int run();

  /**
   * @brief Inicializa ventana, GUI, pista, waypoints, marcadores y NPCs.
   * @return true si todo ok, false si algo crítico falló (ej. crear ventana).
   */
  bool init();

  /**
   * @brief Lógica por frame: timers, actores y GUI/HUD.
   */
  void update();

  /**
   * @brief Dibuja escena completa (pista, marcadores, racers) + GUI.
   */
  void render();

  /**
   * @brief Apaga subsistemas y limpia lo necesario.
   */
  void destroy();

private:
  // ==========================
  // Ventana / GUI / Actores
  // ==========================

  /**
   * @brief Lista de actores en escena (jerarquía simple para el outliner).
   */
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors;

  /**
   * @brief Ventana principal (SFML) envuelta en puntero compartido del motor.
   */
  EngineUtilities::TSharedPointer<Window> m_windowPtr;

  /**
   * @brief Actor que representa la pista (un rectángulo con textura).
   */
  EngineUtilities::TSharedPointer<Actor> m_ACirlce;

  /**
   * @brief Control de la interfaz (ImGui).
   */
  EngineGUI m_engineGUI;

  // ==========================
  // Waypoints
  // ==========================

  /**
   * @brief Lista ordenada de puntos del circuito (en pixeles/coords de mundo).
   */
  std::vector<sf::Vector2f> m_waypoints;

  /**
   * @brief Marcadores visuales (actores con círculos) para debug/edición.
   */
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_waypointMarkers;

  /**
   * @brief Índice del waypoint “actual” (para lógicas que lo necesiten).
   */
  size_t m_currentWaypointIndex = 0;

  /**
   * @brief Flag para mostrar/ocultar los marcadores de los waypoints.
   */
  bool m_showWaypointMarkers = true;

  // ==========================
  // Editor de waypoints (HUD)
  // ==========================

  /**
   * @brief Índice del waypoint seleccionado en el editor (ImGui).
   */
  int m_wpEditIndex = 0;

  /**
   * @brief Flag para abrir/cerrar el panel de edición de WPs.
   */
  bool m_wpEditorOpen = true;

  // ==========================
  // NPCs (Racers)
  // ==========================

  /**
   * @brief Lista de racers (NPCs) que siguen el path y compiten.
   */
  std::vector<EngineUtilities::TSharedPointer<A_Racer>> m_racers;

  // ==========================
  // Parrilla de salida
  // ==========================

  /**
   * @brief Posiciones de parrilla para hacer reset/colocar al inicio.
   *
   * @details
   * Se calculan en el tramo AB (primer segmento de `m_waypoints`) con un gap fijo.
   */
  std::vector<sf::Vector2f> m_gridPositions;

  // ==========================
  // Estado de carrera + cronómetro
  // ==========================

  /**
   * @brief Estados posibles de la carrera.
   */
  enum class RaceState {
    Ready,     ///< Antes de iniciar (esperando).
    Countdown, ///< Contando hacia atrás.
    Running,   ///< Carrera en curso.
    Finished   ///< Todos terminaron o se dio por finalizada.
  };

  /**
   * @brief Estado actual de la carrera.
   */
  RaceState m_state = RaceState::Ready;

  /**
   * @brief Tiempo del countdown (en segundos).
   */
  float m_countdown = 3.0f;

  /**
   * @brief Tiempo restante del countdown (va decreciendo).
   */
  float m_countLeft = 0.0f;

  /**
   * @brief Reloj SFML para medir tiempos (countdown/race).
   */
  sf::Clock m_clock;

  /**
   * @brief Tiempo de carrera total (se acumula mientras `Running`).
   */
  float m_raceTime = 0.0f;

  /**
   * @brief Vueltas totales de la carrera.
   */
  int m_totalLaps = 3;

  // ==========================
  // Helpers (utilidades privadas)
  // ==========================

  /**
   * @brief Carga waypoints desde CSV (formato simple: `x,y` por línea, admite `#`).
   * @param path Ruta del archivo CSV.
   * @return true si se cargó al menos un punto válido.
   */
  bool loadWaypointsCSV(const std::string& path);

  /**
   * @brief Guarda los waypoints actuales a un CSV (`x,y` por línea).
   * @param path Ruta destino.
   * @return true si pudo escribir el archivo.
   */
  bool saveWaypointsCSV(const std::string& path);

  /**
   * @brief Inicializa un circuito por defecto (fallback cuando no hay CSV).
   */
  void initWaypoints_DefaultTrack();

  /**
   * @brief Crea los actores de marcadores (círculos) para cada waypoint.
   *
   * @note El primero va en verde, el último en rojo, y los intermedios en cian.
   */
  void buildWaypointMarkers();

  /**
   * @brief Actualiza la posición de los marcadores según `m_waypoints`.
   *
   * @details
   * Si cambia el tamaño de la lista, reconstruye los marcadores desde cero.
   */
  void refreshMarkersFromWaypoints();

  /**
   * @brief Reaplica `m_waypoints` a todos los racers y opcionalmente los devuelve a parrilla.
   */
  void rebuildRacersPath();

  /**
   * @brief Intenta cargar una textura por clave (sin extensión) y loguea si falla.
   * @param keyNoExt Clave/ruta sin extensión (asume `"png"`).
   * @return true si cargó bien, false si no.
   */
  bool loadTextureOrLog(const std::string& keyNoExt);

  // ==========================
  // HUD / lógica de carrera
  // ==========================

  /**
   * @brief Dibuja el HUD de la carrera (estado, laps, modos y ranking).
   */
  void drawHUD();

  /**
   * @brief Dibuja el panel del editor de waypoints (ImGui).
   */
  void drawWaypointEditor();

  /**
   * @brief Resetea la carrera (estado, tiempos) y devuelve racers a parrilla.
   * @param hardResetSprites Si es true, fuerza `update(0)` para alinear visuales.
   */
  void resetRace(bool hardResetSprites);

  /**
   * @brief Arranca el conteo regresivo para pasar a `Running`.
   */
  void startCountdown();

  /**
   * @brief Actualiza timers de carrera (countdown y race time).
   * @param dt Delta time en segundos.
   */
  void updateRaceTimers(float dt);

  /**
   * @brief Aplica configuración global de carrera (vueltas) a todos los racers.
   */
  void applyRaceConfigToRacers();

  /**
   * @brief Calcula y pinta el ranking actual (en base a lap + progress).
   */
  void computeAndShowRanking();
};

