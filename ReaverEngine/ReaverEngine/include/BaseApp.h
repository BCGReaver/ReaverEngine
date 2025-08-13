/**
 * @file BaseApp.h
 * @brief Definición de la clase BaseApp, encargada del ciclo principal de la aplicación.
 *
 * @details
 * BaseApp es la clase base que controla:
 * - La inicialización de la ventana, GUI y actores.
 * - El ciclo principal (update → render).
 * - La destrucción de recursos al final.
 *
 * Estilo aplicado:
 * - Métodos en lowerCamelCase.
 * - Variables miembro privadas con prefijo `m_`.
 * - Comentarios Doxygen en español con un enfoque claro y sencillo.
 */

#pragma once
#include "Prerequisites.h"
#include "Window.h"
#include "CShape.h"
#include "ECS\Actor.h"
#include "EngineGUI.h"

 /**
  * @class BaseApp
  * @brief Clase base que implementa el flujo principal de la aplicación.
  *
  * @details
  * La estructura de BaseApp está pensada para:
  * - Iniciar los recursos necesarios (`init`).
  * - Ejecutar un bucle principal (`run`) que actualiza y renderiza.
  * - Gestionar actores y su interacción con la GUI.
  * - Apagar y limpiar recursos (`destroy`) al final.
  *
  * @note Está diseñada para ser instanciada en `main()` y controlarlo todo desde ahí.
  */
class BaseApp {
public:
  /**
   * @brief Constructor por defecto.
   */
  BaseApp() = default;

  /**
   * @brief Destructor.
   * @note Se encarga de liberar recursos en `destroy()` si es necesario.
   */
  ~BaseApp();

  /**
   * @brief Ejecuta la aplicación desde `main`.
   * @return 0 si se ejecuta y cierra correctamente.
   *
   * @details
   * - Llama a `init()` para configurar todo.
   * - Mantiene un bucle que llama a `update()` y `render()`.
   * - Al terminar, llama a `destroy()`.
   */
  int run();

  /**
   * @brief Inicializa los recursos necesarios para la app.
   * @return true si la inicialización fue exitosa, false si falló.
   */
  bool init();

  /**
   * @brief Lógica de actualización que se ejecuta cada frame.
   *
   * @details
   * Actualiza:
   * - Ventana y deltaTime.
   * - GUI (menú, jerarquía, inspector).
   * - Actores y lógica de juego.
   */
  void update();

  /**
   * @brief Encargada de dibujar en pantalla cada frame.
   *
   * @details
   * - Limpia la ventana.
   * - Renderiza actores.
   * - Renderiza la GUI.
   * - Muestra el frame en pantalla.
   */
  void render();

  /**
   * @brief Apaga y libera recursos antes de salir.
   */
  void destroy();

private:
  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors; ///< Lista de actores activos.
  EngineUtilities::TSharedPointer<Window> m_windowPtr; ///< Ventana principal de la aplicación.
  EngineUtilities::TSharedPointer<Actor> m_ACirlce; ///< Actor de prueba (círculo) creado en init().
  EngineGUI m_engineGUI; ///< Gestor de la interfaz gráfica con ImGui.
};
