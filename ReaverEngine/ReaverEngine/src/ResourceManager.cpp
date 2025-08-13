/**
 * @file ResourceManager.cpp
 * @brief Implementación del ResourceManager para manejo de texturas.
 *
 * @details
 * Esta clase se encarga de:
 * - Cargar texturas desde disco.
 * - Evitar cargas repetidas (usa cache interna con `m_textures`).
 * - Devolver texturas cargadas o una textura por defecto si no existe la solicitada.
 *
 * Estilo aplicado:
 * - Nombres en lowerCamelCase para funciones y variables locales.
 * - Variables miembro privadas con `m_`.
 * - Globales (si existieran) usarían prefijo `g_`.
 */

#include "ResourceManager.h"

 /**
  * @brief Carga una textura si no está ya en memoria.
  *
  * @param fileName  Nombre base del archivo (sin extensión).
  * @param extension Extensión del archivo (png, jpg, etc.).
  * @return true si la textura se carga o ya estaba cargada.
  *
  * @details
  * - Primero revisa si `fileName` ya existe en `m_textures`. Si sí, no la vuelve a cargar.
  * - Si no existe, crea un `TSharedPointer<Texture>` con `EngineUtilities::MakeShared`.
  * - Guarda la textura en el mapa para futuras consultas.
  *
  * @note
  * Esto ayuda a evitar cargas repetidas y ahorrar memoria.
  */
bool
ResourceManager::loadTexture(const std::string& fileName,
  const std::string& extension) {
  // Verificar si la textura está cargada
  if (m_textures.find(fileName) != m_textures.end()) {
    return true;
  }

  // Crear y cargar la textura
  auto texture = EngineUtilities::MakeShared<Texture>(fileName, extension);
  m_textures[fileName] = texture;
  return true;
}

/**
 * @brief Devuelve una textura previamente cargada o una por defecto.
 *
 * @param fileName Nombre de la textura buscada.
 * @return Puntero compartido (`TSharedPointer<Texture>`) a la textura.
 *
 * @details
 * - Si la textura existe en `m_textures`, la devuelve directamente.
 * - Si no existe, muestra advertencia por consola y busca/carga la textura "Default".
 * - Si la textura por defecto tampoco está cargada, la crea con formato "png".
 *
 * @warning
 * Asegúrate de que la textura por defecto exista en los recursos del proyecto para evitar problemas.
 */
EngineUtilities::TSharedPointer<Texture>
ResourceManager::getTexture(const std::string& fileName) {
  // Buscar la textura
  auto it = m_textures.find(fileName);
  if (it != m_textures.end()) {
    return it->second;
  }

  // Mensaje de advertencia
  std::cerr << "[ResourceManager] Texture not found: " << fileName << ". Using default texture.\n";

  const std::string defaultKey = "Default";

  // Verificar si la textura por defecto ya está cargada
  auto defaultIt = m_textures.find(defaultKey);
  if (defaultIt != m_textures.end()) {
    return defaultIt->second;
  }

  // Cargar textura por defecto
  auto defaultTexture = EngineUtilities::MakeShared<Texture>(defaultKey, "png");
  m_textures[defaultKey] = defaultTexture;
  return defaultTexture;
}
