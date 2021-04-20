#include <iostream>
#include "../inc/Sphere.hpp"


void Sphere::printSphere(char* funcName)
{
    std::cout << "Sphere object spawned with function " << funcName << std::endl;
}


/*

  formula for sphere at the origin:
  x*x + y*y + z*z == R*R

  formula for sphere at any position:
  (x-cx)*(x-cx) + (y-cy)*(y-cy) + (z-cz)*(z-cz) == R*R

  in vector form (p = point ray hits the sphere, c = sphere center):
  dot((p-c),(p-c)) == R*R

  adding formula for our ray (p(t) = point on ray at time t:
  dot((p(t)-c),(p(t)-c)) == R*R

  full formula for using ray (ray formula is A + t*B):
  dot((A + t*B -c),(A + t*B -c)) == R*R

  FOIL, etc.:
  t*t*dot(B,B) + 2*t*dot(B, A-C) + dot(A-C, A-C) - R*R == 0

  A, B, and C are constants, so we solve the quadratic with the quadratic formula

*/
float Sphere::hitSphere(const Ray& ray)
{
#ifdef DEBUG
    printSphere("hitSphere");
#endif

    Vector3 rayOriginToCenter = ray.origin - center;

    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(rayOriginToCenter, ray.direction);
    float c = dot(rayOriginToCenter, rayOriginToCenter) - radius * radius;
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0)
    {
        return -1.0f;
    }
    else
    {
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }
}

