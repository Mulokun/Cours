#include "point.hpp"

#include <cmath>

#include <iostream>
using namespace std;

#include "vector.hpp"

Point::Point() : x(0), y(0), z(0) {}
Point::Point(double x, double y, double z) : x(x), y(y), z(z) {}
Point::Point(const Point& p) : x(p.x), y(p.y), z(p.z) {}

double Point::getX() { return x; }
double Point::getY() { return y; }
double Point::getZ() { return z; }

void Point::setX(double x) { this->x = x; }
void Point::setY(double y) { this->y = y; }
void Point::setZ(double z) { this->z = z; }

Point Point::projectOnLine(Point p1, Point p2) {
	Vector BA(p1, *this);
	Vector BC(p1, p2);
	double BaNorme( BA.scalar(BC) / BC.norme() );

	cout << "Norme BA' : " << BaNorme << endl;

	BC.normalize();

	Point projection;
	projection.x = p1.x + (BC.getX() * BaNorme);
	projection.y = p1.y + (BC.getY() * BaNorme);
	projection.z = p1.z + (BC.getZ() * BaNorme);

	return projection;
}

Point Point::projectOnLine(Point p1, Vector v) {
	Point p2(p1.getX() + v.getX(), p1.getY() + v.getY(), p1.getZ() + v.getZ());
	return projectOnLine(p1, p2);
}

Point Point::projectOnPlan(Point pointOnPlan, Vector normalOfPlan) {
	Vector AM(pointOnPlan, *this);
	double MmNorme(AM.scalar(normalOfPlan) / normalOfPlan.norme());

	cout << "Norme MM' : " << MmNorme << endl;
	cout << "NoP : " << normalOfPlan.getX() << " " << normalOfPlan.getY() << " " << normalOfPlan.getZ() << endl;
	
	normalOfPlan.normalize();
	cout << "NoP : " << normalOfPlan.getX() << " " << normalOfPlan.getY() << " " << normalOfPlan.getZ() << endl;
	Point projection;
	projection.x = x - (normalOfPlan.getX() * MmNorme);
	projection.y = y - (normalOfPlan.getY() * MmNorme);
	projection.z = z - (normalOfPlan.getZ() * MmNorme);

	return projection;
}


