/**
 * @file main.cpp
 * @brief Punto de entrada de la app. Aquí arrancamos la BaseApp y dejamos que haga su magia.
 *
 * @details
 * - Estilo: 2 espacios de indentación, nombres en lowerCamelCase para funciones/variables locales.
 * - Nada de prefijos raros para locales; los globales (si existieran) usarían el prefijo g_.
 * - La clase BaseApp es la que se encarga del ciclo principal (game loop / app loop).
 * - Este archivo mantiene la responsabilidad súper simple: crear la app y correrla.
 *
 * @see BaseApp
 */

#include "BaseApp.h"

 /**
  * @brief Función principal del programa.
  * @return Código de salida que entrega BaseApp::run().
  *
  * @details
  * Creamos una instancia local de `BaseApp` (sin prefijo porque NO es global) y llamamos a `run()`.
  * La idea es que `run()` maneje todo el ciclo de vida de la aplicación (init → update → render → destroy).
  * Si algún día necesitas pasar configuración, argumentos, o inicializar servicios, este es el punto ideal.
  *
  * @note Mantén esta función ligera; el chiste es delegar todo a la clase de la app.
  */
int main() {
  BaseApp app;          ///< Objeto principal de la aplicación; se destruye automáticamente al salir de main().
  return app.run();     ///< Delegamos el control del programa al loop de BaseApp.
}

