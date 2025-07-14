#include "Utilities/CVector2.h" 


// Operadores Aritméticos
CVector2
CVector2::operator+(const CVector2& other) const {
  return CVector2(x + other.x, y + other.y);
}

CVector2
CVector2::operator-(const CVector2& other) const {
  return CVector2(x - other.x, y - other.y);
}

CVector2
CVector2::operator*(float scalar) const {
  return CVector2(x * scalar, y * scalar);
}

CVector2
CVector2::operator/(float scalar) const {
  if (scalar == 0.0f) {
    return CVector2(0.0f, 0.0f); // Evitar división por cero
  }
  return CVector2(x / scalar, y / scalar);
}

CVector2&
CVector2::operator+=(const CVector2& other) {
  x += other.x;
  y += other.y;
  return *this;
}

CVector2&
CVector2::operator-=(const CVector2& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

CVector2&
CVector2::operator*=(float scalar) {
  x *= scalar;
  y *= scalar;
  return *this;
}

CVector2&
CVector2::operator/=(float scalar) {
  if (scalar != 0.0f) {
    x /= scalar;
    y /= scalar;
  }
  return *this;
}

// Comparaciones
bool
CVector2::operator==(const CVector2& other) const {
  return (x == other.x && y == other.y);
}
bool
CVector2::operator!=(const CVector2& other) const {
  return !(*this == other);
}

// Acceso por Índice
float&
CVector2::operator[](std::size_t index) {
  return (index == 0) ? this->x : this->y;
}

const float&
CVector2::operator[](std::size_t index) const {
  return (index == 0) ? this->x : this->y;
}

// Funciones Geométricas
float
CVector2::lengthSquared() const {
  return (x * x) + (y * y);
}

float
CVector2::length() const {
  return std::sqrt(lengthSquared());
}

float
CVector2::dot(const CVector2& other) const {
  return (x * other.x) + (y * other.y);
}

float
CVector2::cross(const CVector2& other) const {
  return (x * other.y) - (y * other.x);
}

CVector2
CVector2::normalized() const {
  float len = length();
  if (len == 0.0f) {
    return CVector2(0.0f, 0.0f);
  }
  return CVector2(x / len, y / len);
}

void
CVector2::normalize() {
  float len = length();
  if (len != 0.0f) {
    x /= len;
    y /= len;
  }
}

// Métodos Estáticos de Utilidad
float
CVector2::distance(const CVector2& pointA, const CVector2& pointB) {
  CVector2 diff = pointB - pointA;
  return diff.length();
}

CVector2
CVector2::lerp(const CVector2& vectorA, const CVector2& vectorB, float t) {
  if (t < 0.0f) t = 0.0f;
  if (t > 1.0f) t = 1.0f;
  return vectorA + (vectorB - vectorA) * t;
}

CVector2
CVector2::zero() {
  return CVector2(0.0f, 0.0f);
}

CVector2
CVector2::one() {
  return CVector2(1.0f, 1.0f);
}

// Métodos para Depuración y Utilidad de Posicionamiento (Uso conceptual)
void
CVector2::setPosition(const CVector2& position) {
  this->x = position.x;
  this->y = position.y;
}

void
CVector2::move(const CVector2& offset) {
  this->x += offset.x;
  this->y += offset.y;
}

void
CVector2::setScale(const CVector2& factors) {
  this->x = factors.x;
  this->y = factors.y;
}

void
CVector2::scale(const CVector2& factors) {
  this->x *= factors.x;
  this->y *= factors.y;
}

void
CVector2::setOrigin(const CVector2& origin) {
  // Este método conceptualmente establecería un punto de origen
  // en otra clase que CONTIENE un CVector2. No tiene un efecto directo
  // en los miembros x,y del CVector2 actual para este reto.
}

// Sobrecarga global del operador de multiplicación por escalar
CVector2
operator*(float scalar, const CVector2& vector) {
  return CVector2(vector.x * scalar, vector.y * scalar);
}

// Sobrecarga global para depuración (operator<<)
std::ostream&
operator<<(std::ostream& os, const CVector2& v) {
  return os << "(" << v.x << ", " << v.y << ")";
}