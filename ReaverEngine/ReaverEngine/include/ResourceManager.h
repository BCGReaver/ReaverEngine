/**
 * @file ResourceManager.h
 * @brief Definición de la clase ResourceManager para gestión centralizada de texturas.
 *
 * @details
 * Este manager:
 * - Se encarga de cargar texturas desde disco.
 * - Evita cargas duplicadas usando un cache interno (`m_textures`).
 * - Devuelve texturas ya cargadas o una textura por defecto si no existe la solicitada.
 *
 * Patrón aplicado:
 * - Singleton: solo hay una instancia accesible por `getInstance()`.
 * - Constructor y destructor privados para impedir creación directa.
 *
 * Estilo aplicado:
 * - Métodos en lowerCamelCase.
 * - Variables miembro privadas con prefijo `m_`.
 */

#pragma once
#include "Prerequisites.h"
#include "ECS\Texture.h"

 /**
  * @class ResourceManager
  * @brief Clase Singleton que administra la carga y almacenamiento de texturas.
  *
  * @details
  * - El acceso a la instancia es global pero controlado (solo mediante `getInstance()`).
  * - Las texturas se guardan en un `unordered_map` con clave `std::string` (nombre del recurso).
  * - El uso de smart pointers (`TSharedPointer<Texture>`) evita fugas de memoria.
  *
  * @note
  * Este manager solo gestiona `Texture`, pero podría ampliarse a otros recursos (sonidos, fuentes, etc.).
  */
class ResourceManager {
private:
  /**
   * @brief Constructor privado para el patrón Singleton.
   */
  ResourceManager() = default;

  /**
   * @brief Destructor privado para evitar destrucción externa.
   */
  ~ResourceManager() = default;

public:
  /**
   * @brief Evita copia del Singleton.
   */
  ResourceManager(const ResourceManager&) = delete;

  /**
   * @brief Evita asignación del Singleton.
   */
  ResourceManager& operator=(const ResourceManager&) = delete;

  /**
   * @brief Obtiene la instancia única del ResourceManager.
   * @return Referencia a la instancia estática.
   */
  static ResourceManager& getInstance() {
    static ResourceManager instance;
    return instance;
  }

  /**
   * @brief Carga una textura en memoria si no existe.
   * @param fileName  Nombre base del archivo (sin extensión).
   * @param extension Extensión del archivo (png, jpg, etc.).
   * @return true si la textura se carga o ya estaba cargada.
   *
   * @note Las texturas se indexan por su `fileName`.
   */
  bool loadTexture(const std::string& fileName, const std::string& extension);

  /**
   * @brief Obtiene una textura cargada o la textura por defecto.
   * @param fileName Nombre de la textura a buscar.
   * @return `TSharedPointer<Texture>` con la textura encontrada o la por defecto.
   *
   * @warning Si no existe la textura por defecto en los recursos, se intentará cargar en el momento.
   */
  EngineUtilities::TSharedPointer<Texture> getTexture(const std::string& fileName);

private:
  std::unordered_map<std::string, EngineUtilities::TSharedPointer<Texture>> m_textures; ///< Cache de texturas.
};
