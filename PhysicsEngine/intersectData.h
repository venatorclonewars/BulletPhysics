#pragma once

class IntersectData
{
public:
	IntersectData(const bool doesIntersect, const float distance)
		: m_doesIntersect(doesIntersect), m_distance(distance) {}

	inline bool getDoesIntersect() const { return m_doesIntersect; }
	inline float getDistance() const { return m_distance; }

private:
	const bool m_doesIntersect;
	const float m_distance;
};
