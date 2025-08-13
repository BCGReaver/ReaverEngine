/**
 * @file Prerequisites.h
 * @brief Archivo de inclusión global para bibliotecas estándar, dependencias externas, macros y enumeraciones base del motor.
 *
 * @details
 * Este archivo agrupa:
 * - Inclusiones de librerías estándar de C++.
 * - Dependencias externas como SFML e ImGui.
 * - Macros utilitarias para mensajes y manejo de punteros.
 * - Enumeraciones globales para tipos de figuras y errores de consola.
 */

#pragma once

 // ==========================
 // Librerías estándar
 // ==========================
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <map>
#include <fstream>
#include <unordered_map>

// ==========================
// Librerías de terceros
// ==========================
#include <SFML/Graphics.hpp>

// Módulos de manejo de memoria del motor
#include "Memory/TSharedPointer.h"
#include "Memory/TWeakPointer.h"
#include "Memory/TStaticPtr.h"
#include "Memory/TUniquePtr.h"

// ImGui
#include <../imgui-master/imgui.h>
#include <imgui-SFML.h>

// ==========================
// Macros
// ==========================

/**
 * @brief Libera un puntero de forma segura y lo asigna a nullptr.
 */
#define SAFE_PTR_RELEASE(x) if(x != nullptr) { delete x; x = nullptr; }

 /**
  * @brief Macro para mostrar un mensaje de creación de recurso en consola.
  * @param classObj Nombre de la clase.
  * @param method Nombre del método.
  * @param state Estado del recurso.
  */
#define MESSAGE(classObj, method, state)                      \
{                                                             \
    std::ostringstream os_;                                   \
    os_ << classObj << "::" << method << " : "                \
        << "[CREATION OF RESOURCE" << ": " << state << "]\n"; \
    std::cerr << os_.str();                                   \
}

  /**
   * @brief Macro para mostrar un mensaje de error en consola y finalizar el programa.
   * @param classObj Nombre de la clase.
   * @param method Nombre del método.
   * @param errorMSG Mensaje de error.
   */
#define ERROR(classObj, method, errorMSG)                         \
{                                                                 \
    std::ostringstream os_;                                       \
    os_ << "ERROR : " << classObj << "::" << method << " : "      \
        << "  Error in data from params [" << errorMSG << "]\n";  \
    std::cerr << os_.str();                                       \
    exit(1);                                                      \
}

   // ==========================
   // Enumeraciones globales
   // ==========================

   /**
    * @enum ShapeType
    * @brief Tipos de figuras soportadas por el motor.
    */
enum ShapeType {
  EMPTY = 0, ///< Sin forma.
  CIRCLE = 1, ///< Círculo.
  RECTANGLE = 2, ///< Rectángulo.
  TRIANGLE = 3, ///< Triángulo.
  POLYGON = 4, ///< Polígono personalizado.
};

/**
 * @enum ConsolErrorType
 * @brief Niveles de severidad para mensajes en consola.
 */
enum ConsolErrorType {
  INFO = 0, ///< Mensaje informativo.
  WARNING = 1, ///< Mensaje de advertencia.
  ERROR = 2  ///< Mensaje de error.
};
