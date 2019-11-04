#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include "stb_image_write.h"
#include "vec3.h"
#include "ray.h"

struct Color
{
	unsigned char r, g, b, a;
	Color() = default;
	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : r(_r), g(_g), b(_b), a(_a) {};
};

vec3 calc_color(const ray& r)
{
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

int main()
{
	const int width = 200;
	const int height = 100;

	Color colors[height][width];
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			float u = float(i) / float(width);
			float v = float(height - j) / float(height);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = calc_color(r);
			int ir = int(255.99f * col.r());
			int ig = int(255.99f * col.g());
			int ib = int(255.99f * col.b());
			//std::cout << ir << " " << ig << " " << ib << "\n";
			colors[j][i] = Color(ir, ig, ib, 255);
		}
	}

	stbi_write_png("output.png", width, height, sizeof(Color), colors, 0);
}