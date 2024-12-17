#pragma once
class SphereCollider
{
	float radius_;

public:
	SphereCollider(float radius);
	float GetRadius() { return radius_; }
};

