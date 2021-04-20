#include "../inc/Camera.hpp"

Camera::Camera()
{}


Ray Camera::getRay(float s, float t) const
{
    return Ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin);
}

