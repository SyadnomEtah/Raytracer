#pragma once

#include "rtpch.h"

class vec3
{
public:
	vec3();
	vec3(double e0, double e1, double e2);

	virtual double x() const;
	virtual double y() const;
	virtual double z() const;

	virtual vec3 operator-() const;
	virtual double operator[](int i) const;
	virtual double& operator[](int i);

	virtual vec3& operator+=(const vec3& v);
	virtual vec3& operator*=(const double t);
	virtual vec3& operator/=(const double t);

	inline static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}
	inline static vec3 random(double min, double max)
	{
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	virtual double length() const;
	virtual double length_squared() const;

public:
	double e[3];
};

#pragma region Utility
inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3& v)
{
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t)
{
	return t * v;
}
inline vec3 operator/(const vec3& v, double t)
{
	return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0]	//x1 * x2
		+ u.e[1] * v.e[1]	//y1 * y2
		+ u.e[2] * v.e[2];	//z1 * z2
}
inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],	//y1 * z2 - z1 * y2
		u.e[2] * v.e[0] - u.e[0] * v.e[2],			//z1 * x2 - x1 * z2
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);			//x1 * y2 - y1 * x2
}
inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

inline vec3 random_in_unit_sphere()
{
	while (true)
	{
		auto p = vec3::random(-1, 1);

		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}
inline vec3 random_unit_vector()
{
	auto a = random_double(0, 2 * pi);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);

	return vec3(r * cos(a), r * sin(a), z);
}
inline vec3 random_in_hemisphere(const vec3& normal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();

	if (dot(in_unit_sphere, normal) > 0.0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
	auto cos_theta = dot(-uv, n);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;

	return r_out_perp + r_out_parallel;
}
#pragma endregion

#pragma region Aliases
using point3 = vec3;
using color = vec3;
#pragma endregion

