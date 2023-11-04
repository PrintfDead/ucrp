#pragma once
#include <corecrt_math.h>

typedef struct {
	float x;
	float y;
} Point2D;

static inline Point2D Point2DMake(float x, float y)
{
	return Point2D{ x, y };
}

static inline float Point2DDistance(Point2D a, Point2D b)
{
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}

#define Point2DZero Point2D{0, 0}

typedef struct {
	float x;
	float y;
	float z;
} Point3D;

static inline Point3D Point3DMake(float x, float y, float z)
{
	return Point3D{ x, y, z };
}

static inline float Point3DDistance(Point3D a, Point3D b)
{
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2));
}

#define Point3DZero Point3D{0, 0, 0}