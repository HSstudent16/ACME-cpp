#ifndef VECN
#define VECN
namespace std {

  /**
   * Vector types
   */
  struct Vector2D {
    double x;
    double y;
  };
  struct Vector3D {
    double x;
    double y;
    double z;
  };

  /**
   * Vector2D Constructor functions
   */
  Vector2D vec2 ();
  Vector2D vec2 (double a);
  Vector2D vec2 (double a, double b);
  Vector2D vec2 (Vector3D v);
  Vector2D vec2 (Vector2D v);

  /**
   * Vector2D operator overloads (arithmetic)
   */
  Vector2D operator+(const Vector2D &a, const Vector2D &b);
  Vector2D operator+(const Vector2D &a, const double &b);
  Vector2D operator+(const double &a, const Vector2D &b);

  Vector2D operator+=(Vector2D &a, const Vector2D &b);
  Vector2D operator+=(Vector2D &a, const double &b);

  Vector2D operator-(const Vector2D &a, const Vector2D &b);
  Vector2D operator-(const Vector2D &a, const double &b);
  Vector2D operator-(const double &a, const Vector2D &b);

  Vector2D operator-=(Vector2D &a, const Vector2D &b);
  Vector2D operator-=(Vector2D &a, const double &b);

  Vector2D operator*(const Vector2D &a, const double &b);
  Vector2D operator*(const double &a, const Vector2D &b);

  Vector2D operator*=(Vector2D &a, const double &b);

  Vector2D operator/(const Vector2D &a, const double &b);

  Vector2D operator/=(Vector2D &a, const double &b);

  /**
   * Vector3D constructor functions
   */
  Vector3D vec3 ();
  Vector3D vec3 (double a);
  Vector3D vec3 (double a, double b, double c);
  Vector3D vec3 (Vector2D v);
  Vector3D vec3 (Vector2D v, double a);
  Vector3D vec3 (double a, Vector2D v);
  Vector3D vec3 (Vector3D v);

  /**
   * Vector3D Operator overloads (arithmetic)
   */
  Vector3D operator+(const Vector3D &a, const Vector3D &b);
  Vector3D operator+(const Vector3D &a, const double &b);
  Vector3D operator+(const double &a, const Vector3D &b);

  Vector3D operator+=(Vector3D &a, const Vector3D &b);
  Vector3D operator+=(Vector3D &a, const double &b);

  Vector3D operator-(const Vector3D &a, const Vector3D &b);
  Vector3D operator-(const Vector3D &a, const double &b);
  Vector3D operator-(const double &a, const Vector3D &b);

  Vector3D operator-=(Vector3D &a, const Vector3D &b);
  Vector3D operator-=(Vector3D &a, const double &b);

  Vector3D operator*(const Vector3D &a, const double &b);
  Vector3D operator*(const double &a, const Vector3D &b);

  Vector3D operator*=(Vector3D &a, const double &b);

  Vector3D operator/(const Vector3D &a, const double &b);

  Vector3D operator/=(Vector3D &a, const double &b);

  /**
   * Vector-specific functions
   */
  double dot(Vector2D a, Vector2D b);
  double dot(Vector3D a, Vector3D b);

  double sqMagnitude(Vector2D a);
  double sqMagnitude(Vector3D a);
  double sqMagnitude(double a, double b);
  double sqMagnitude(double a, double b, double c);

  double magnitude(Vector2D a);
  double magnitude(Vector3D a);
  double magnitude(double a, double b);
  double magnitude(double a, double b, double c);

  double cross (Vector2D a, Vector2D b);
  Vector3D cross (Vector3D a, Vector3D b);

  double mix (double a, double b, double t);
  Vector2D mix(Vector2D a, Vector2D b, double t);
  Vector3D mix(Vector3D a, Vector3D b, double t);
}
#endif