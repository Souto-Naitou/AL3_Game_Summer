#include "mathExtension.h"
#include <cmath>
#include <math/Vector3/calc/vector3calc.h>

float cotanf(float _x)
{
	return 1.0f / std::tanf(_x);
}

float clamp(float _x, float _min, float _max)
{
	float result = _x;
	if (_x < _min) result = _min;
	else if (_x > _max) result = _max;
	return result;
}

int clamp(int _x, int _min, int _max)
{
	int result = _x;
	if (_x < _min) result = _min;
	else if (_x > _max) result = _max;
	return result;
}

float Lerp(float _x, float _y, float _t)
{
	return (1.0f - _t) * _x + _t * _y;
}

Vector3 TransformNormal(const Vector3& _v, const Matrix4x4& _m)
{
	Vector3 result
	{
		_v.x * _m.m[0][0] + _v.y * _m.m[1][0] + _v.z * _m.m[2][0],
		_v.x * _m.m[0][1] + _v.y * _m.m[1][1] + _v.z * _m.m[2][1],
		_v.x * _m.m[0][2] + _v.y * _m.m[1][2] + _v.z * _m.m[2][2]
	};

	return result;
}

Vector3 Lerp(const Vector3& _vx, const Vector3& _vy, float _t)
{
	return (1.0f - _t) * _vx + _t * _vy;
}

Vector3 Slerp(const Vector3& _vx, const Vector3& _vy, float _t)
{
	Vector3 normvx = Normalize(_vx);
	Vector3 normvy = Normalize(_vy);
	// dot
	float dot = Dot(normvx, normvy);
	// gosa
	dot = dot > 1.0f ? 1.0f : dot;
	// arccos
	float theta = std::acos(dot);
	// theta to sin(theta)
	float sinTheta = std::sin(theta);
	// sin(theta(1-t))
	float sinThetaFrom = std::sin((1.0f - _t) * theta);
	// sin theta t
	float sinThetaTo = std::sin(_t * theta);
	Vector3 正規化補間ベクトル = sinThetaFrom / sinTheta * normvx + sinThetaTo / sinTheta * normvy;
	// prevent zero division
	if (sinTheta < 1.0e-5)
	{
		正規化補間ベクトル = normvx;
	}
	else
	{
		// 球面線形補間したベクトル
		正規化補間ベクトル = (sinThetaFrom * normvx + sinThetaTo * normvy) * (1.0f / sinTheta);
	}
	float lengthx = Length(_vx);
	float lengthy = Length(_vy);
	float length = Lerp(lengthx, lengthy, _t);

	//float vxlen = Length(_vx);
	//float vylen = Length(_vy);
	//if (vylen < 1.0e-5) { vylen = 0.00001f; }
	//Vector3 evx = _vx * (1.0f / vxlen);
	//Vector3 evy = _vy * (1.0f / vylen);
	//float theta = acosf(Dot(_vx, _vy) / vxlen * vylen);
	//float s = Lerp(vxlen, vylen, _t);
	//resultPoint = s * Vector3(std::sinf(1.0f - _t) * theta / std::sinf(theta) * evx + std::sinf(_t * theta) / std::sinf(theta) * evy);
	return length * 正規化補間ベクトル;
}
