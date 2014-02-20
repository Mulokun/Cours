class Point;

class Vector {
	
private :
	double x, y, z;

public :
	Vector();
	Vector(double x, double y, double z);
	Vector(Point p1, Point p2);
	Vector(const Vector& v);

	double getX();
	double getY();
	double getZ();

	void setX(double x);
	void setY(double y);
	void setZ(double z);

	double norme();
	void normalize();
	
	double scalar(Vector v);
	Vector vectoriel(Vector v);

	double angle(Vector v);
	double angleDegree(Vector v);
};
