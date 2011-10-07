#pragma once

#include "math.h"

namespace remath {

//! Axis aligned bounding box in 3d dimensional space.
/** Has some useful methods used with occlusion culling or clipping.
*/
template <class T>
class aabbox3d
{
public:

	//! Default Constructor.
	aabbox3d(): MinEdge(-1,-1,-1), MaxEdge(1,1,1) {}
	//! Constructor with min edge and max edge.
	aabbox3d(const Vector3<T>& min, const Vector3<T>& max): MinEdge(min), MaxEdge(max) {}
	//! Constructor with only one point.
	aabbox3d(const Vector3<T>& init): MinEdge(init), MaxEdge(init) {}
	//! Constructor with min edge and max edge as single values, not vectors.
	aabbox3d(T minx, T miny, T minz, T maxx, T maxy, T maxz): MinEdge(minx, miny, minz), MaxEdge(maxx, maxy, maxz) {}

	// operators
	//! Equality operator
	/** \param other box to compare with.
  \return True if both boxes are equal, else false. */
	inline bool operator==(const aabbox3d<T>& other) const { return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}
	//! Inequality operator
	/** \param other box to compare with.
  \return True if both boxes are different, else false. */
	inline bool operator!=(const aabbox3d<T>& other) const { return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}

	// functions

	//! Resets the bounding box to a one-point box.
	/** \param x X coord of the point.
  \param y Y coord of the point.
  \param z Z coord of the point. */
	void reset(T x, T y, T z)
	{
		MaxEdge.set(x,y,z);
		MinEdge = MaxEdge;
	}

	//! Resets the bounding box.
	/** \param initValue New box to set this one to. */
	void reset(const aabbox3d<T>& initValue)
	{
		*this = initValue;
	}

	//! Resets the bounding box to a one-point box.
	/** \param initValue New point. */
	void reset(const Vector3<T>& initValue)
	{
		MaxEdge = initValue;
		MinEdge = initValue;
	}

	//! Adds a point to the bounding box
	/** The box grows bigger, if point was outside of the box.
  \param p: Point to add into the box. */
	void addInternalPoint(const Vector3<T>& p)
	{
		addInternalPoint(p.x, p.y, p.z);
	}

	//! Adds another bounding box
	/** The box grows bigger, if the new box was outside of the box.
  \param b: Other bounding box to add into this box. */
	void addInternalBox(const aabbox3d<T>& b)
	{
		addInternalPoint(b.MaxEdge);
		addInternalPoint(b.MinEdge);
	}

	//! Adds a point to the bounding box
	/** The box grows bigger, if point is outside of the box.
  \param x X coordinate of the point to add to this box.
  \param y Y coordinate of the point to add to this box.
  \param z Z coordinate of the point to add to this box. */
	void addInternalPoint(T x, T y, T z)
	{
		if (x>MaxEdge.x) MaxEdge.x = x;
		if (y>MaxEdge.y) MaxEdge.y = y;
		if (z>MaxEdge.z) MaxEdge.z = z;

		if (x<MinEdge.x) MinEdge.x = x;
		if (y<MinEdge.y) MinEdge.y = y;
		if (z<MinEdge.z) MinEdge.z = z;
	}

	//! Get center of the bounding box
	/** \return Center of the bounding box. */
	Vector3<T> getCenter() const
	{
		return (MinEdge + MaxEdge) / 2;
	}

	//! Get extent of the box (maximal distance of two points in the box)
	/** \return Extent of the bounding box. */
	Vector3<T> getExtent() const
	{
		return MaxEdge - MinEdge;
	}

	//! Check if the box is empty.
	/** This means that there is no space between the min and max edge.
  \return True if box is empty, else false. */
	bool isEmpty() const
	{
		return MinEdge.equals ( MaxEdge );
	}

	//! Get the volume enclosed by the box in cubed units
	T getVolume() const
	{
		const Vector3<T> e = getExtent();
		return e.x * e.y * e.z;
	}

	//! Get the surface area of the box in squared units
	T getArea() const
	{
		const Vector3<T> e = getExtent();
		return 2*(e.x*e.y + e.x*e.z + e.y*e.z);
	}

	//! Stores all 8 edges of the box into an array
	/** \param edges: Pointer to array of 8 edges. */
	void getEdges(Vector3<T> *edges) const
	{
		const Vector3<T> middle = getCenter();
		const Vector3<T> diag = middle - MaxEdge;

		/*
   Edges are stored in this way:
   Hey, am I an ascii artist, or what? :) niko.
	   /3--------/7
	  / |       / |
	 /  |      /  |
	1---------5   |
	|  /2- - -|- -6
	| /       |  /
	|/        | /
	0---------4/
   */

		edges[0].set(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
		edges[1].set(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
		edges[2].set(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
		edges[3].set(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
		edges[4].set(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
		edges[5].set(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
		edges[6].set(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
		edges[7].set(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
	}

	//! Repairs the box.
	/** Necessary if for example MinEdge and MaxEdge are swapped. */
	void repair()
	{
		T t;

		if (MinEdge.x > MaxEdge.x)
		{ t=MinEdge.x; MinEdge.x = MaxEdge.x; MaxEdge.x=t; }
		if (MinEdge.y > MaxEdge.y)
		{ t=MinEdge.y; MinEdge.y = MaxEdge.y; MaxEdge.y=t; }
		if (MinEdge.z > MaxEdge.z)
		{ t=MinEdge.z; MinEdge.z = MaxEdge.z; MaxEdge.z=t; }
	}

	//! Calculates a new interpolated bounding box.
	/** d=0 returns other, d=1 returns this, all other values blend between
  the two boxes.
  \param other Other box to interpolate between
  \param d Value between 0.0f and 1.0f.
  \return Interpolated box. */
	aabbox3d<T> getInterpolated(const aabbox3d<T>& other, float d) const
	{
		float inv = 1.0f - d;
		return aabbox3d<T>((other.MinEdge*inv) + (MinEdge*d),
						   (other.MaxEdge*inv) + (MaxEdge*d));
	}

	//! Determines if a point is within this box.
	/** Border is included (IS part of the box)!
  \param p: Point to check.
  \return True if the point is within the box and false if not */
	bool isPointInside(const Vector3<T>& p) const
	{
		return (p.x >= MinEdge.x && p.x <= MaxEdge.x &&
				p.y >= MinEdge.y && p.y <= MaxEdge.y &&
				p.z >= MinEdge.z && p.z <= MaxEdge.z);
	}

	//! Determines if a point is within this box and not its borders.
	/** Border is excluded (NOT part of the box)!
  \param p: Point to check.
  \return True if the point is within the box and false if not. */
	bool isPointTotalInside(const Vector3<T>& p) const
	{
		return (p.x > MinEdge.x && p.x < MaxEdge.x &&
				p.y > MinEdge.y && p.y < MaxEdge.y &&
				p.z > MinEdge.z && p.z < MaxEdge.z);
	}

	//! Check if this box is completely inside the 'other' box.
	/** \param other: Other box to check against.
  \return True if this box is completly inside the other box,
  otherwise false. */
	bool isFullInside(const aabbox3d<T>& other) const
	{
		return (MinEdge.x >= other.MinEdge.x && MinEdge.y >= other.MinEdge.y && MinEdge.z >= other.MinEdge.z &&
				MaxEdge.x <= other.MaxEdge.x && MaxEdge.y <= other.MaxEdge.y && MaxEdge.z <= other.MaxEdge.z);
	}

	//! Determines if the axis-aligned box intersects with another axis-aligned box.
	/** \param other: Other box to check a intersection with.
  \return True if there is an intersection with the other box,
  otherwise false. */
	bool intersectsWithBox(const aabbox3d<T>& other) const
	{
		return (MinEdge.x <= other.MaxEdge.x && MinEdge.y <= other.MaxEdge.y && MinEdge.z <= other.MaxEdge.z &&
				MaxEdge.x >= other.MinEdge.x && MaxEdge.y >= other.MinEdge.y && MaxEdge.z >= other.MinEdge.z);
	}
	//TODO
#if 0
	//! Tests if the box intersects with a line
	/** \param line: Line to test intersection with.
  \return True if there is an intersection , else false. */
	bool intersectsWithLine(const line3d<T>& line) const
	{
		return intersectsWithLine(line.getMiddle(), line.getVector().normalize(),
								  (T)(line.getLength() * 0.5));
	}

	//! Tests if the box intersects with a line
	/** \param linemiddle Center of the line.
  \param linevect Vector of the line.
  \param halflength Half length of the line.
  \return True if there is an intersection, else false. */
	bool intersectsWithLine(const Vector3<T>& linemiddle,
							const Vector3<T>& linevect, T halflength) const
	{
		const Vector3<T> e = getExtent() * (T)0.5;
		const Vector3<T> t = getCenter() - linemiddle;

		if ((fabs(t.x) > e.x + halflength * fabs(linevect.x)) ||
			(fabs(t.y) > e.y + halflength * fabs(linevect.y)) ||
			(fabs(t.z) > e.z + halflength * fabs(linevect.z)) )
			return false;

		T r = e.y * (T)fabs(linevect.z) + e.z * (T)fabs(linevect.y);
		if (fabs(t.y*linevect.z - t.z*linevect.y) > r )
			return false;

		r = e.x * (T)fabs(linevect.z) + e.z * (T)fabs(linevect.x);
		if (fabs(t.z*linevect.x - t.x*linevect.z) > r )
			return false;

		r = e.x * (T)fabs(linevect.y) + e.y * (T)fabs(linevect.x);
		if (fabs(t.x*linevect.y - t.y*linevect.x) > r)
			return false;

		return true;
	}

	//! Classifies a relation with a plane.
	/** \param plane Plane to classify relation to.
  \return Returns ISREL3D_FRONT if the box is in front of the plane,
  ISREL3D_BACK if the box is behind the plane, and
  ISREL3D_CLIPPED if it is on both sides of the plane. */
	EIntersectionRelation3D classifyPlaneRelation(const plane3d<T>& plane) const
	{
		Vector3<T> nearPoint(MaxEdge);
		Vector3<T> farPoint(MinEdge);

		if (plane.Normal.x > (T)0)
		{
			nearPoint.x = MinEdge.x;
			farPoint.x = MaxEdge.x;
		}

		if (plane.Normal.y > (T)0)
		{
			nearPoint.y = MinEdge.y;
			farPoint.y = MaxEdge.y;
		}

		if (plane.Normal.z > (T)0)
		{
			nearPoint.z = MinEdge.z;
			farPoint.z = MaxEdge.z;
		}

		if (plane.Normal.dotProduct(nearPoint) + plane.D > (T)0)
			return ISREL3D_FRONT;

		if (plane.Normal.dotProduct(farPoint) + plane.D > (T)0)
			return ISREL3D_CLIPPED;

		return ISREL3D_BACK;
	}
#endif
	//! The near edge
	Vector3<T> MinEdge;

	//! The far edge
	Vector3<T> MaxEdge;
};

//! Typedef for a float 3d bounding box.
typedef aabbox3d<float> aabbox3df;
//! Typedef for an integer 3d bounding box.
typedef aabbox3d<int> aabbox3di;

} // end of namespace 'remath'
