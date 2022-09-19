#include <iostream>
#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "vec3.h"
#include "material.h"

// FUCNTIONS
color ColorRay(const ray& r, const hittable& world, int depth);

// If there is no object in the scene, then the ray color is used to colorize pixels.
// In other words; it is background color.
color ColorRay(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0) // depth is the limit of recursing
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ColorRay(scattered, world, depth - 1);
		return color(0, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //background
}

int main()
{
	// Image
	const auto aspect_ratio = 3.0 / 2.0;
	const int image_width = 1200;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 500;
	const int max_depth = 50;

	// World
	hittable_list world;
	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (size_t i = 0; i < 20; i++)
	{
		auto randomNumberX = random_double();
		auto randomNumberZ = random_double();
		auto randomMaterial = random_double();
		auto randomSignX = random_double();
		auto randomSignZ = random_double();
		int signX, signZ;
		if (randomSignX <= 0.5)
			signX = -1;
		else
			signX = 1;
		if (randomSignZ <= 0.5)
			signZ = -1;
		else
			signZ = 1;

		point3 centerPoint(signX*randomNumberX*5, 0.2, signZ*randomNumberZ*5);

		shared_ptr<material> sphere_material;
		if (randomMaterial < 0.33)
		{
			// diffuse
			auto albedo = color::random();
			sphere_material = make_shared<lambertian>(albedo);
			world.add(make_shared<sphere>(centerPoint, 0.2, sphere_material));
		}
		else if (randomMaterial < 0.8 && randomMaterial >= 0.33)
		{
			// metal
			auto albedo = color::random();
			auto fuzz = random_double(0, 0.5);
			sphere_material = make_shared<metal>(albedo, fuzz);
			world.add(make_shared<sphere>(centerPoint, 0.2, sphere_material));
		}
		else
		{
			// glass
			sphere_material = make_shared<dielectric>(1.5);
			world.add(make_shared<sphere>(centerPoint, 0.2, sphere_material));
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.6, 0.05, 0.87));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	// Camera
	// lookfrom, lookat, vup, vfov(degree), aspect_ratio
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0.2);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	// MAIN RENDER LOOP
	// Create ppm format file, image
	//left-top corner: (0,0), right-bottom corner: (1,1)
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) { 
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) { //multiple sample for a pixel
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ColorRay(r, world, max_depth); //multi-sampled color computation
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	return 0;
}