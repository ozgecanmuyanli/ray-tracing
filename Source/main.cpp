#include <iostream>
#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

// FUCNTIONS
color ColorRay(const ray& r, const hittable& world);

// If there is no object in the scene, then the ray color is used to colorize pixels.
// In other words; it is background color.
color ColorRay(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, 0, infinity, rec)) {
		return 0.5 * (rec.normal + color(1, 1, 1));
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, 1, -1), 0.3));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// Camera
	// (-2, 1, -1): left-top corner, (2, -1, -1): right-bottom corner
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0; // distance between the projection plane and the projection point

	auto origin = point3(0, 0, 0); // camera position
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);
	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
	
	// MAIN RENDER LOOP
	// Create ppm format file, image
	//left-top corner: (0,0), right-bottom corner: (1,1)
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = image_height - 1; j >= 0; --j) { 
		for (int i = 0; i < image_width; ++i) {
			auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ColorRay(r, world);
			write_color(std::cout, pixel_color);
		}
	}
	return 0;
}