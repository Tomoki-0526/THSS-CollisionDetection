#include "Vector3D.h"

Vector3D::Vector3D() :
    x(0), y(0), z(0)
{
}

Vector3D::Vector3D(float x, float y, float z) :
    x(x), y(y), z(z)
{
}

float Vector3D::norm()
{
    return x * x + y * y + z * z + 0.00000001;
}

void Vector3D::operator+=(const Vector3D& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

Vector3D Vector3D::operator-(const Vector3D& v)
{
    return Vector3D(x - v.x, y - v.y, z - v.z);
}

Vector3D Vector3D::operator-(const float diff)
{
    return Vector3D(x - diff, y - diff, z - diff);
}

Vector3D Vector3D::operator*(const float scalar)
{
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

float Vector3D::operator*(const Vector3D& v)
{
    return x * v.x + y * v.y + z * v.z;
}

Vector3D Vector3D::operator/(const float scalar)
{
    return Vector3D(x / scalar, y / scalar, z / scalar);
}

Vector3D Vector3D::operator-()
{
    return Vector3D(-x, -y, -z);
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
    if (&v != this) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}
