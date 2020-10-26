#include "rtpch.h"

#include "color.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

color ray_color(const ray& r, const hittable& world, int depth)
{
	hit_record rec;

	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene()
{
	hittable_list world;

	auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_shared<sphere>(point3(0, -1000.0, 0), 1000.0, ground_material));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8)
				{
					auto albedo = color::random() * color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if(choose_mat < 0.95)
				{
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
				else
				{
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}

			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

int main()
{
#pragma region Image
	const auto aspect_ratio = 3.0 / 2.0;
	const int image_width = 400;
	const int image_heigth = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixels = 500;
	const int max_depth = 5;
#pragma endregion

	auto world = random_scene();

	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	double fov = 20.0;
	auto aperture = 0.1;
	auto dist_to_focus = 10.0;

	camera cam(lookfrom, lookat, vup, fov, aspect_ratio, aperture, dist_to_focus);

#pragma region Render
	std::cout << "P3\n" << image_width << ' ' << image_heigth << "\n255\n";

	for (int j = image_heigth; j >= 0; j--)
	{
		std::cerr << "\rScanlines remainng: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; i++)
		{
			color pixel_color(0, 0, 0);

			for (int s = 0; s < samples_per_pixels; s++)
			{
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_heigth - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);

			}
			write_color(std::cout, pixel_color, samples_per_pixels);
		}
	}

	std::cerr << "\nDone.\n";
#pragma endregion
}
