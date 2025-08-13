/**
 * @file Texture.h
 * @brief Define la clase Texture para la gestión de texturas en el motor.
 *
 * @details
 * La clase Texture hereda de Component y se encarga de cargar y almacenar
 * texturas usando SFML. Admite especificar nombre de archivo y extensión,
 * y expone el objeto `sf::Texture` para su uso en renderizado.
 */

#pragma once
#include "Prerequisites.h"
#include "Component.h"

 /**
  * @class Texture
  * @brief Componente para manejar y cargar texturas en el motor.
  *
  * @details
  * Este componente:
  * - Carga texturas desde disco usando SFML.
  * - Almacena el nombre y extensión del archivo.
  * - Permite obtener una referencia al `sf::Texture` cargado.
  */
class Texture : public Component {
public:

  /**
   * @brief Constructor por defecto.
   *
   * Crea un componente de textura vacío, sin datos cargados.
   */
  Texture() = default;

  /**
   * @brief Constructor que carga una textura desde archivo.
   * @param textureName Nombre del archivo de textura sin extensión.
   * @param extension Extensión del archivo (por defecto `"png"`).
   *
   * @note Si la carga falla, se imprime un mensaje de error en consola.
   */
  Texture(const std::string& textureName, const std::string& extension = "png") :
    m_textureName(textureName), m_extension(extension), Component(ComponentType::TEXTURE) {
    if (!m_texture.loadFromFile(m_textureName + "." + m_extension)) {
      std::cout << "Error de carga de textura: "
        << m_textureName << "." << m_extension << std::endl;
    }
  }

  /**
   * @brief Destructor por defecto.
   */
  virtual ~Texture() = default;

  /**
   * @brief Método de inicio del componente (no utilizado en esta clase).
   */
  void start() override {};

  /**
   * @brief Método de actualización por frame (no utilizado en esta clase).
   * @param deltaTime Tiempo transcurrido entre frames.
   */
  void update(float deltaTime) override {};

  /**
   * @brief Método de renderizado (no utilizado en esta clase).
   * @param window Puntero a la ventana donde renderizar.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override {};

  /**
   * @brief Método de destrucción del componente (no utilizado en esta clase).
   */
  void destroy() override {};

  /**
   * @brief Obtiene la textura SFML cargada.
   * @return Referencia a `sf::Texture`.
   */
  sf::Texture& getTexture() {
    return m_texture;
  }

private:
  sf::Texture m_texture;      ///< Objeto de textura SFML.
  std::string m_textureName;  ///< Nombre del archivo de textura (sin extensión).
  std::string m_extension;    ///< Extensión del archivo de textura.
};
