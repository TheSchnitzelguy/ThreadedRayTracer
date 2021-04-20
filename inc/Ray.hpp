#pragma once
#include "linAlg.hpp"


class Ray
{
public:

    Vector3 origin;
    Vector3 direction;

    Ray();
    Ray(const Vector3& o, const Vector3& d) : origin(o), direction(d){};

    Vector3 pointAtParameter(float t) const;
};
