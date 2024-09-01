#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 {
	float x;
	float y;

	Vector2 operator+(const Vector2& _v);
	Vector2 operator*(float _f) const;
	Vector2 operator*(const float& _f);
};
