#pragma once
#include <iostream>
#include <math.h>

class Vector3
{
public:
    Vector3();

    Vector3(float e0, float e1, float e2);

    inline float x() const {return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }

    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }


    inline Vector3& operator+=(const Vector3& v);
    inline Vector3& operator/=(const float t);


    float e[3];

    float length() const;

};

