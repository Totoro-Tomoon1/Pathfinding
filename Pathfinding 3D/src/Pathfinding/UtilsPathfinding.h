#pragma once

class UtilsPathfinding
{
public:
	static bool Normalize(gce::Vector3f32& vector);
	static float GetDistance(float x1, float y1, float z1, float x2, float y2, float z2);
	static bool IsInside(Geometry* shape, gce::Vector3f32 pos);
};

