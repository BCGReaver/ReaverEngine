/**
 * @file window.cpp
 * @brief Implementación de la clase Window que envuelve una sf::RenderWindow (SFML 3) con utilidades de la engine.
 *
 * @details
 * - Esta clase centraliza la gestión de la ventana: creación, loop de eventos, limpiar/dibujar/mostrar y ciclo básico.
 * - Usa un smart pointer tipo `EngineUtilities::MakeUnique` con helpers como `isNull()` y `release()`.
 * - Todos los métodos verifican que la ventana exista antes de operar, y reportan con `MESSAGE`/`ERROR`.
 *
 * @note
 * Mantén esta clase enfocada a I/O de ventana. La GUI (EngineGUI) se procesa en `handleEvents()` para no mezclar capas.
 *
 * @see window.h, EngineGUI
 */

#include "window.h"
#include "EngineGUI.h"

 /**
  * @brief Constructor: crea la ventana con tamaño y título solicitados.
  *
  * @param width  Ancho de la ventana en pixeles.
  * @param height Alto de la ventana en pixeles.
  * @param title  Título de la ventana.
  *
  * @details
  * - Crea `sf::RenderWindow` (SFML 3) con estilo por defecto.
  * - Limita el framerate a 60 FPS para que no se dispare la GPU (y el ventilador 😅).
  * - Reporta por log si la creación fue exitosa o no.
  */
Window::Window(int width, int height, const std::string& title) {
  // Crear ventana con SFML 3
  m_windowPtr = EngineUtilities::MakeUnique<sf::RenderWindow>(
    sf::VideoMode({ static_cast<unsigned int>(width),
                    static_cast<unsigned int>(height) }),
    title,
    sf::Style::Default
  );

  if (!m_windowPtr.isNull()) {
    m_windowPtr->setFramerateLimit(60);
    MESSAGE("Window", "Window", "Window created successfully");
  }
  else {
    ERROR("Window", "Window", "Failed to create window");
  }
}

/**
 * @brief Destructor: libera la ventana si existe.
 *
 * @details
 * Usamos `release()` del smart pointer de la engine. Aquí la idea es soltar el recurso
 * para que no quede ninguna referencia colgada.
 */
Window::~Window() {
  m_windowPtr.release();
}

/**
 * @brief Atiende eventos de la ventana y los pasa a la capa de GUI.
 *
 * @param engineGUI Referencia a la GUI para que procese el evento (teclado, mouse, etc.).
 *
 * @details
 * - Itera sobre los eventos pendientes con `pollEvent()` (versión de SFML 3 que regresa std::optional).
 * - Pasa cada evento a `engineGUI.processEvent(...)`.
 * - Si llega un `sf::Event::Closed`, cerramos la ventana de forma segura.
 *
 * @warning
 * Este método asume que `m_windowPtr` no es nulo. Si lo fuera, no habrá eventos que procesar.
 */
void
Window::handleEvents(EngineGUI& engineGUI) const {

  //while (m_windowPtr->isOpen())
  //{
  //}
  // Process events
  while (const std::optional event = m_windowPtr->pollEvent())
  {
    engineGUI.processEvent(*m_windowPtr, *event);
    // Close window: exit
    if (event->is<sf::Event::Closed>())
      m_windowPtr->close();
  }
}

/**
 * @brief Indica si la ventana sigue abierta.
 * @return true si la ventana existe y está abierta; false en otro caso.
 *
 * @details
 * Hacemos check de nulidad antes de preguntar `isOpen()` para evitar crasheos.
 * Si es nulo, logueamos error y devolvemos false.
 */
bool
Window::isOpen() const {
  // Check that window is not null
  if (!m_windowPtr.isNull()) {
    return m_windowPtr->isOpen();
  }
  else {
    ERROR("Window", "isOpen", "Window is null");
    return false;
  }
}

/**
 * @brief Limpia el backbuffer con el color indicado.
 * @param color Color de limpieza.
 *
 * @details
 * Este es el primer paso típico del frame (clear → draw → display).
 */
void
Window::clear(const sf::Color& color) const {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->clear(color);
  }
  else {
    ERROR("Window", "clear", "Window is null");
  }
}

/**
 * @brief Dibuja un `sf::Drawable` con estados opcionales.
 * @param drawable Cualquier cosa dibujable de SFML (sprite, shape, text, etc.).
 * @param states   Estados de render (transform, blend, shader...), por defecto los que pase el caller.
 */
void
Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states) const {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->draw(drawable, states);
  }
  else {
    ERROR("Window", "draw", "Window is null");
  }
}

/**
 * @brief Presenta en pantalla lo que se dibujó en el frame.
 *
 * @details
 * Último paso del frame: hace el swap de buffers y muestra el resultado.
 */
void
Window::display() const {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->display();
  }
  else {
    ERROR("Window", "display", "Window is null");
  }
}

/**
 * @brief Actualiza el temporizador del frame y guarda el deltaTime.
 *
 * @details
 * Reinicia el reloj interno y almacena el delta para que el resto del sistema
 * pueda moverse a tiempo real (movimiento, animaciones, etc.).
 *
 * @note
 * Se asume que `deltaTime` y `clock` son miembros de la clase (ver `window.h`).
 */
void
Window::update() {
  // Almacena el deltaTime una sola vez
  deltaTime = clock.restart();

}

/**
 * @brief Hook de render de alto nivel (si se necesita).
 *
 * @details
 * Aquí podrías orquestar llamadas a `draw()` de sistemas/escenas. Ahorita está vacío
 * porque la clase solo administra la ventana; el dibujo lo hacemos desde fuera.
 */
void
Window::render() {
}

/**
 * @brief Destruye la ventana soltando el recurso del smart pointer.
 *
 * @details
 * Úsalo cuando quieras apagar la ventana de forma explícita fuera del destructor.
 */
void
Window::destroy() {
  m_windowPtr.release();
}
