﻿#include <iostream>
#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "vec3.h"

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
		point3 target = rec.p + rec.normal + random_unit_vector();
		return 0.5 * ColorRay(ray(rec.p, target - rec.p), world, depth - 1);
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); //background
}

int main()
{
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 100;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// Camera
	// (-2, 1, -1): left-top corner, (2, -1, -1): right-bottom corner
	camera cam;

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