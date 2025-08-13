/**
 * @file BaseApp.cpp
 * @brief Implementación de la clase BaseApp, que controla el ciclo principal de la aplicación.
 *
 * @details
 * BaseApp se encarga de:
 * - Inicializar los recursos principales (ventana, GUI, actores).
 * - Ejecutar el bucle principal (update → render).
 * - Manejar la destrucción de recursos al cerrar.
 *
 * Estilo aplicado:
 * - Nombres de métodos en lowerCamelCase.
 * - Variables miembro privadas con prefijo `m_`.
 */

#include "BaseApp.h"
#include "ResourceManager.h"

 /**
  * @brief Destructor de BaseApp.
  * @note Actualmente vacío, pero puede servir para liberar recursos adicionales.
  */
BaseApp::~BaseApp() {
}

/**
 * @brief Ejecuta el ciclo principal de la aplicación.
 * @return 0 si termina correctamente.
 *
 * @details
 * - Llama a `init()` para configurar todo.
 * - Mientras la ventana esté abierta:
 *    - Procesa eventos de ventana.
 *    - Llama a `update()` y `render()`.
 * - Al salir del bucle, llama a `destroy()`.
 */
int BaseApp::run() {
  if (!init()) {
    ERROR("BaseApp",
      "run",
      "Initializes result on a false statemente, check method validations");
  }

  while (m_windowPtr->isOpen()) {
    m_windowPtr->handleEvents(m_engineGUI);
    update();
    render();
  }

  destroy();
  return 0;
}

/**
 * @brief Inicializa los recursos principales.
 * @return true si la inicialización fue exitosa, false si falló algo crítico.
 *
 * @details
 * - Crea la ventana principal.
 * - Inicializa ImGui con `EngineGUI`.
 * - Crea un actor de prueba con un `CShape` circular.
 * - Carga y asigna una textura a dicho actor.
 * - Agrega el actor a la lista `m_actors`.
 */
bool BaseApp::init() {
  ResourceManager& resourceMan = ResourceManager::getInstance();

  m_windowPtr = EngineUtilities::MakeShared<Window>(1920, 1080, "Labrid Engine");
  if (!m_windowPtr) {
    ERROR("BaseApp", "init", "Failed to create window pointer, check memory allocation");
    return false;
  }

  // Inicializar ImGui
  m_engineGUI.init(m_windowPtr);

  // Crear Actor de tipo Círculo
  m_ACirlce = EngineUtilities::MakeShared<Actor>("Circle Actor");
  if (m_ACirlce) {
    m_ACirlce->getComponent<CShape>()->createShape(CIRCLE);
    m_ACirlce->getComponent<CShape>()->setFillColor(sf::Color::White);
    m_ACirlce->getComponent<Transform>()->setPosition(sf::Vector2f(200.f, 150.f));
    m_ACirlce->getComponent<Transform>()->setScale(sf::Vector2f(2.f, 2.f));

    // Cargar y asignar textura
    if (!resourceMan.loadTexture("Sprites/Mushroom", "png")) {
      MESSAGE("BaseApp", "Init", "Can't load the texture")
    }
    m_ACirlce->setTexture(resourceMan.getTexture("Sprites/Mushroom"));

    // Registrar actor
    m_actors.push_back(m_ACirlce);
  }
  else {
    ERROR("BaseApp", "init", "Failed to create Circle Actor, check memory allocation");
    return false;
  }
  return true;
}

/**
 * @brief Lógica que se ejecuta cada frame.
 *
 * @details
 * - Actualiza la ventana y el deltaTime.
 * - Actualiza la GUI (barra de menú, jerarquía, inspector).
 * - Actualiza actores y lógica de juego.
 */
void BaseApp::update() {
  if (!m_windowPtr.isNull()) {
    m_windowPtr->update();
  }

  // Actualizar GUI
  m_engineGUI.update(m_windowPtr, m_windowPtr->deltaTime);
  m_engineGUI.outliner(m_actors);
  m_engineGUI.inspector(m_actors);
  ImGui::ShowDemoWindow();

  // Actualizar actores
  if (!m_ACirlce.isNull()) {
    m_ACirlce->update(m_windowPtr->deltaTime.asSeconds());

    // Ejemplo: posición objetivo (comentado)
    // sf::Vector2f targetPos(1200.f, 150.f);
    // m_ACirlce->getComponent<Transform>()->seek(targetPos, 200.0f, m_windowPtr->deltaTime.asSeconds(), 10.0f);
  }
}

/**
 * @brief Renderiza todos los elementos visibles.
 *
 * @details
 * - Limpia la ventana.
 * - Renderiza los actores.
 * - Llama al render de la GUI.
 * - Presenta en pantalla.
 */
void BaseApp::render() {
  if (!m_windowPtr) {
    return;
  }

  m_windowPtr->clear();

  if (!m_ACirlce.isNull()) {
    m_ACirlce->getComponent<CShape>()->render(m_windowPtr);
  }
  m_windowPtr->render();

  // Renderizar ImGui
  m_engineGUI.render(m_windowPtr);

  m_windowPtr->display();
}

/**
 * @brief Libera recursos y apaga subsistemas.
 *
 * @details
 * - Cierra la GUI de ImGui.
 * - Aquí se podrían liberar actores, texturas, etc.
 */
void BaseApp::destroy() {
  // Destruir GUI
  m_engineGUI.destroy();

  // Limpieza futura:
  // m_shapePtr.reset();
  // m_window->destroy();
}
