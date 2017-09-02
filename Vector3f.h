#pragma once
#include<stdio.h>
#include<math.h>

namespace MathUtility{
	struct Vector3f
	{
	public:
		union {
			struct { float X, Y, Z; };
			struct { float R, G, B; };
			float Array[3];
		};
	public:
		enum UpDirection
		{
			X_UP,
			Y_UP,
			Z_UP
		};
	public:
		Vector3f() : X(0.0), Y(0.0), Z(0.0) {}
		Vector3f(const float _x, const float _y, const float _z) :X(_x), Y(_y), Z(_z) {}
		Vector3f(const Vector3f& v) : X(v.X), Y(v.Y), Z(v.Z) {}

		// 単項演算子
		inline Vector3f operator - () const
		{
			return Vector3f(-this->X, -this->Y, -this->Z);
		}
		inline float&  operator[](int i)
		{
			return this->Array[i];
		}
		inline float operator[](int i) const
		{
			return this->Array[i];
		}
		inline Vector3f& operator +=(const Vector3f& v)
		{
			this->X += v.X;
			this->Y += v.Y;
			this->Z += v.Z;
			return *this;
		}
		inline Vector3f& operator -=(const Vector3f& v)
		{
			this->X -= v.X;
			this->Y -= v.Y;
			this->Z -= v.Z;
			return *this;
		}
		// 2項演算子
		inline friend const Vector3f operator+(const Vector3f& left, const Vector3f& right)
		{
			return Vector3f(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
		}
		inline friend const Vector3f operator-(const Vector3f& left, const Vector3f& right)
		{
			return Vector3f(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
		}
		inline friend const Vector3f operator*(const float left, const Vector3f& right)
		{
			return Vector3f(left * right.X, left * right.Y, left * right.Z);
		}
		inline friend const Vector3f operator*(const Vector3f& left, const Vector3f& right)
		{
			return Vector3f(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
		}
		inline friend const Vector3f operator/(const Vector3f& left, const Vector3f& right)
		{
			return Vector3f(left.X / right.X, left.Y / right.Y, left.Z / right.Z);
		}
		inline void Normalize(void)
		{
			float len = Vector3f::Length(*this);
			this->X = this->X / len;
			this->Y = this->Y / len;
			this->Z = this->Z / len;
		}

		inline static float Dot(const Vector3f& left, const Vector3f& right)
		{
			return left.X*right.X + left.Y*right.Y + left.Z*right.Z;
		}
		inline static Vector3f Cross(const Vector3f& left, const Vector3f& right)
		{
			return Vector3f(
				left.Y*right.Z - left.Z*right.Y,
				left.Z*right.X - left.X*right.Z,
				left.X*right.Y - left.Y*right.X);
		}
		inline static Vector3f Mult(const Vector3f& left, const Vector3f& right)
		{
			return Vector3f(left.X*right.X, left.Y*right.Y, left.Z*right.Z);
		}

		inline static float LengthSq(const Vector3f& v)
		{
			return Vector3f::Dot(v, v);
		}
		inline static float Length(const Vector3f& v)
		{
			return sqrt(Vector3f::LengthSq(v));
		}
		inline static Vector3f Normalize(const Vector3f& v)
		{
			Vector3f n = v;
			n.Normalize();

			return Vector3f(n);
		}
		// 極座標から変換
		inline static Vector3f FromSpherical(const float r, const float theta, const float phi)
		{
			return r * Vector3f(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
		}
		// 極座標から変換
		inline static Vector3f FromSpherical(const float r, const float theta, const float phi, const Vector3f::UpDirection up)
		{
			/* Z-up */
			Vector3f d = Vector3f::FromSpherical(r, theta, phi);
			if (up == Vector3f::Z_UP) return d;
			else if (up == Vector3f::Y_UP) return Vector3f(d.Y, d.Z, d.X);
			else if (up == Vector3f::X_UP) return Vector3f(d.Z, d.X, d.Y);
		}
		// 極座標へ変換
		inline static void ToSpherical(const Vector3f& v, float& theta, float& phi)
		{
			Vector3f unit = Vector3f::Normalize(v);
			theta = acos(unit.Z);
			phi = atan2(unit.Y, unit.Z);
		}
		// ベクトルを進行方向にtheta, 軸周りにphi回転させる関数
		inline static Vector3f Rotate(const Vector3f& source, const float theta, const float phi)
		{
			Vector3f e1 = source;
			Vector3f e2, e3;

			if (e1.Y == 0.0 && e1.Z == 0.0)
			{
				e2[1] = e1[0] >= 0.0 ? 1.0 : -1.0;
				e2[0] = e2[2] = 0.0;
			}
			else
			{
				e2[0] = 0.0;
				e2[1] = e1[2];
				e2[2] = -e1[1];
				e2.Normalize();
			}

			// make e3
			e3 = Vector3f::Cross(e1, e2);


			Vector3f e = (float)cos(phi) * e2 + (float)sin(phi) * e3;

			Vector3f vec = (float)cos(theta) * source + (float)sin(theta) * e;

			return vec;
		}
		inline static void MakeBiNormalTangent(const Vector3f&n, Vector3f& t, Vector3f& b)
		{
			Vector3f e1 = n;
			Vector3f e2, e3;

			if (e1.Y == 0.0 && e1.Z == 0.0)
			{
				e2[1] = e1[0] >= 0.0 ? 1.0 : -1.0;
				e2[0] = e2[2] = 0.0;
			}
			else
			{
				e2[0] = 0.0;
				e2[1] = e1[2];
				e2[2] = -e1[1];
				e2.Normalize();
			}

			// make e3
			e3 = Vector3f::Cross(e1, e2);

			t = e2;
			b = e3;
		}
		inline static Vector3f Reflect(const Vector3f& v, const Vector3f& n)
		{
			return Vector3f::Normalize(2.0 * Vector3f::Dot(v, n)*n - v);
		}
		//Debug
		inline void Dump() const
		{
			printf("(%g, %g, %g)\n", this->X, this->Y, this->Z);
		}
		bool IsNaN() const
		{
			if (isnan(this->X) || isnan(this->Y) || isnan(this->Z)) return true;
			else return false;
		}
		float Max() const
		{
			return fmax(this->X, fmax(this->Y, this->Z));
		}
		float Min() const
		{
			return fmin(this->X, fmin(this->Y, this->Z));
		}
	};
};