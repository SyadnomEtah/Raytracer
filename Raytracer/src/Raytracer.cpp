#include "rtpch.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

color ray_color(const ray& r, const hittable& world)
{
	hit_record rec;

	if (world.hit(r, 0, infinity, rec))
		return 0.5 * (rec.normal + color(1, 1, 1));

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
#pragma region Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_heigth = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixels = 100;
#pragma endregion

#pragma region World
	hittable_list world;
	world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));
#pragma endregion

	camera cam;

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
				pixel_color += ray_color(r, world);

			}
			write_color(std::cout, pixel_color, samples_per_pixels);
		}
	}

	std::cerr << "\nDone.\n";
#pragma endregion
}
