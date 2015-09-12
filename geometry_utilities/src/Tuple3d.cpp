#include "Tuple3d.hpp"

namespace geometry_utilities
{
Tuple3d::Tuple3d()
{
	set(0.0, 0.0, 0.0);
}

Tuple3d::Tuple3d(const Tuple3d &tupleToCopy)
{
	this->x = tupleToCopy.x;
	this->y = tupleToCopy.y;
	this->z = tupleToCopy.z;
}

Tuple3d::Tuple3d(const double &x, const double &y, const double &z)
{
	set(x, y, z);
}

Tuple3d::Tuple3d(double array[3])
{
	set(array[0], array[1], array[2]);
}

Tuple3d::Tuple3d(std::vector<double> vector)
{
	if (vector.size() != 3)
	{
		throw std::runtime_error("Vector size is not equal to 3!");
	}

	set(vector[0], vector[1], vector[2]);
}

void Tuple3d::set(const double &x, const double &y, const double &z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Tuple3d::set(std::vector<double> vector)
{
	set(vector[0], vector[1], vector[2]);
}

void Tuple3d::set(double array[3])
{
	set(array[0], array[1], array[2]);
}

void Tuple3d::add(const Tuple3d &tuple)
{
	this->x += tuple.x;
	this->y += tuple.y;
	this->z += tuple.z;
}

void Tuple3d::add(const double &x, const double &y, const double &z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void Tuple3d::add(const Tuple3d &tuple1, const Tuple3d &tuple2)
{
	this->x = tuple1.x + tuple2.x;
	this->y = tuple1.y + tuple2.y;
	this->z = tuple1.z + tuple2.z;
}

void Tuple3d::subtract(const Tuple3d &tuple)
{
	this->x -= tuple.x;
	this->y -= tuple.y;
	this->z -= tuple.z;
}

void Tuple3d::subtract(const double &x, const double &y, const double &z)
{
	this->x -= x;
	this->y -= y;
	this->z -= z;
}

void Tuple3d::subtract(const Tuple3d &tuple1, const Tuple3d &tuple2)
{
	this->x = tuple1.x - tuple2.x;
	this->y = tuple1.y - tuple2.y;
	this->z = tuple1.z - tuple2.z;
}

void Tuple3d::negate()
{
	this->x *= -1;
	this->y *= -1;
	this->z *= -1;
}

void Tuple3d::negate(const Tuple3d &tuple)
{
	set(tuple.x, tuple.y, tuple.z);
	negate();
}

void Tuple3d::scale(const double &value)
{
	this->x *= value;
	this->y *= value;
	this->z *= value;
}

void Tuple3d::scale(const double &value, const Tuple3d &tuple)
{
	set(tuple.x, tuple.y, tuple.z);
	scale(value);
}

void Tuple3d::scaleAdd(const double &value, const Tuple3d &tuple)
{
	scale(value);
	add(tuple);
}

void Tuple3d::scaleAdd(const double &value, const Tuple3d &tuple1, const Tuple3d &tuple2)
{
	set(tuple1.x, tuple1.y, tuple1.z);
	scale(value);
	add(tuple2);
}
}