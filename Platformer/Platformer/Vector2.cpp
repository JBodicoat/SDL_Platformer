#pragma once
#include "Vector2.h"

Vector2& Vector2::operator=(const Vector2& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2& Vector2::operator*=(const float& multiplier)
{
	x *= multiplier;
	y *= multiplier;

	return *this;
}

Vector2 Vector2::operator/=(const float& divider)
{
	x /= divider;
	y /= divider;

	return *this;
}

Vector2 Vector2::operator+(const Vector2& other)
{
	Vector2 result{ x + other.x, y + other.y };

	return result;
}

Vector2 Vector2::operator-(const Vector2& other)
{
	Vector2 result{ x - other.x, y - other.y };

	return result;
}

Vector2 Vector2::operator*(const float& multiplier)
{
	Vector2 result{ x * multiplier, y * multiplier };

	return result;
}

Vector2 Vector2::operator/(const float& divider)
{
	Vector2 result{ x / divider, y / divider };

	return result;
}

bool Vector2::operator==(const Vector2& other)
{
	if (x = other.x && y == other.y)
	{
		return true;
	}

	return false;
}
