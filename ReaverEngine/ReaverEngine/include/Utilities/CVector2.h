/**
 * @file CVector2.h
 * @brief Declaración de la clase CVector2 para operaciones con vectores 2D.
 *
 * @details
 * CVector2 es una clase utilitaria para el manejo y operaciones matemáticas con vectores en 2D.
 * Proporciona:
 * - Constructores y operadores sobrecargados para operaciones vectoriales comunes.
 * - Métodos geométricos como magnitud, normalización, producto punto y producto cruz.
 * - Funciones estáticas para operaciones como distancia e interpolación lineal.
 */

#pragma once
#include "../Prerequisites.h"

 /**
  * @class CVector2
  * @brief Representa un vector 2D con componentes en punto flotante.
  */
class CVector2 {
public:
  /// Componente X del vector.
  float x;
  /// Componente Y del vector.
  float y;

  // ==========================
  // Constructores
  // ==========================

  /**
   * @brief Constructor por defecto. Inicializa el vector en (0,0).
   */
  CVector2();

  /**
   * @brief Constructor con valores iniciales.
   * @param x_ Valor de la componente X.
   * @param y_ Valor de la componente Y.
   */
  CVector2(float x_, float y_);

  // ==========================
  // Operadores aritméticos
  // ==========================

  /**
   * @brief Suma de vectores.
   */
  CVector2 operator+(const CVector2& v) const;

  /**
   * @brief Resta de vectores.
   */
  CVector2 operator-(const CVector2& v) const;

  /**
   * @brief Multiplicación por un escalar.
   */
  CVector2 operator*(float s) const;

  /**
   * @brief División por un escalar.
   */
  CVector2 operator/(float s) const;

  /**
   * @brief Suma-igual con otro vector.
   */
  CVector2& operator+=(const CVector2& v);

  /**
   * @brief Resta-igual con otro vector.
   */
  CVector2& operator-=(const CVector2& v);

  /**
   * @brief Multiplicación-igual por escalar.
   */
  CVector2& operator*=(float s);

  /**
   * @brief División-igual por escalar.
   */
  CVector2& operator/=(float s);

  // ==========================
  // Comparaciones
  // ==========================

  /**
   * @brief Compara si dos vectores son iguales.
   */
  bool operator==(const CVector2& v) const;

  /**
   * @brief Compara si dos vectores son diferentes.
   */
  bool operator!=(const CVector2& v) const;

  // ==========================
  // Acceso por índice
  // ==========================

  /**
   * @brief Accede a una componente por índice (0 = x, 1 = y).
   */
  float& operator[](std::size_t i);

  /**
   * @brief Accede a una componente por índice (0 = x, 1 = y) (const).
   */
  const float& operator[](std::size_t i) const;

  // ==========================
  // Funciones geométricas
  // ==========================

  /**
   * @brief Devuelve la longitud (magnitud) del vector.
   */
  float length() const;

  /**
   * @brief Devuelve la longitud al cuadrado del vector (evita sqrt).
   */
  float lengthSquared() const;

  /**
   * @brief Calcula el producto punto con otro vector.
   */
  float dot(const CVector2& v) const;

  /**
   * @brief Calcula el producto cruz en 2D (retorna un escalar).
   */
  float cross(const CVector2& v) const;

  /**
   * @brief Retorna una copia del vector normalizado.
   */
  CVector2 normalized() const;

  /**
   * @brief Normaliza el vector en el lugar.
   */
  void normalize();

  // ==========================
  // Métodos estáticos
  // ==========================

  /**
   * @brief Calcula la distancia entre dos vectores.
   */
  static float distance(const CVector2& a, const CVector2& b);

  /**
   * @brief Interpolación lineal entre dos vectores.
   * @param t Factor de interpolación (0 = a, 1 = b).
   */
  static CVector2 lerp(const CVector2& a, const CVector2& b, float t);

  /**
   * @brief Devuelve un vector (0,0).
   */
  static CVector2 zero();

  /**
   * @brief Devuelve un vector (1,1).
   */
  static CVector2 one();

  // ==========================
  // Depuración
  // ==========================

  /**
   * @brief Sobrecarga del operador << para impresión en streams.
   */
  friend std::ostream& operator<<(std::ostream& os, const CVector2& v);
};
