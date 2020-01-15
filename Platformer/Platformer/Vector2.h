#pragma once
struct Vector2
{
	float x;
	float y;

	Vector2& operator=(const Vector2& other);
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);
	Vector2& operator*=(const float& multiplier);
	Vector2 operator/=(const float& divider);
	Vector2 operator+(const Vector2& other);
	Vector2 operator-(const Vector2& other);
	Vector2 operator*(const float& multiplier);
	Vector2 operator/(const float& divider);
	bool operator==(const Vector2& other);
};