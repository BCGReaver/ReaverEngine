/**
 * @file window.h
 * @brief Definición de la clase Window: la capa que maneja la ventana principal usando SFML 3.
 *
 * @details
 * Esta clase:
 * - Crea y destruye la ventana principal.
 * - Atiende eventos y se los pasa a la GUI.
 * - Controla el ciclo básico de render: clear → draw → display.
 * - Mantiene un deltaTime para que todo corra a tiempo real.
 *
 * Estilo aplicado:
 * - Nombres en lowerCamelCase para métodos y variables locales.
 * - Variables miembro privadas con prefijo `m_` para diferenciarlas.
 * - Globales (si hubiera) usarían prefijo `g_`.
 */

#pragma once
#include "Prerequisites.h"

class EngineGUI;

/**
 * @class Window
 * @brief Clase que administra una `sf::RenderWindow` y su ciclo de vida.
 *
 * @details
 * - Encapsula creación, eventos, render y destrucción.
 * - Integra con EngineGUI para manejar entrada de usuario.
 * - Usa smart pointers personalizados de EngineUtilities para manejar memoria de la ventana.
 *
 * @note
 * Esta clase no se encarga de lógica de juego; su chamba es solo la ventana y su loop de I/O gráfico.
 */
class Window {
public:
  /**
   * @brief Constructor por defecto (no crea ventana).
   */
  Window() = default;

  /**
   * @brief Constructor: crea la ventana con el tamaño y título indicados.
   * @param width  Ancho de la ventana en pixeles.
   * @param height Alto de la ventana en pixeles.
   * @param title  Título de la ventana.
   */
  Window(int width, int height, const std::string& title);

  /**
   * @brief Destructor: libera la ventana si existe.
   */
  ~Window();

  /**
   * @brief Procesa los eventos de la ventana y se los pasa a EngineGUI.
   * @param engineGUI Referencia a la instancia de GUI que procesará los eventos.
   */
  void handleEvents(EngineGUI& engineGUI) const;

  /**
   * @brief Comprueba si la ventana está abierta.
   * @return true si la ventana existe y sigue abierta; false en otro caso.
   */
  bool isOpen() const;

  /**
   * @brief Limpia la ventana con un color de fondo.
   * @param color Color con el que se limpia (por defecto, negro opaco).
   */
  void clear(const sf::Color& color = sf::Color(0, 0, 0, 255)) const;

  /**
   * @brief Dibuja cualquier objeto `sf::Drawable` en la ventana.
   * @param drawable Objeto a dibujar.
   * @param states   Estados de render (transform, blend, shaders...). Por defecto, los de SFML.
   */
  void draw(const sf::Drawable& drawable,
    const sf::RenderStates& states = sf::RenderStates::Default) const;

  /**
   * @brief Muestra en pantalla lo dibujado en el frame.
   */
  void display() const;

  /**
   * @brief Actualiza el deltaTime y reinicia el reloj interno.
   */
  void update();

  /**
   * @brief Punto de render de alto nivel (puede coordinar draw calls).
   *
   * @note Actualmente está vacío; ideal para extenderlo.
   */
  void render();

  /**
   * @brief Libera el recurso de ventana de forma explícita.
   */
  void destroy();

private:
  sf::View m_view; ///< Vista SFML (cámara virtual).

public:
  EngineUtilities::TUniquePtr<sf::RenderWindow> m_windowPtr; ///< Smart pointer a la ventana SFML.
  sf::Time deltaTime;   ///< Tiempo transcurrido desde el último frame.
  sf::Clock clock;      ///< Reloj interno para medir deltaTime.
};
