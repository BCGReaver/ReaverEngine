/**
 * @file EngineGUI.h
 * @brief Definición de la clase EngineGUI para manejar la interfaz gráfica del editor/juego con ImGui + SFML.
 *
 * @details
 * EngineGUI se encarga de:
 * - Inicializar y destruir la capa de interfaz ImGui.
 * - Renderizar menús, jerarquías, consola e inspector de objetos.
 * - Aplicar estilos personalizados (oscuro y gris estilo Unreal).
 * - Manejar interacciones como selección de actores y edición de transformaciones.
 *
 * Estilo aplicado:
 * - Nombres de métodos en lowerCamelCase.
 * - Variables miembro privadas con prefijo `m_` (en este caso `selectedActorIndex` no lleva m_ pero está en private).
 * - Código identado a 2 espacios.
 */

#pragma once
#include "Prerequisites.h"

class Window;
class Actor;

/**
 * @class EngineGUI
 * @brief Clase para administrar y renderizar la interfaz gráfica del motor usando ImGui.
 *
 * @details
 * Esta clase no dibuja nada del juego en sí; solo dibuja las herramientas visuales para el desarrollador:
 * - Barra de menú principal (File, Edit, View, etc.).
 * - Jerarquía de actores ("Outliner").
 * - Consola de mensajes y logs.
 * - Inspector de propiedades de actores.
 * - Controles visuales custom (como `vec2Control`).
 *
 * @note Requiere que SFML esté inicializado y que haya una ventana activa antes de llamar a `init()`.
 */
class EngineGUI {
public:
  /**
   * @brief Constructor por defecto.
   */
  EngineGUI() = default;

  /**
   * @brief Destructor por defecto.
   */
  ~EngineGUI() = default;

  /**
   * @brief Inicializa ImGui con la ventana SFML y aplica estilo por defecto.
   * @param window Puntero compartido a la ventana principal.
   */
  void init(const EngineUtilities::TSharedPointer<Window>& window);

  /**
   * @brief Actualiza el estado de ImGui y dibuja la barra de menú.
   * @param window    Puntero compartido a la ventana principal.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   */
  void update(const EngineUtilities::TSharedPointer<Window>& window,
    sf::Time deltaTime);

  /**
   * @brief Renderiza toda la interfaz acumulada en el frame actual.
   * @param window Puntero compartido a la ventana principal.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window);

  /**
   * @brief Libera todos los recursos y apaga ImGui.
   */
  void destroy();

  /**
   * @brief Procesa eventos de SFML para ImGui (mouse, teclado, etc.).
   * @param window Ventana SFML que recibe el evento.
   * @param event  Evento de SFML a procesar.
   */
  void processEvent(const sf::Window& window, const sf::Event& event);

  /**
   * @brief Aplica un estilo gris inspirado en Unreal Engine 5.
   */
  void setupGreyGUIStyle();

  /**
   * @brief Aplica un estilo oscuro personalizado.
   */
  void setupDarkGUIStyle();

  /**
   * @brief Dibuja la barra de menú principal con opciones básicas.
   */
  void barMenu();

  /**
   * @brief Muestra la jerarquía de actores y permite seleccionarlos.
   * @param actors Lista de actores que se mostrarán.
   */
  void outliner(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

  /**
   * @brief Muestra la consola de mensajes clasificados por tipo.
   * @param programMessages Mapa con tipo de mensaje y lista de textos.
   */
  void console(const std::map<ConsolErrorType, std::vector<std::string>>& programMessages);

  /**
   * @brief Muestra el inspector de propiedades del actor seleccionado.
   * @param actors Lista de actores disponibles.
   */
  void inspector(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors);

  /**
   * @brief Control ImGui para editar un vector 2D con botones de reseteo por eje.
   * @param label        Etiqueta del control.
   * @param values       Puntero a los valores X,Y.
   * @param resetValues  Valor al que se resetea cada eje.
   * @param columnWidth  Ancho de la columna para la etiqueta.
   */
  void vec2Control(const std::string& label,
    float* values,
    float resetValues = 0.0f,
    float columnWidth = 100.0f);

private:
  int selectedActorIndex = -1; ///< Índice del actor actualmente seleccionado en la jerarquía.
};
