#include "vector.hpp"

#include "point.hpp"

#include <cmath>

#define PI 3.141559265

Vector::Vector() : x(0), y(0), z(0) {}
Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}
Vector::Vector(Point p1, Point p2) : x(p2.getX() - p1.getX()), y(p2.getY() - p1.getY()), z(p2.getZ() - p1.getZ()) {}
Vector::Vector(const Vector& v) : x(v.x), y(v.y), z(v.z) {}

double Vector::getX() { return x; }
double Vector::getY() { return y; }
double Vector::getZ() { return z; }

void Vector::setX(double x) { this->x = x; }
void Vector::setY(double y) { this->y = y; }
void Vector::setZ(double z) { this->z = z; }

double Vector::norme() {
	return sqrt( x*x + y*y + z*z );
}

void Vector::normalize() {
	double n(norme());
	x = x/n;
	y = y/n;
	z = z/n;
}

double Vector::scalar(Vector v) {
	return x*v.x + y*v.y + z*v.z;
}

Vector Vector::vectoriel(Vector v) {
	Vector r;
	r.x = y*v.z - z*v.y;
	r.y = z*v.x - x*v.z;
	r.z = x*v.y - y*v.x;
	return r;
}

double Vector::angle(Vector v) {
	bool positif( asin( vectoriel(v).norme() / (norme() * v.norme()) ) > 0 ? true : false);
	double a( acos(scalar(v) / (norme() * v.norme())));
	return positif ? a : 2*PI - a;
}

double Vector::angleDegree(Vector v) {
	return angle(v) * 180.0 / PI;
}

