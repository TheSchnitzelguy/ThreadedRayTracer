#pragma once
#include "RenderObject.hpp"



class Cube : public RenderObject
{
public:
    Cube(const Vector3 p0, const Vector3 p1) : _p0(p0), _p1(p1) {};

    float hitBox(const Vector3 p0, const Vector3 p1);

private:
    const Vector3 _p0;
    const Vector3 _p1;
};
