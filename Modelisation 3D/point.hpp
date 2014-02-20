class Vector;

class Point {
	
private :
	double x, y, z;

public :
	Point();
	Point(double x, double y, double z);
	Point(const Point& p);

	double getX();
	double getY();
	double getZ();

	void setX(double x);
	void setY(double y);
	void setZ(double z);

	Point projectOnLine(Point p1, Point p2);
	Point projectOnLine(Point p1, Vector v);
	Point projectOnPlan(Point pointOnPlan, Vector normalOfPlan);
};

