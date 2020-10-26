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

class dielectric : public material
{
public:
	dielectric(double index_of_refraction);
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
	double ir;

private:
	static double reflectance(double cosine, double ref_idx)
	{
		//Approximation of reflectance
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;

		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

