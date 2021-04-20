#include "../inc/Vector3.hpp"

Vector3::Vector3()
{}

Vector3::Vector3(float e0, float e1, float e2)
{
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
}

float Vector3::length() const
{
    return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
}

