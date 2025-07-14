#pragma once 
#include <cmath>   
#include <cstddef> 
#include <iostream> 

/**
 * @class CVector2
 * @brief Implementación de un vector 2D para operaciones geométricas y aritméticas.
 *
 * Esta clase proporciona funcionalidades básicas para el manejo de vectores
 * en un espacio bidimensional, incluyendo constructores, operadores aritméticos,
 * comparaciones, acceso por índice y funciones geométricas.
 */
class
  CVector2 {
public:
  // Miembros públicos del vector
  float x; ///< Componente X del vector.
  float y; ///< Componente Y del vector.

  // Constructores 
  /**
   * @brief Constructor por defecto. Inicializa el vector a (0,0).
   */
  CVector2() : x(0.0f), y(0.0f) {}

  /**
   * @brief Constructor con componentes específicos.
   * @param initialX Valor inicial para el componente X.
   * @param initialY Valor inicial para el componente Y.
   */
  CVector2(float initialX, float initialY) : x(initialX), y(initialY) {}

  /**
   * @brief Constructor de copia.
   * @param other Otro vector CVector2 para copiar.
   */
  CVector2(const CVector2& other) : x(other.x), y(other.y) {}

  // Operadores Aritméticos
  CVector2
    operator+(const CVector2& other) const;

  CVector2
    operator-(const CVector2& other) const;

  CVector2
    operator*(float scalar) const;

  CVector2
    operator/(float scalar) const;

  CVector2&
    operator+=(const CVector2& other);

  CVector2&
    operator-=(const CVector2& other);

  CVector2&
    operator*=(float scalar);

  CVector2&
    operator/=(float scalar);

  //  Comparaciones 
  bool
    operator==(const CVector2& other) const;

  bool
    operator!=(const CVector2& other) const;

  //Acceso por Índice 
  float&
    operator[](std::size_t index);

  const float&
    operator[](std::size_t index) const;

  // Funciones Geométricas 
  float
    lengthSquared() const;

  float
    length() const;

  float
    dot(const CVector2& other) const;

  float
    cross(const CVector2& other) const;

  CVector2
    normalized() const;

  void
    normalize();

  // Métodos Estáticos de Utilidad 
  static float
    distance(const CVector2& pointA, const CVector2& pointB);

  static CVector2
    lerp(const CVector2& vectorA, const CVector2& vectorB, float t);

  static CVector2
    zero();

  static CVector2
    one();

  //  Métodos para Depuración y Utilidad de Posicionamiento (Uso conceptual) 
  void
    setPosition(const CVector2& position);

  void
    move(const CVector2& offset);

  void
    setScale(const CVector2& factors);

  void
    scale(const CVector2& factors);

  void
    setOrigin(const CVector2& origin);
};

//Sobrecarga global del operador de multiplicación por escalar
// Permite que el escalar vaya primero (ej. 5.0f * myVector)
CVector2
operator*(float scalar, const CVector2& vector);

//Sobrecarga global para depuración (operator<<) 
std::ostream&
operator<<(std::ostream& os, const CVector2& v);