#pragma once

#include "hittable.h"

class sphere : public hittable
{
public:
	sphere();
	sphere(point3 cen, double r, std::shared_ptr<material> m);

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

public:
	point3 center;
	double radius;
	std::shared_ptr<material> mat_ptr;
};