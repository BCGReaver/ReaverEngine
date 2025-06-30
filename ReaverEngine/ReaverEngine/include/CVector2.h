#pragma once
#include <cmath> // Sigo sin saber como no usarla Dx de momento lo conservare profe, mi chompa no supo como adaptarlo

class
  CVector2 {
public:
  // Miembros públicos del vector ---
  float x; 
  float y; 

  // Constructores
  CVector2() : x(0.0f), y(0.0f) {}

  // Constructor con componentes específicos.
  CVector2(float initialX, float initialY) : x(initialX), y(initialY) {}

  // Constructor de copia
  CVector2(const CVector2& other) : x(other.x), y(other.y) {}

  // Operadores Aritméticos
  // Suma de vectores
  CVector2
    operator+(const CVector2& other) const {
    return CVector2(x + other.x, y + other.y);
  }

  // Resta de vectores
  CVector2
    operator-(const CVector2& other) const {
    return CVector2(x - other.x, y - other.y);
  }

  // Multiplicación de vectores
  CVector2
    operator*(float scalar) const {
    return CVector2(x * scalar, y * scalar);
  }

  // División de Vectores
  CVector2
    operator/(float scalar) const {
    if (scalar == 0.0f) {
      return CVector2(0.0f, 0.0f); // Evitar división por cero
    }
    return CVector2(x / scalar, y / scalar);
  }

  //Operador de asignación con suma
  CVector2&
    operator+=(const CVector2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  //Operador de asignación con resta
  CVector2&
    operator-=(const CVector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  //Operador de asignación con multiplicación
  CVector2&
    operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  //Operador de asignación con división
  CVector2&
    operator/=(float scalar) {
    if (scalar != 0.0f) {
      x /= scalar;
      y /= scalar;
    }
    return *this;
  }

  // Comparaciones
  // Igualdad
  bool
    operator==(const CVector2& other) const {
    return (x == other.x && y == other.y);
  }

  // Desigualdad
  bool
    operator!=(const CVector2& other) const {
    return !(*this == other);
  }

  // ¿Acceso por Índice? Tengo mis dudas aqui Dx
  // Acceso a componentes del vector por índice.
  float&
    operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    return x;
  }

  // Acceso a componentes del vector por índice (const)
  const float&
    operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    return x;
  }

  // Funciones Geométricas
  // Calcula la longitud al cuadrado del vector
  float
    lengthSquared() const {
    return (x * x) + (y * y);
  }

  // Calcula la longitud (magnitud) del vector
  float
    length() const {
    return std::sqrt(lengthSquared());
  }

  // Calcula el producto escalar (dot product) con otro vector
  float
    dot(const CVector2& other) const {
    return (x * other.x) + (y * other.y);
  }

  //Calcula el "producto cruz" 2D (que es un escalar, el componente Z)
  float
    cross(const CVector2& other) const {
    return (x * other.y) - (y * other.x);
  }

  // Devuelve un vector normalizado (longitud = 1)
  CVector2
    normalized() const {
    float len = length();
    if (len == 0.0f) {
      return CVector2(0.0f, 0.0f); // Devuelve vector cero si la longitud es cero
    }
    return CVector2(x / len, y / len);
  }

  // Normaliza este vector in-place (modifica el vector actual)
  void
    normalize() {
    float len = length();
    if (len != 0.0f) {
      x /= len;
      y /= len;
    }
  }

  // Métodos Estáticos de Utilidad
  // Calcula la distancia entre dos puntos (vectores)
  static float
    distance(const CVector2& a, const CVector2& b) {
    CVector2 diff = b - a; // Vector diferencia
    return diff.length();
  }

  // Realiza una interpolación lineal entre dos vectores
  static CVector2
    lerp(const CVector2& a, const CVector2& b, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return a + (b - a) * t;
  }

  // Devuelve un vector CVector2(0, 0)
  static CVector2
    zero() {
    return CVector2(0.0f, 0.0f);
  }

  // Devuelve un vector CVector2(1, 1)
  static CVector2
    one() {
    return CVector2(1.0f, 1.0f);
  }

  // Métodos para Depuración y Utilidad de Posicionamiento
  // Establece la posición absoluta en coordenadas del objeto
  void
    setPosition(const CVector2& position) {
    this->x = position.x;
    this->y = position.y;
  }

  // Traslada el objeto respecto a su posición actual
  void
    move(const CVector2& offset) {
    this->x += offset.x;
    this->y += offset.y;
  }

  // Define la escala X e Y simultáneamente
  void
    setScale(const CVector2& factors) {
    this->x = factors.x;
    this->y = factors.y;
  }

  // Multiplica la escala actual por los factores dados
  void
    scale(const CVector2& factors) {
    this->x *= factors.x;
    this->y *= factors.y;
  }

  // Fija el punto de origen
  void
    setOrigin(const CVector2& origin) {
  }
};

// Sobrecarga global del operador de multiplicación
inline CVector2
operator*(float scalar, const CVector2& vector) {
  return CVector2(vector.x * scalar, vector.y * scalar);
}
