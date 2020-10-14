#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <random>
#include "stb_image_write.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

struct Color
{
	unsigned char r, g, b, a;
	Color() = default;
	Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) : r(_r), g(_g), b(_b), a(_a) {};
};

vec3 random_in_unit_sphere()
{
	vec3 p;

	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::uniform_real_distribution<> dist(0, 1);

	do
	{
		p = 2.0f * vec3(dist(engine), dist(engine), dist(engine)) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1);

	return p;
}

vec3 calc_color(const ray& r, hitable* world)
{
	hit_record rec;
	if (world->hit(r, 0.0001f, FLT_MAX, rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5f * calc_color(ray(rec.p, target - rec.p), world);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{
	const int width = 200;
	const int height = 100;
	const int ns = 100;

	// output buffer
	Color colors[height][width];

	hitable* list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5f);
	list[1] = new sphere(vec3(0, -100.5f, -1), 100);
	hitable* world = new hitable_list(list, 2);
	camera cam;

	// random
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::uniform_real_distribution<> dist(0, 1);

	for (int j = height - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + dist(engine)) / float(width);
				float v = float(height - j + dist(engine)) / float(height);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0f);
				col += calc_color(r, world);
			}

			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99f * col.r());
			int ig = int(255.99f * col.g());
			int ib = int(255.99f * col.b());
			//std::cout << ir << " " << ig << " " << ib << "\n";
			colors[j][i] = Color(ir, ig, ib, 255);
		}
	}

	// output image
	stbi_write_png("output.png", width, height, sizeof(Color), colors, 0);
}