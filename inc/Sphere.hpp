#pragma once
#include "debug.hpp"
#include "RenderObject.hpp"


class Sphere : public RenderObject
{
public:
    Sphere(const Vector3& c, float r) : center(c), radius(r) {} ;

    void printSphere(char* funcName);
    float hitSphere(const Ray& ray);


private:
    const Vector3 center;
    float radius;

};
