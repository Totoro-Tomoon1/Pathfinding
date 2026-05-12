#include "pch.h"
#include "UtilsPathfinding.h"


bool UtilsPathfinding::Normalize(gce::Vector3f32& vector)
{
    // Calcul de la magnitude en 3D
    float magnitude = std::sqrt(vector.x * vector.x +
        vector.y * vector.y +
        vector.z * vector.z);

    if (magnitude != 0.0f)
    {
        vector.x /= magnitude;
        vector.y /= magnitude;
        vector.z /= magnitude;

        return true;
    }

    return false;
}

float UtilsPathfinding::GetDistance(float x1, float y1, float z1, float x2, float y2, float z2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	float z = z2 - z1;

	return std::sqrt(x * x + y * y + z * z);
}

bool UtilsPathfinding::IsInside(Geometry* shape, gce::Vector3f32 pos)
{
	gce::Vector3f32 shapePos = shape->GetPosition();
	float distance = GetDistance(shapePos.x, shapePos.y, shapePos.z, pos.x, pos.y, pos.z);

	float radius = shape->GetScale().x;

	if (distance < radius)
		return true;

	return false;
}
