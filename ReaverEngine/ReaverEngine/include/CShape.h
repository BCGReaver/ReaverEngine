/**
 * @file CShape.h
 * @brief Definición del componente CShape para manejar figuras geométricas en SFML.
 *
 * @details
 * Este componente:
 * - Permite crear figuras de distintos tipos (círculo, rectángulo, triángulo, polígono).
 * - Soporta configuración de posición, color, rotación, escala y textura.
 * - Se integra con el sistema de componentes del motor.
 *
 * Estilo aplicado:
 * - Nombres de métodos en lowerCamelCase.
 * - Variables miembro con prefijo `m_`.
 * - Documentación Doxygen en español con descripciones claras y directas.
 */

#pragma once
#include "Prerequisites.h"
#include "ECS\Component.h"
#include "ECS\Texture.h"

class Window;

/**
 * @class CShape
 * @brief Componente para renderizar y manipular figuras 2D usando SFML.
 *
 * @details
 * CShape hereda de `Component` y encapsula un `sf::Shape` para permitir:
 * - Creación de formas geométricas básicas.
 * - Modificación en tiempo de ejecución de sus propiedades visuales.
 * - Renderizado en una ventana administrada por `Window`.
 *
 * @note El componente debe inicializarse con `createShape()` antes de intentar modificarlo o dibujarlo.
 */
class CShape : public Component {
public:
  /**
   * @brief Constructor por defecto.
   */
  CShape() = default;

  /**
   * @brief Constructor que inicializa el componente como tipo SHAPE.
   * @param shapeType Tipo de figura a crear (inicialmente no se crea la forma hasta llamar a `createShape`).
   */
  CShape(ShapeType shapeType)
    : m_shapePtr(nullptr),
    m_shapeType(ShapeType::EMPTY),
    Component(ComponentType::SHAPE) {
  }

  /**
   * @brief Destructor por defecto.
   */
  virtual ~CShape() = default;

  /**
   * @brief Crea una figura según el tipo especificado.
   * @param shapeType Tipo de figura (`CIRCLE`, `RECTANGLE`, `TRIANGLE`, `POLYGON`).
   *
   * @details
   * La figura se inicializa con color blanco y tamaño predeterminado.
   * Internamente, se guarda en `m_shapePtr` como `sf::Shape`.
   */
  void createShape(ShapeType shapeType);

  /**
   * @brief Método llamado al iniciar el componente.
   */
  void start() override;

  /**
   * @brief Actualiza la lógica del componente cada frame.
   * @param deltaTime Tiempo transcurrido desde el último frame.
   */
  void update(float deltaTime) override;

  /**
   * @brief Renderiza la figura en la ventana.
   * @param window Puntero compartido a la ventana donde se dibujará.
   */
  void render(const EngineUtilities::TSharedPointer<Window>& window) override;

  /**
   * @brief Libera recursos asociados a la figura.
   */
  void destroy() override;

  /**
   * @brief Establece la posición de la figura usando coordenadas X e Y.
   */
  void setPosition(float x, float y);

  /**
   * @brief Establece la posición de la figura usando un vector 2D.
   */
  void setPosition(const sf::Vector2f& position);

  /**
   * @brief Cambia el color de relleno de la figura.
   */
  void setFillColor(const sf::Color& color);

  /**
   * @brief Rota la figura en grados.
   */
  void setRotation(float angle);

  /**
   * @brief Cambia la escala de la figura.
   */
  void setScale(const sf::Vector2f& scl);

  /**
   * @brief Asigna una textura a la figura.
   * @param texture Textura que se aplicará a la figura.
   */
  void setTexture(const EngineUtilities::TSharedPointer<Texture>& texture);

private:
  EngineUtilities::TSharedPointer<sf::Shape> m_shapePtr; ///< Puntero a la figura SFML.
  ShapeType m_shapeType; ///< Tipo actual de figura.
  sf::VertexArray* m_line; ///< Opcional, para representar líneas o formas personalizadas.
};
