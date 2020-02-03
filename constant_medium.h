#pragma once
#include"hittable.h"
#include"texture.h"
#include"material.h"

class constant_medium :public hittable
{
public:
	constant_medium(hittable* b, float d, texture* a) :
		boundary(b), density(d)
	{
		phase_function = new isotropic(a);
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box)const
	{
		return boundary->bounding_box(t0, t1, box);
	}
	material* phase_function;
	hittable* boundary;
	float density;
};


bool constant_medium::hit(const ray& r, float tmin, float tmax, hit_record& rec)const
{
	hit_record rec1, rec2;
	if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1))
	{
		if (boundary->hit(r, rec1.t + 0.0001, FLT_MAX, rec2))
		{
			if (rec1.t < tmin) rec1.t = tmin;
			if (rec2.t > tmax) rec2.t = tmax;
			if (rec1.t >= rec2.t) return false;
			if (rec1.t < 0) rec1.t = 0;
			float distance_inside = rec2.t - rec1.t;
			float hit_distance = -1 / density * log(random_double());
			if (hit_distance < distance_inside)
			{
				rec.t = rec1.t + hit_distance;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = vec3(0, 1, 2);
				rec.mat_ptr = phase_function;
				return true;
			}
		}
	}
	return false;
}