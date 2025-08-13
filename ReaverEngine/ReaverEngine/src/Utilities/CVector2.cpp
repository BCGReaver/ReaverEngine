/**
 * @file CVector2.cpp
 * @brief Implementación de la clase CVector2 para operaciones con vectores 2D.
 *
 * @details
 * CVector2 es una clase utilitaria que ofrece:
 * - Constructores para inicializar vectores.
 * - Sobrecarga de operadores aritméticos y de asignación.
 * - Métodos para cálculos geométricos como longitud, producto punto y cruz.
 * - Funciones estáticas para utilidades comunes (distancia, interpolación, etc.).
 */

#include "Utilities/CVector2.h"

 // ==========================
 // Constructores
 // ==========================

 /**
  * @brief Constructor por defecto: inicializa el vector en (0,0).
  */
CVector2::CVector2() : x(0.f), y(0.f) {}

/**
 * @brief Constructor con valores iniciales.
 * @param x_ Valor de la componente X.
 * @param y_ Valor de la componente Y.
 */
CVector2::CVector2(float x_, float y_) : x(x_), y(y_) {}

// ==========================
// Operadores aritméticos
// ==========================

CVector2 CVector2::operator+(const CVector2& v) const {
  return { x + v.x, y + v.y };
}

CVector2 CVector2::operator-(const CVector2& v) const {
  return { x - v.x, y - v.y };
}

CVector2 CVector2::operator*(float s) const {
  return { x * s, y * s };
}

CVector2 CVector2::operator/(float s) const {
  return { x / s, y / s };
}

CVector2& CVector2::operator+=(const CVector2& v) {
  x += v.x;
  y += v.y;
  return *this;
}

CVector2& CVector2::operator-=(const CVector2& v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

CVector2& CVector2::operator*=(float s) {
  x *= s;
  y *= s;
  return *this;
}

CVector2& CVector2::operator/=(float s) {
  x /= s;
  y /= s;
  return *this;
}

// ==========================
// Comparaciones
// ==========================

bool CVector2::operator==(const CVector2& v) const {
  return x == v.x && y == v.y;
}

bool CVector2::operator!=(const CVector2& v) const {
  return !(*this == v);
}

// ==========================
// Acceso por índice
// ==========================

float& CVector2::operator[](std::size_t i) {
  return (i == 0) ? x : y;
}

const float& CVector2::operator[](std::size_t i) const {
  return (i == 0) ? x : y;
}

// ==========================
// Funciones geométricas
// ==========================

/**
 * @brief Calcula la longitud (magnitud) del vector.
 * @return Valor de la longitud.
 */
float CVector2::length() const {
  return std::sqrt(x * x + y * y);
}

/**
 * @brief Calcula la longitud al cuadrado (evita el sqrt).
 * @return Longitud al cuadrado.
 */
float CVector2::lengthSquared() const {
  return x * x + y * y;
}

/**
 * @brief Calcula el producto punto con otro vector.
 */
float CVector2::dot(const CVector2& v) const {
  return x * v.x + y * v.y;
}

/**
 * @brief Calcula el producto cruz 2D (retorna un escalar).
 */
float CVector2::cross(const CVector2& v) const {
  return x * v.y - y * v.x;
}

/**
 * @brief Devuelve una copia del vector normalizado.
 */
CVector2 CVector2::normalized() const {
  float len = length();
  return (len != 0.f) ? CVector2{ x / len, y / len } : CVector2{ 0.f, 0.f };
}

/**
 * @brief Normaliza el vector en el lugar.
 */
void CVector2::normalize() {
  float len = length();
  if (len != 0.f) { x /= len; y /= len; }
}

// ==========================
// Métodos estáticos
// ==========================

/**
 * @brief Calcula la distancia entre dos vectores.
 */
float CVector2::distance(const CVector2& a, const CVector2& b) {
  return (a - b).length();
}

/**
 * @brief Interpolación lineal entre dos vectores.
 * @param t Factor de interpolación (0 = a, 1 = b).
 */
CVector2 CVector2::lerp(const CVector2& a, const CVector2& b, float t) {
  return a + (b - a) * t;
}

/**
 * @brief Devuelve el vector (0,0).
 */
CVector2 CVector2::zero() {
  return { 0.f, 0.f };
}

/**
 * @brief Devuelve el vector (1,1).
 */
CVector2 CVector2::one() {
  return { 1.f, 1.f };
}

// ==========================
// Depuración
// ==========================

/**
 * @brief Sobrecarga del operador << para imprimir el vector.
 */
std::ostream& operator<<(std::ostream& os, const CVector2& v) {
  return os << "(" << v.x << ", " << v.y << ")";
}
