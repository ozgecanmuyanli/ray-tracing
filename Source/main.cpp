#include <iostream>
#include "color.h"
#include "vec3.h"
#include "ray.h"

// FUCNTIONS
bool isHitSphere(const point3& center, double radius, const ray& r);
color ColorRay(const ray& r);

// If there is no object in the scene, then the ray color is used to colorize pixels.
// In other words; it is background color.
color ColorRay(const ray& r) {
	auto sphereCenter = point3(0, 0, -1);
	double sphereRadius = 0.5;
	if (isHitSphere(sphereCenter, sphereRadius, r))
		return color(121.0/255.0, 16.0/255.0, 97.0/255.0);
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

// (P-C)*(P-C) = r*r, P(t) = A + tb so
// t2b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r2=0
// This function finds the roots(t) of this ray-sphere intersection equation
bool isHitSphere(const point3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction()); //r*r
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant > 0); // means that equation has at least 2 roots
}

int main()
{
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);

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
			color pixel_color = ColorRay(r);
			write_color(std::cout, pixel_color);
		}
	}
	return 0;
}