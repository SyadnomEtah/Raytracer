#pragma once

#include "vec3.h"

class ray
{
public:
	ray();
	ray(const point3& origin, const vec3& direction);

	virtual point3 origin() const;
	virtual vec3 direction() const;

	virtual point3 at(double t) const;

public:
	point3 orig;
	vec3 dir;
};