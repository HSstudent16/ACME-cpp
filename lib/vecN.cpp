#include "vecN.hpp"
#include <math.h>

namespace std {

  /**
   * Vector2D constructor functions
   */
  Vector2D vec2 () {
    return Vector2D{0, 0};
  }
  Vector2D vec2 (double a) {
    return Vector2D{a, a};
  }
  Vector2D vec2 (double a, double b) {
    return Vector2D{a, b};
  }
  Vector2D vec2 (Vector3D v) {
    return Vector2D{v.x, v.y};
  }
  Vector2D vec2 (Vector2D v) {
    return Vector2D{v.x, v.y};
  }

  /**
   * Vector2D Operator overloads (arithmetic)
   */
  Vector2D operator+(const Vector2D &a, const Vector2D &b) {
    return Vector2D{a.x + b.x, a.y + b.y};
  }
  Vector2D operator+(const Vector2D &a, const double &b) {
    return Vector2D{a.x + b, a.y + b};
  }
  Vector2D operator+(const double &a, const Vector2D &b) {
    return Vector2D{a + b.x, a + b.y};
  }

  Vector2D operator+=(Vector2D &a, const Vector2D &b) {
    a.x += b.x;
    a.y += b.y;
    return a;
  }
  Vector2D operator+=(Vector2D &a, const double &b) {
    a.x += b;
    a.y += b;
    return a;
  }

  Vector2D operator-(const Vector2D &a, const Vector2D &b) {
    return Vector2D{a.x - b.x, a.y - b.y};
  }
  Vector2D operator-(const Vector2D &a, const double &b) {
    return Vector2D{a.x - b, a.y - b};
  }
  Vector2D operator-(const double &a, const Vector2D &b) {
    return Vector2D{a - b.x, a - b.y};
  }

  Vector2D operator-=(Vector2D &a, const Vector2D &b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
  }
  Vector2D operator-=(Vector2D &a, const double &b) {
    a.x -= b;
    a.y -= b;
    return a;
  }

  Vector2D operator*(const Vector2D &a, const double &b) {
    return Vector2D{a.x * b, a.y * b};
  }
  Vector2D operator*(const double &a, const Vector2D &b) {
    return Vector2D{a * b.x, a * b.y};
  }

  Vector2D operator*=(Vector2D &a, const double &b) {
    a.x *= b;
    a.y *= b;
    return a;
  }

  Vector2D operator/(const Vector2D &a, const double &b) {
    return Vector2D {a.x / b, a.y / b};
  }

  Vector2D operator/=(Vector2D &a, const double &b) {
    a.x /= b;
    a.y /= b;
    return a;
  }

  /**
   * Vector3D constructor functions
   */
  Vector3D vec3 () {
    return Vector3D{0.0, 0.0, 0.0};
  }
  Vector3D vec3 (double a) {
    return Vector3D{a, a, a};
  }
  Vector3D vec3 (double a, double b, double c) {
    return Vector3D{a, b, c};
  }
  Vector3D vec3 (Vector2D v) {
    return Vector3D{v.x, v.y, 0.0};
  }
  Vector3D vec3 (Vector2D v, double a) {
    return Vector3D{v.x, v.y, a};
  }
  Vector3D vec3 (double a, Vector2D v) {
    return Vector3D{a, v.x, v.y};
  }
  Vector3D vec3 (Vector3D v) {
    return Vector3D{v.x, v.y, v.z};
  }

  /**
   * Vector3D Operator overloads (arithmetic)
   */
  Vector3D operator+(const Vector3D &a, const Vector3D &b) {
    return Vector3D{a.x + b.x, a.y + b.y, a.z + b.z};
  }
  Vector3D operator+(const Vector3D &a, const double &b) {
    return Vector3D{a.x + b, a.y + b, a.z + b};
  }
  Vector3D operator+(const double &a, const Vector3D &b) {
    return Vector3D{a + b.x, a + b.y, a + b.z};
  }

  Vector3D operator+=(Vector3D &a, const Vector3D &b) {
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
  }
  Vector3D operator+=(Vector3D &a, const double &b) {
    a.x += b;
    a.y += b;
    a.z += b;
    return a;
  }

  Vector3D operator-(const Vector3D &a, const Vector3D &b) {
    return Vector3D{a.x - b.x, a.y - b.y, a.z - b.z};
  }
  Vector3D operator-(const Vector3D &a, const double &b) {
    return Vector3D{a.x - b, a.y - b, a.z - b};
  }
  Vector3D operator-(const double &a, const Vector3D &b) {
    return Vector3D{a - b.x, a - b.y, a - b.z};
  }

  Vector3D operator-=(Vector3D &a, const Vector3D &b) {
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
  }
  Vector3D operator-=(Vector3D &a, const double &b) {
    a.x -= b;
    a.y -= b;
    a.z -= b;
    return a;
  }

  Vector3D operator*(const Vector3D &a, const double &b) {
    return Vector3D{a.x * b, a.y * b, a.z * b};
  }
  Vector3D operator*(const double &a, const Vector3D &b) {
    return Vector3D{a * b.x, a * b.y, a * b.z};
  }

  Vector3D operator*=(Vector3D &a, const double &b) {
    a.x *= b;
    a.y *= b;
    a.z *= b;
    return a;
  }

  Vector3D operator/(const Vector3D &a, const double &b) {
    return Vector3D {a.x / b, a.y / b, a.z / b};
  }

  Vector3D operator/=(Vector3D &a, const double &b) {
    a.x /= b;
    a.y /= b;
    a.z /= b;
    return a;
  }

  /**
   * Vector-specific functions
   */
  double dot(Vector2D a, Vector2D b) {
    return a.x * b.x + a.y * b.y;
  }
  double dot(Vector3D a, Vector3D b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
  }

  double sqMagnitude(Vector2D a) {
    return a.x * a.x + a.y * a.y;
  }
  double sqMagnitude(Vector3D a) {
    return a.x * a.x + a.y * a.y + a.z * a.z;
  }
  double sqMagnitude(double a, double b) {
    return a * a + b * b;
  }
  double sqMagnitude(double a, double b, double c) {
    return a * a + b * b + c * c;
  }

  double magnitude(Vector2D a) {
    return sqrt(sqMagnitude(a));
  }
  double magnitude(Vector3D a) {
    return sqrt(sqMagnitude(a));
  }
  double magnitude(double a, double b) {
    return sqrt(sqMagnitude(a, b));
  }
  double magnitude(double a, double b, double c) {
    return sqrt(sqMagnitude(a, b, c));
  }

  double cross (Vector2D a, Vector2D b) {
    return a.x * b.y - b.x * a.y;
  }
  Vector3D cross (Vector3D a, Vector3D b) {
    return Vector3D {
      a.y * b.z - a.z * b.y,
      a.x * b.z - a.z * b.x,
      a.x * b.y - a.y * b.x
    };
  }

  double mix (double a, double b, double t) {
    return a + (b - a) * t;
  }
  Vector2D mix(Vector2D a, Vector2D b, double t) {
    return Vector2D {
      mix(a.x, b.x, t),
      mix(a.y, b.y, t)
    };
  }
  Vector3D mix(Vector3D a, Vector3D b, double t) {
    return Vector3D {
      mix(a.x, b.x, t),
      mix(a.y, b.y, t),
      mix(a.z, b.z, t)
    };
  }
}
