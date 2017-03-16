#pragma once
#include "ObjParser.h"


//template<typename val>
//struct v2
//{
//	union
//	{
//		struct {
//			
//			val x;
//			val y;
//		};
//		struct {
//			val width;
//			val height;
//		};
//	};
//};
//typedef v2<int> vec2i;
//typedef v2<float> vec2f;
//
//template<typename val>
//struct v3 {
//	union
//	{
//		struct
//		{
//			val x;
//			val y;
//			val z;
//		};
//		struct
//		{
//			val r;
//			val g;
//			val b;
//		};
//	};
//};
//
//typedef v3<int> vec3i;
//typedef v3<float> vec3f;

template<typename val>
struct v4 {
	union
	{
		struct
		{
			val x;
			val y;
			val z;
			val w;
		};
		struct
		{
			val r;
			val g;
			val b;
			val a;
		};
		struct
		{
			val left;
			val top;
			val right;
			val bottom;
		};
		struct
		{
			v2<val> min_pt_2d;
			v2<val> max_pt_2d;
		};
	};
};
typedef v4<unsigned int> vec4u;
typedef v4<int> vec4i;
typedef v4<float> vec4f;


template<typename T>
struct Triangle3D
{
	v3<T> pt0;
	v3<T> pt1;
	v3<T> pt2;
	Triangle3D() = default;
	v3<T> getCenter()
	{

		v3<T> result;
		T x = pt0.x + pt1.x + pt2.x;
		T y = pt0.y + pt1.y + pt2.y;
		T x = pt0.z + pt1.z + pt2.z;
		result.x = x / 3;
		result.y = y / 3;
		result.z = z / 3;
		return result;


	}
};

template <typename T>
struct AABB_3D
{
	v3<T> min_pt;
	v3<T> max_pt;
	v3<T> center;
	v3<T> getDim()
	{
		v3<T> r;
		r.x = max_pt.x - min_pt.x;
		r.y = max_pt.y - min_pt.y;
		r.z = max_pt.z - min_pt.z;
		return r;
	}
};
template<typename Type>
class Node_3D
{
public:
	AABB_3D<float> cell;
	std::vector<Node_3D> children;
public:
	Node_3D()
	{}
	Node_3D(AABB_3D<float> box)
		:cell(box) {}

	void AddNode(Node_3D<Type> node);
	void Spawn();
};
template <class Type> 
inline void Node_3D<Type>::Spawn()
{
	v3<float> dims = cell.getDim();
	float incX = dims.x * 0.25f;
	float incZ = dims.z * 0.25f;
	float incY = dims.y * 0.25f;

	for (int stack = 0; stack < 2; stack++)
	{
		for (int row = 0; row < 2; row++)
		{
			for (int col = 0; col < 2; col++)
			{
				const int index = stack * 2 + row * 2 + col;
				AABB_3D<float> ab;
				int d = 0;
 			}
		}
	}
	

};
template<typename Type>
inline void Node_3D<Type>::AddNode(Node_3D<Type> node)
{

}