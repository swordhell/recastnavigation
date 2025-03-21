//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#define _USE_MATH_DEFINES
#include <string.h>
#include "DebugDraw.h"
#include "DetourMath.h"
#include "DetourNavMesh.h"
#include "mathfu/matrix.h"
#include "mathfu/vector.h"

duDebugDraw::~duDebugDraw()
{
	// Empty
}

unsigned int duDebugDraw::areaToCol(unsigned int area)
{
	if (area == 0)
	{
		// Treat zero area type as default.
		return duRGBA(0, 192, 255, 255);
	}
	else
	{
		return duIntToCol(area, 255);
	}
}

inline int bit(int a, int b)
{
	return (a & (1 << b)) >> b;
}

unsigned int duIntToCol(int i, int a)
{
	int	r = bit(i, 1) + bit(i, 3) * 2 + 1;
	int	g = bit(i, 2) + bit(i, 4) * 2 + 1;
	int	b = bit(i, 0) + bit(i, 5) * 2 + 1;
	return duRGBA(r*63,g*63,b*63,a);
}

void duIntToCol(int i, float* col)
{
	int	r = bit(i, 0) + bit(i, 3) * 2 + 1;
	int	g = bit(i, 1) + bit(i, 4) * 2 + 1;
	int	b = bit(i, 2) + bit(i, 5) * 2 + 1;
	col[0] = 1 - r*63.0f/255.0f;
	col[1] = 1 - g*63.0f/255.0f;
	col[2] = 1 - b*63.0f/255.0f;
}

void duCalcBoxColors(unsigned int* colors, unsigned int colTop, unsigned int colSide)
{
	if (!colors) return;
	
	colors[0] = duMultCol(colTop, 250);
	colors[1] = duMultCol(colSide, 140);
	colors[2] = duMultCol(colSide, 165);
	colors[3] = duMultCol(colSide, 217);
	colors[4] = duMultCol(colSide, 165);
	colors[5] = duMultCol(colSide, 217);
}

// 绘制线条
void duDebugDrawCylinderWire(struct duDebugDraw* dd, float minx, float miny, float minz,
							 float maxx, float maxy, float maxz, unsigned int col, const float lineWidth)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_LINES, lineWidth);
	duAppendCylinderWire(dd, minx,miny,minz, maxx,maxy,maxz, col);
	dd->end();
}

void duDebugDrawBoxWire(struct duDebugDraw* dd, float minx, float miny, float minz,
						float maxx, float maxy, float maxz, unsigned int col, const float lineWidth)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_LINES, lineWidth);
	duAppendBoxWire(dd, minx,miny,minz, maxx,maxy,maxz, col);
	dd->end();
}

void duDebugDrawOBBWire(struct duDebugDraw* dd, float cx, float cy, float cz,
	float extendx, float extendy, float extendz, float yaw, const unsigned int col, const float lineWidth)
{

	if (!dd) return;

	dd->begin(DU_DRAW_LINES, lineWidth);
	duAppendOBBWire(dd, cx, cy, cz, extendx, extendy, extendz, yaw, col);
	dd->end();
}

void duDebugDrawArc(struct duDebugDraw* dd, const float x0, const float y0, const float z0,
					const float x1, const float y1, const float z1, const float h,
					const float as0, const float as1, unsigned int col, const float lineWidth)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_LINES, lineWidth);
	duAppendArc(dd, x0,y0,z0, x1,y1,z1, h, as0, as1, col);
	dd->end();
}

void duDebugDrawArrow(struct duDebugDraw* dd, const float x0, const float y0, const float z0,
					  const float x1, const float y1, const float z1,
					  const float as0, const float as1, unsigned int col, const float lineWidth)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_LINES, lineWidth);
	duAppendArrow(dd, x0,y0,z0, x1,y1,z1, as0, as1, col);
	dd->end();
}

void duDebugDrawCircle(struct duDebugDraw* dd, const float x, const float y, const float z,
					   const float r, unsigned int col, const float lineWidth)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_LINES, lineWidth);
	duAppendCircle(dd, x,y,z, r, col);
	dd->end();
}

void duDebugDrawCross(struct duDebugDraw* dd, const float x, const float y, const float z,
					  const float size, unsigned int col, const float lineWidth)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_LINES, lineWidth);
	duAppendCross(dd, x,y,z, size, col);
	dd->end();
}

// 输入参数其实就是AABB的两个顶点位置，fcol是给定颜色
void duDebugDrawBox(struct duDebugDraw* dd, float minx, float miny, float minz,
					float maxx, float maxy, float maxz, const unsigned int* fcol)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_QUADS);
	duAppendBox(dd, minx,miny,minz, maxx,maxy,maxz, fcol);
	dd->end();
}


// 绘制OBB立方体
void duDebugDrawOBB(struct duDebugDraw* dd, float cx, float cy, float cz,
	float extendx, float extendy, float extendz, float yaw, const unsigned int* fcol)
{
	if (!dd) return;

	dd->begin(DU_DRAW_TRIS);
	duAppendOBB(dd, cx, cy, cz, extendx, extendy, extendz, yaw, fcol);
	dd->end();
}


// 绘制圆柱体，是按照三角形来绘制
void duDebugDrawCylinder(struct duDebugDraw* dd, float minx, float miny, float minz,
						 float maxx, float maxy, float maxz, unsigned int col)
{
	if (!dd) return;
	
	dd->begin(DU_DRAW_TRIS);
	duAppendCylinder(dd, minx,miny,minz, maxx,maxy,maxz, col);
	dd->end();
}

void duDebugDrawGridXZ(struct duDebugDraw* dd, const float ox, const float oy, const float oz,
					   const int w, const int h, const float size,
					   const unsigned int col, const float lineWidth)
{
	if (!dd) return;

	dd->begin(DU_DRAW_LINES, lineWidth);
	for (int i = 0; i <= h; ++i)
	{
		dd->vertex(ox,oy,oz+i*size, col);
		dd->vertex(ox+w*size,oy,oz+i*size, col);
	}
	for (int i = 0; i <= w; ++i)
	{
		dd->vertex(ox+i*size,oy,oz, col);
		dd->vertex(ox+i*size,oy,oz+h*size, col);
	}
	dd->end();
}
		 
// 画线条
void duAppendCylinderWire(struct duDebugDraw* dd, float minx, float miny, float minz,
						  float maxx, float maxy, float maxz, unsigned int col)
{
	if (!dd) return;

	// 等分16分，计算出对应的标准化后的向量
	static const int NUM_SEG = 16;
	static float dir[NUM_SEG*2];
	static bool init = false;
	if (!init)
	{
		init = true;
		for (int i = 0; i < NUM_SEG; ++i)
		{
			const float a = (float)i/(float)NUM_SEG*DU_PI*2;
			dir[i*2] = dtMathCosf(a);
			dir[i*2+1] = dtMathSinf(a);
		}
	}
	
	const float cx = (maxx + minx)/2; // 算出中心点位置
	const float cz = (maxz + minz)/2;
	const float rx = (maxx - minx)/2; // x,z的半径
	const float rz = (maxz - minz)/2; 
	
	// 循环画圆面
	for (int i = 0, j = NUM_SEG-1; i < NUM_SEG; j = i++)
	{
		// 画底部圆，两点连成线
		dd->vertex(cx+dir[j*2+0]*rx, miny, cz+dir[j*2+1]*rz, col);
		dd->vertex(cx+dir[i*2+0]*rx, miny, cz+dir[i*2+1]*rz, col);
		// 画顶部圆，两点连成线
		dd->vertex(cx+dir[j*2+0]*rx, maxy, cz+dir[j*2+1]*rz, col);
		dd->vertex(cx+dir[i*2+0]*rx, maxy, cz+dir[i*2+1]*rz, col);
	}
	// 绘制圆柱体4个轴线方向的垂直圆通黑线
	for (int i = 0; i < NUM_SEG; i += NUM_SEG/4)
	{
		dd->vertex(cx+dir[i*2+0]*rx, miny, cz+dir[i*2+1]*rz, col);
		dd->vertex(cx+dir[i*2+0]*rx, maxy, cz+dir[i*2+1]*rz, col);
	}
}

void duAppendBoxWire(struct duDebugDraw* dd, float minx, float miny, float minz,
					 float maxx, float maxy, float maxz, unsigned int col)
{
	if (!dd) return;
	// Top
	dd->vertex(minx, miny, minz, col);
	dd->vertex(maxx, miny, minz, col);
	dd->vertex(maxx, miny, minz, col);
	dd->vertex(maxx, miny, maxz, col);
	dd->vertex(maxx, miny, maxz, col);
	dd->vertex(minx, miny, maxz, col);
	dd->vertex(minx, miny, maxz, col);
	dd->vertex(minx, miny, minz, col);
	
	// bottom
	dd->vertex(minx, maxy, minz, col);
	dd->vertex(maxx, maxy, minz, col);
	dd->vertex(maxx, maxy, minz, col);
	dd->vertex(maxx, maxy, maxz, col);
	dd->vertex(maxx, maxy, maxz, col);
	dd->vertex(minx, maxy, maxz, col);
	dd->vertex(minx, maxy, maxz, col);
	dd->vertex(minx, maxy, minz, col);
	
	// Sides
	dd->vertex(minx, miny, minz, col);
	dd->vertex(minx, maxy, minz, col);
	dd->vertex(maxx, miny, minz, col);
	dd->vertex(maxx, maxy, minz, col);
	dd->vertex(maxx, miny, maxz, col);
	dd->vertex(maxx, maxy, maxz, col);
	dd->vertex(minx, miny, maxz, col);
	dd->vertex(minx, maxy, maxz, col);
}

// 画出OBB棱角
void duAppendOBBWire(struct duDebugDraw* dd, float cx, float cy, float cz,
	float extendx, float extendy, float extendz, float yaw,
	unsigned int col)
{
	if (!dd) return;

	mathfu::Vector<float, 3> centerPos(cx, cy, cz);

	// 找到AABB点
	mathfu::Vector<float, 3> p0(-extendx, -extendy, -extendz);
	mathfu::Vector<float, 3> p1(extendx, -extendy, -extendz);
	mathfu::Vector<float, 3> p2(extendx, -extendy, extendz);
	mathfu::Vector<float, 3> p3(-extendx, -extendy, extendz);

	mathfu::Vector<float, 3> p4(-extendx, extendy, -extendz);
	mathfu::Vector<float, 3> p5(extendx, extendy, -extendz);
	mathfu::Vector<float, 3> p6(extendx, extendy, extendz);
	mathfu::Vector<float, 3> p7(-extendx, extendy, extendz);

	// 通过矩阵将点yaw。
	mathfu::Matrix<float, 3> rotation_around_y(mathfu::Matrix<float, 3>::RotationY(-yaw));
	p0 = p0 * rotation_around_y + centerPos;
	p1 = p1 * rotation_around_y + centerPos;
	p2 = p2 * rotation_around_y + centerPos;
	p3 = p3 * rotation_around_y + centerPos;
	p4 = p4 * rotation_around_y + centerPos;
	p5 = p5 * rotation_around_y + centerPos;
	p6 = p6 * rotation_around_y + centerPos;
	p7 = p7 * rotation_around_y + centerPos;


	const float verts[8 * 3] =
	{
		// 正面4个顶点，按照逆时针顺序
		p0.x, p0.y, p0.z, // 0
		p1.x, p1.y, p1.z, // 1
		p2.x, p2.y, p2.z, // 2
		p3.x, p3.y, p3.z, // 3

		// 背面4个顶点
		p4.x, p4.y, p4.z,
		p5.x, p5.y, p5.z,
		p6.x, p6.y, p6.z,
		p7.x, p7.y, p7.z,
	};

	// Top
	dd->vertex(p3.x, p3.y, p3.z, col);
	dd->vertex(p2.x, p2.y, p2.z, col);
	dd->vertex(p2.x, p2.y, p2.z, col);
	dd->vertex(p6.x, p6.y, p6.z, col);
	dd->vertex(p6.x, p6.y, p6.z, col);
	dd->vertex(p7.x, p7.y, p7.z, col);
	dd->vertex(p7.x, p7.y, p7.z, col);
	dd->vertex(p3.x, p3.y, p3.z, col);

	// bottom
	dd->vertex(p0.x, p0.y, p0.z, col);
	dd->vertex(p1.x, p1.y, p1.z, col);
	dd->vertex(p1.x, p1.y, p1.z, col);
	dd->vertex(p5.x, p5.y, p5.z, col);
	dd->vertex(p5.x, p5.y, p5.z, col);
	dd->vertex(p4.x, p4.y, p4.z, col);
	dd->vertex(p4.x, p4.y, p4.z, col);
	dd->vertex(p0.x, p0.y, p0.z, col);

	// Sides
	dd->vertex(p0.x, p0.y, p0.z, col);
	dd->vertex(p3.x, p3.y, p3.z, col);
	dd->vertex(p1.x, p1.y, p1.z, col);
	dd->vertex(p2.x, p2.y, p2.z, col);
	dd->vertex(p5.x, p5.y, p5.z, col);
	dd->vertex(p6.x, p6.y, p6.z, col);
	dd->vertex(p4.x, p4.y, p4.z, col);
	dd->vertex(p7.x, p7.y, p7.z, col);
}

void duAppendBoxPoints(struct duDebugDraw* dd, float minx, float miny, float minz,
					   float maxx, float maxy, float maxz, unsigned int col)
{
	if (!dd) return;
	// Top
	dd->vertex(minx, miny, minz, col);
	dd->vertex(maxx, miny, minz, col);
	dd->vertex(maxx, miny, minz, col);
	dd->vertex(maxx, miny, maxz, col);
	dd->vertex(maxx, miny, maxz, col);
	dd->vertex(minx, miny, maxz, col);
	dd->vertex(minx, miny, maxz, col);
	dd->vertex(minx, miny, minz, col);
	
	// bottom
	dd->vertex(minx, maxy, minz, col);
	dd->vertex(maxx, maxy, minz, col);
	dd->vertex(maxx, maxy, minz, col);
	dd->vertex(maxx, maxy, maxz, col);
	dd->vertex(maxx, maxy, maxz, col);
	dd->vertex(minx, maxy, maxz, col);
	dd->vertex(minx, maxy, maxz, col);
	dd->vertex(minx, maxy, minz, col);
}

// 绘制AABB方式的Box
void duAppendBox(struct duDebugDraw* dd, float minx, float miny, float minz,
				 float maxx, float maxy, float maxz, const unsigned int* fcol)
{
	if (!dd) return;
	const float verts[8*3] =
	{
		// 正面4个顶点，按照逆时针顺序
		minx, miny, minz, // 0
		maxx, miny, minz, // 1
		maxx, miny, maxz, // 2
		minx, miny, maxz, // 3

		// 背面4个顶点
		minx, maxy, minz,
		maxx, maxy, minz,
		maxx, maxy, maxz,
		minx, maxy, maxz,
	};

	static const unsigned char inds[6*4] =
	{
		// 每一句话都是从立方体上去一个点，而且刚好是6面体的某个面
		7, 6, 5, 4,
		0, 1, 2, 3,
		1, 5, 6, 2,
		3, 7, 4, 0,
		2, 6, 7, 3,
		0, 4, 5, 1,
	};
	
	// 一共能支持读取6种颜色
	const unsigned char* in = inds;
	for (int i = 0; i < 6; ++i)
	{
		// 每一句话都是从立方体上去一个点，而且刚好是6面体的某个面
		dd->vertex(&verts[*in*3], fcol[i]); in++;
		dd->vertex(&verts[*in*3], fcol[i]); in++;
		dd->vertex(&verts[*in*3], fcol[i]); in++;
		dd->vertex(&verts[*in*3], fcol[i]); in++;
	}
}

// 绘制圆柱体
void duAppendCylinder(struct duDebugDraw* dd, float minx, float miny, float minz,
					  float maxx, float maxy, float maxz, unsigned int col)
{
	if (!dd) return;
	
	// 等分16分
	static const int NUM_SEG = 16;
	static float dir[NUM_SEG*2];
	static bool init = false;
	if (!init)
	{
		init = true;
		for (int i = 0; i < NUM_SEG; ++i)
		{
			const float a = (float)i/(float)NUM_SEG*DU_PI*2;
			dir[i*2] = cosf(a);
			dir[i*2+1] = sinf(a);
		}
	}
	
	unsigned int col2 = duMultCol(col, 160);
	
	const float cx = (maxx + minx)/2; // 计算中心位置
	const float cz = (maxz + minz)/2;
	const float rx = (maxx - minx)/2; // 计算x,z的半径
	const float rz = (maxz - minz)/2;

	// 绘制底部圆，固定第一个点，然后开始在圆面上按照逆时针找圆周上的邻近两个点，连成三角形，最终构成圆面
	for (int i = 2; i < NUM_SEG-2; ++i)
	{
		const int a = 0, b = i-1, c = i;
		dd->vertex(cx+dir[a*2+0]*rx, miny, cz+dir[a*2+1]*rz, col2);
		dd->vertex(cx+dir[b*2+0]*rx, miny, cz+dir[b*2+1]*rz, col2);
		dd->vertex(cx+dir[c*2+0]*rx, miny, cz+dir[c*2+1]*rz, col2);
	}
	// 绘制顶部圆面
	for (int i = 2; i < NUM_SEG; ++i)
	{
		const int a = 0, b = i, c = i-1;
		dd->vertex(cx+dir[a*2+0]*rx, maxy, cz+dir[a*2+1]*rz, col);
		dd->vertex(cx+dir[b*2+0]*rx, maxy, cz+dir[b*2+1]*rz, col);
		dd->vertex(cx+dir[c*2+0]*rx, maxy, cz+dir[c*2+1]*rz, col);
	}
	// 绘制圆筒，用两个三角形平凑出一个矩形出来
	for (int i = 0, j = NUM_SEG-1; i < NUM_SEG; j = i++)
	{
		dd->vertex(cx+dir[i*2+0]*rx, miny, cz+dir[i*2+1]*rz, col2);
		dd->vertex(cx+dir[j*2+0]*rx, miny, cz+dir[j*2+1]*rz, col2);
		dd->vertex(cx+dir[j*2+0]*rx, maxy, cz+dir[j*2+1]*rz, col);

		dd->vertex(cx+dir[i*2+0]*rx, miny, cz+dir[i*2+1]*rz, col2);
		dd->vertex(cx+dir[j*2+0]*rx, maxy, cz+dir[j*2+1]*rz, col);
		dd->vertex(cx+dir[i*2+0]*rx, maxy, cz+dir[i*2+1]*rz, col);
	}
}

// 绘制OBB
void duAppendOBB(struct duDebugDraw* dd, float cx, float cy, float cz,
	float extendx, float extendy, float extendz, float yaw, const unsigned int *fcol)
{
	if (!dd) return;

	mathfu::Vector<float, 3> centerPos(cx, cy, cz);

	// 找到AABB点
	mathfu::Vector<float, 3> p0(-extendx,-extendy, -extendz);
	mathfu::Vector<float, 3> p1(extendx, -extendy, -extendz);
	mathfu::Vector<float, 3> p2(extendx, -extendy, extendz);
	mathfu::Vector<float, 3> p3(-extendx, -extendy, extendz);

	mathfu::Vector<float, 3> p4(-extendx, extendy, -extendz);
	mathfu::Vector<float, 3> p5(extendx, extendy, -extendz);
	mathfu::Vector<float, 3> p6(extendx, extendy, extendz);
	mathfu::Vector<float, 3> p7(-extendx, extendy, extendz);

	// 通过矩阵将点yaw。
	mathfu::Matrix<float, 3> rotation_around_y(mathfu::Matrix<float,3>::RotationY(-yaw));
	p0 = p0 * rotation_around_y + centerPos;
	p1 = p1 * rotation_around_y + centerPos;
	p2 = p2 * rotation_around_y + centerPos;
	p3 = p3 * rotation_around_y + centerPos;
	p4 = p4 * rotation_around_y + centerPos;
	p5 = p5 * rotation_around_y + centerPos;
	p6 = p6 * rotation_around_y + centerPos;
	p7 = p7 * rotation_around_y + centerPos;

	const float verts[8 * 3] =
	{
		// 正面4个顶点，按照逆时针顺序
		p0.x, p0.y, p0.z, // 0
		p1.x, p1.y, p1.z, // 1
		p2.x, p2.y, p2.z, // 2
		p3.x, p3.y, p3.z, // 3

		// 背面4个顶点
		p4.x, p4.y, p4.z,
		p5.x, p5.y, p5.z,
		p6.x, p6.y, p6.z,
		p7.x, p7.y, p7.z,
	};

	static const unsigned char inds[12 * 3] =
	{
		// 将立方体的每个面都使用两个三角形
		0,1,2,
		2,3,0,
		5,4,6,
		4,7,6,
		1,5,6,
		6,2,1,
		0,3,7,
		7,4,0,
		3,2,6,
		6,7,3,
		5,1,0,
		4,5,0,
	};

	// 一共能支持读取6种颜色
	const unsigned char* in = inds;
	for (int i = 0; i < 12; ++i)
	{
		auto colidx = i / 4;
		// 每一句话都是从立方体上去一个点，而且刚好是6面体的某个面
		dd->vertex(verts[inds[i * 3] * 3], verts[inds[i * 3] * 3 + 1], verts[inds[i * 3] * 3 + 2], fcol[colidx]);
		dd->vertex(verts[inds[i * 3 + 1] * 3], verts[inds[i * 3 + 1] * 3 + 1], verts[inds[i * 3 + 1] * 3 + 2], fcol[colidx]);
		dd->vertex(verts[inds[i * 3 + 2] * 3], verts[inds[i * 3 + 2] * 3 + 1], verts[inds[i * 3 + 2] * 3 + 2], fcol[colidx]);
	}
}


inline void evalArc(const float x0, const float y0, const float z0,
					const float dx, const float dy, const float dz,
					const float h, const float u, float* res)
{
	res[0] = x0 + dx * u;
	res[1] = y0 + dy * u + h * (1-(u*2-1)*(u*2-1));
	res[2] = z0 + dz * u;
}


inline void vcross(float* dest, const float* v1, const float* v2)
{
	dest[0] = v1[1]*v2[2] - v1[2]*v2[1];
	dest[1] = v1[2]*v2[0] - v1[0]*v2[2];
	dest[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
}

inline void vnormalize(float* v)
{
	float d = 1.0f / sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] *= d;
	v[1] *= d;
	v[2] *= d;
}

inline void vsub(float* dest, const float* v1, const float* v2)
{
	dest[0] = v1[0]-v2[0];
	dest[1] = v1[1]-v2[1];
	dest[2] = v1[2]-v2[2];
}

inline float vdistSqr(const float* v1, const float* v2)
{
	const float x = v1[0]-v2[0];
	const float y = v1[1]-v2[1];
	const float z = v1[2]-v2[2];
	return x*x + y*y + z*z;
}


void appendArrowHead(struct duDebugDraw* dd, const float* p, const float* q,
					 const float s, unsigned int col)
{
	const float eps = 0.001f;
	if (!dd) return;
	if (vdistSqr(p,q) < eps*eps) return;
	float ax[3], ay[3] = {0,1,0}, az[3];
	vsub(az, q, p);
	vnormalize(az);
	vcross(ax, ay, az);
	vcross(ay, az, ax);
	vnormalize(ay);

	dd->vertex(p, col);
//	dd->vertex(p[0]+az[0]*s+ay[0]*s/2, p[1]+az[1]*s+ay[1]*s/2, p[2]+az[2]*s+ay[2]*s/2, col);
	dd->vertex(p[0]+az[0]*s+ax[0]*s/3, p[1]+az[1]*s+ax[1]*s/3, p[2]+az[2]*s+ax[2]*s/3, col);

	dd->vertex(p, col);
//	dd->vertex(p[0]+az[0]*s-ay[0]*s/2, p[1]+az[1]*s-ay[1]*s/2, p[2]+az[2]*s-ay[2]*s/2, col);
	dd->vertex(p[0]+az[0]*s-ax[0]*s/3, p[1]+az[1]*s-ax[1]*s/3, p[2]+az[2]*s-ax[2]*s/3, col);
	
}

void duAppendArc(struct duDebugDraw* dd, const float x0, const float y0, const float z0,
				 const float x1, const float y1, const float z1, const float h,
				 const float as0, const float as1, unsigned int col)
{
	if (!dd) return;
	static const int NUM_ARC_PTS = 8;
	static const float PAD = 0.05f;
	static const float ARC_PTS_SCALE = (1.0f-PAD*2) / (float)NUM_ARC_PTS;
	const float dx = x1 - x0;
	const float dy = y1 - y0;
	const float dz = z1 - z0;
	const float len = sqrtf(dx*dx + dy*dy + dz*dz);
	float prev[3];
	evalArc(x0,y0,z0, dx,dy,dz, len*h, PAD, prev);
	for (int i = 1; i <= NUM_ARC_PTS; ++i)
	{
		const float u = PAD + i * ARC_PTS_SCALE;
		float pt[3];
		evalArc(x0,y0,z0, dx,dy,dz, len*h, u, pt);
		dd->vertex(prev[0],prev[1],prev[2], col);
		dd->vertex(pt[0],pt[1],pt[2], col);
		prev[0] = pt[0]; prev[1] = pt[1]; prev[2] = pt[2];
	}
	
	// End arrows
	if (as0 > 0.001f)
	{
		float p[3], q[3];
		evalArc(x0,y0,z0, dx,dy,dz, len*h, PAD, p);
		evalArc(x0,y0,z0, dx,dy,dz, len*h, PAD+0.05f, q);
		appendArrowHead(dd, p, q, as0, col);
	}

	if (as1 > 0.001f)
	{
		float p[3], q[3];
		evalArc(x0,y0,z0, dx,dy,dz, len*h, 1-PAD, p);
		evalArc(x0,y0,z0, dx,dy,dz, len*h, 1-(PAD+0.05f), q);
		appendArrowHead(dd, p, q, as1, col);
	}
}

void duAppendArrow(struct duDebugDraw* dd, const float x0, const float y0, const float z0,
				   const float x1, const float y1, const float z1,
				   const float as0, const float as1, unsigned int col)
{
	if (!dd) return;

	dd->vertex(x0,y0,z0, col);
	dd->vertex(x1,y1,z1, col);
	
	// End arrows
	const float p[3] = {x0,y0,z0}, q[3] = {x1,y1,z1};
	if (as0 > 0.001f)
		appendArrowHead(dd, p, q, as0, col);
	if (as1 > 0.001f)
		appendArrowHead(dd, q, p, as1, col);
}

void duAppendCircle(struct duDebugDraw* dd, const float x, const float y, const float z,
					const float r, unsigned int col)
{
	if (!dd) return;
	static const int NUM_SEG = 40;
	static float dir[40*2];
	static bool init = false;
	if (!init)
	{
		init = true;
		for (int i = 0; i < NUM_SEG; ++i)
		{
			const float a = (float)i/(float)NUM_SEG*DU_PI*2;
			dir[i*2] = cosf(a);
			dir[i*2+1] = sinf(a);
		}
	}
	
	for (int i = 0, j = NUM_SEG-1; i < NUM_SEG; j = i++)
	{
		dd->vertex(x+dir[j*2+0]*r, y, z+dir[j*2+1]*r, col);
		dd->vertex(x+dir[i*2+0]*r, y, z+dir[i*2+1]*r, col);
	}
}

void duAppendCross(struct duDebugDraw* dd, const float x, const float y, const float z,
				   const float s, unsigned int col)
{
	if (!dd) return;
	dd->vertex(x-s,y,z, col);
	dd->vertex(x+s,y,z, col);
	dd->vertex(x,y-s,z, col);
	dd->vertex(x,y+s,z, col);
	dd->vertex(x,y,z-s, col);
	dd->vertex(x,y,z+s, col);
}

duDisplayList::duDisplayList(int cap) :
	m_pos(0),
	m_color(0),
	m_size(0),
	m_cap(0),
	m_depthMask(true),
	m_prim(DU_DRAW_LINES),
	m_primSize(1.0f)
{
	if (cap < 8)
		cap = 8;
	resize(cap);
}

duDisplayList::~duDisplayList()
{
	delete [] m_pos;
	delete [] m_color;
}

void duDisplayList::resize(int cap)
{
	float* newPos = new float[cap*3];
	if (m_size)
		memcpy(newPos, m_pos, sizeof(float)*3*m_size);
	delete [] m_pos;
	m_pos = newPos;

	unsigned int* newColor = new unsigned int[cap];
	if (m_size)
		memcpy(newColor, m_color, sizeof(unsigned int)*m_size);
	delete [] m_color;
	m_color = newColor;
	
	m_cap = cap;
}

void duDisplayList::clear()
{
	m_size = 0;
}

void duDisplayList::depthMask(bool state)
{
	m_depthMask = state;
}

void duDisplayList::begin(duDebugDrawPrimitives prim, float size)
{
	clear();
	m_prim = prim;
	m_primSize = size;
}

void duDisplayList::vertex(const float x, const float y, const float z, unsigned int color)
{
	if (m_size+1 >= m_cap)
		resize(m_cap*2);
	float* p = &m_pos[m_size*3];
	p[0] = x;
	p[1] = y;
	p[2] = z;
	m_color[m_size] = color;
	m_size++;
}

void duDisplayList::vertex(const float* pos, unsigned int color)
{
	vertex(pos[0],pos[1],pos[2],color);
}

void duDisplayList::end()
{
}

void duDisplayList::draw(struct duDebugDraw* dd)
{
	if (!dd) return;
	if (!m_size) return;
	dd->depthMask(m_depthMask);
	dd->begin(m_prim, m_primSize);
	for (int i = 0; i < m_size; ++i)
		dd->vertex(&m_pos[i*3], m_color[i]);
	dd->end();
}
