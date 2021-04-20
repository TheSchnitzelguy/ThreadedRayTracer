#include "../inc/Ray.hpp"


Ray::Ray()
{}

/* returns the linear interpolation point of t
   by using linear intepolation, we'll only get the parts in front of the object projected onto the plane
*/
Vector3 Ray::pointAtParameter(float t) const
{
    return origin + t * direction;
}

