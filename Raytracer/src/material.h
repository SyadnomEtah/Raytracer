#pragma once
#include "rtpch.h"

#include "hittable.h"

class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const color& a);
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
	color albedo;
};

class metal : public material
{
public:
	metal(const color& a, double f);
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;
public:
	color albedo;
	double fuzz;
};

