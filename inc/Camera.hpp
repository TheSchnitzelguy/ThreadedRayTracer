#pragma once

#include "Ray.hpp"


class Camera
{
public:
    Camera();
    Ray getRay(float s, float t) const;

    //vectors that determine the out of which the rays should be projected
    Vector3 lowerLeftCorner{ -2, -1, -1 };
    Vector3 horizontal{ 4, 0, 0 };
    Vector3 vertical{ 0, 2, 0 };
    Vector3 origin{ 0, 0, 0 };


};
