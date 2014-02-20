///////////////////////////////////////////////////////////////////////////////
// Imagina
// ----------------------------------------------------------------------------
// IN - Synthèse d'images - Modélisation géométrique
// Auteur : Gilles Gesquière
// ----------------------------------------------------------------------------
// Base du TP 1
// programme permettant de créer des formes de bases.
// La forme représentée ici est un polygone blanc dessiné sur un fond rouge
///////////////////////////////////////////////////////////////////////////////  

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include <vector>

#include <iostream>
using namespace std;

#include "point.hpp"
#include "vector.hpp"

/* Dans les salles de TP, vous avez généralement accès aux glut dans C:\Dev. Si ce n'est pas le cas, téléchargez les .h .lib ...
Vous pouvez ensuite y faire référence en spécifiant le chemin dans visual. Vous utiliserez alors #include <glut.h>. 
Si vous mettez glut dans le répertoire courant, on aura alors #include "glut.h" 
*/

#include <GL/glut.h> 

// Définition de la taille de la fenêtre
#define WIDTH  480
#define HEIGHT 480

// Définition de la couleur de la fenêtre
#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 1

#define PI 3.141559265

// Touche echap (Esc) permet de sortir du programme
#define KEY_ESC 27


// Entêtes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 


void drawPoint(Point p);
void drawVector(Vector v);
void drawVector(Vector v, Point o);
void drawTriangles(vector<Point> tri);
void drawLine(Point p1, Point p2);
void drawCurve(vector<Point> p);
void drawCurveSpe(vector<Point> * p, int size);

vector<Point> hermiteCubicCurve(Point p0, Point p1, Vector v0, Vector v1, unsigned int u);
Point hermiteCubicPoint(Point p0, Point p1, Vector v0, Vector v1, double u);
double F1(double u);
double F2(double u);
double F3(double u);
double F4(double u);

int fact(int n);
double polynomeBernstein(unsigned int n, unsigned int i, double u);
Point pointBernstein(vector<Point> ptsControle, double u);
vector<Point> bezierCurveByBernstein( vector<Point> ptsControle, unsigned int nbU);

int _select = -1;
vector<Point> _pts;
int _e = 1;
int _u = 1;

void selectMouse(int button, int state, int x, int y);
void move(int x, int y);

double toGraphY(int y);
double toGraphX(int x);
	
vector<Point> * pointCurveByCasteljau(vector<Point> pControl, int nbU, int e);

vector<Point> surface( vector<Point> p1, vector<Point> p2, int nbU);

//facettisation :

struct fCylindre {
	double r;
	double h;
	int m;
	vector<Point> p;
};

struct fCylindre facettiseCylindre(double r, double h, int m, Vector norme);
void drawCylindre(struct fCylindre c);



int main(int argc, char **argv) 
{  
	_pts.push_back( Point(0, 0, 0) );
	_pts.push_back( Point(1, 1, 0) );
	_pts.push_back( Point(2, 0, 0) );
	_pts.push_back( Point(2, 1, 0) );
	_pts.push_back( Point(2, -1, 0) );
//	_pts.push_back( Point(2, 3, 0) );
//	_pts.push_back( Point(2, -2, 0) );
//	_pts.push_back( Point(1, 2, 0) );

 	// initialisation  des paramètres de GLUT en fonction
	// des arguments sur la ligne de commande
  	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_RGBA);

  	// définition et création de la fenêtre graphique, ainsi que son titre
  	glutInitWindowSize(WIDTH, HEIGHT);
  	glutInitWindowPosition(0, 0);
  	glutCreateWindow("Premier exemple : carré");


	glutMouseFunc(&selectMouse);
	glutMotionFunc(&move);

  	// initialisation de OpenGL et de la scène
  	initGL();  
  	init_scene();

  	// choix des procédures de callback pour 
  	// le tracé graphique
  	glutDisplayFunc(&window_display);
  	// le redimensionnement de la fenêtre
  	glutReshapeFunc(&window_reshape);
  	// la gestion des événements clavier
  	glutKeyboardFunc(&window_key);

  	// la boucle prinicipale de gestion des événements utilisateur
  	glutMainLoop();  

  	return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque
GLvoid initGL() 
{
  	glClearColor(RED, GREEN, BLUE, ALPHA);        
}

// Initialisation de la scene. Peut servir à stocker des variables de votre programme
// à initialiser
void init_scene()
{
}

// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display()
{
  	glClear(GL_COLOR_BUFFER_BIT);
  	glLoadIdentity();

  	// C'est l'endroit où l'on peut dessiner. On peut aussi faire appel
  	// à une fonction (render_scene() ici) qui contient les informations 
  	// que l'on veut dessiner
  	render_scene();

  	// trace la scène grapnique qui vient juste d'être définie
  	glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{  
  	glViewport(0, 0, width, height);

  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	// ici, vous verrez pendant le cours sur les projections qu'en modifiant les valeurs, il est
  	// possible de changer la taille de l'objet dans la fenêtre. Augmentez ces valeurs si l'objet est 
  	// de trop grosse taille par rapport à la fenêtre.
  	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);

  	// toutes les transformations suivantes s´appliquent au modèle de vue 
  	glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y) 
{  
  	switch (key) {    
	  	case KEY_ESC:  
    		exit(1);                    
    		break; 

		case '8' :
		_e++;
		window_display();
		break;

		case '2' :
		_e--;
		if( _e < 0) { _e = 0; }
		window_display();
		break;

		case '4' :
		_u--;
		if(_u < 0) { _u = 0; }
		window_display();
		break;

		case '6' :
		_u++;
		window_display();
		break;

  		default:
    		printf ("La touche %d n´est pas active.\n", key);
    		break;
  	}     
}



//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////
void render_scene()
{
	//Définition de la couleur
 	glColor3f(1.0, 1.0, 1.0);
	
	glPolygonMode(GL_FRONT, GL_LINE);



  	//  Nous créons ici un polygone. Nous pourrions aussi créer un triangle ou des lignes. Voir ci-dessous les parties 
  	// en commentaires (il faut commenter le bloc qui ne vous intéresse pas et décommenter celui que vous voulez tester.

   	// Création de deux lignes
/*	glBegin(GL_LINES);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(-1, 1, 0); 
	glEnd();
*/

 // création d'un polygone
/*	glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(-1, 1, 0);
	glEnd();
*/


/*
// création d'un triangle
	glBegin(GL_TRIANGLES);
		glVertex3f(-1, -1, 0);
		glVertex3f(1, -1, 0);
		glVertex3f(1, 1, 0);
	glEnd();
*/

	glPointSize(3);

	glColor3f(0.2, 0.2, 0.2);

	// ord, abs 	
	Point O(0, 0, 0);
	Vector H(1, 0, 0);
	Vector V(0, 1, 0);
	drawPoint(O);
	drawVector(H, O);
	drawVector(V, O);

	glColor3f(1.0, 1.0, 1.0);

/*
	Point A(0, 0, 0);
	Point C(1.4, 0.1, 0);
	Vector v(0.3, 1.5, 0);
	Point B(-1, -2, 0);
	//Point b(B.projectOnPlan(A, v));
	Point b(B.projectOnLine(A, C));

	cout << "b : " << b.getX() << " " << b.getY() << " " << b.getZ() << endl;

	drawPoint(A);
	//drawVector(v, A);
	drawLine(A, C);
	drawPoint(B);

	glColor3f(1.0, 0.0, 0.0);

	drawPoint(b);
*/
	
//	vector<Point> ps = hermiteCubicCurve( Point(0, 0, 0), Point(2, 0, 0), Vector(1, 1, 0), Vector(1, -1, 0), 16);
//	drawCurve( ps );

	vector<Point> p, p2;
//	p = bezierCurveByBernstein(ps, 50);
//	p2 = bezierCurveByBernstein(_pts, 50);
//	drawCurve( p );
//	drawCurve( p2 );

	glColor3f(1.0, 0.0, 0.0);

	/*
	for(int i = 0; i < _pts.size(); i++) {
		drawPoint(_pts[i]);
	}

/*
	glColor3f(1.0, 1.0, 1.0);
	vector<Point> * p3;
	int u = 3;
	p3 = pointCurveByCasteljau(_pts, u, 1);
	
	drawCurveSpe(p3, u + 1);
	
*/

/*   CASTELJAU :
	glColor3f(0.4, 0.4, 0.4);
	
	for(int i=0;i<_e;i++) {
		vector<Point> * p = pointCurveByCasteljau(_pts, _u, i);
		drawCurveSpe( p, _u+1 );
		delete [] p;
	}
	
	glColor3f(1.0, 1.0, 1.0);

	if(_e > 0) {
		vector<Point> * n = pointCurveByCasteljau(_pts, _u, _e);
		drawCurveSpe(n, _u+1);
		delete [] n;
	} else {
		drawCurve(_pts);
	}
*/

// TRIANGLULATION :
/*
	vector<Point> P1, P2;

	P1.push_back( Point( -3, 2, 0 ) );
	P1.push_back( Point( 1, 2, 0 ) );
	P1.push_back( Point( 3, 1, 0 ) );
	
	P2.push_back( Point( -3, -2, 0 ) );
	P2.push_back( Point( -1, -1, 0 ) );
	P2.push_back( Point( 2, -2, 0 ) );

	vector<Point> tri;
	tri = surface(P1, P2, 10);

	drawTriangles(tri);
*/

	struct fCylindre c = facettiseCylindre(1.0, 1.0, 10, Vector(1.0, 2.0, 1.0));
	drawCylindre(c);

}

void drawTriangles( vector<Point> tri)
{
	glBegin(GL_TRIANGLES);
	for(int i = 0; i<tri.size(); i++) {
		glVertex3f((tri[i]).getX(), (tri[i]).getY(), (tri[i]).getZ());
	}
	glEnd();
}

void drawPoint(Point p) {
	glBegin(GL_POINTS);
		glVertex3f(p.getX(), p.getY(), p.getZ());
	glEnd();
}

void drawVector(Vector v) {
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(v.getX(), v.getY(), v.getZ());
	glEnd();
}

void drawVector(Vector v, Point o) {
	glBegin(GL_LINES);
		glVertex3f(o.getX(), o.getY(), o.getZ());
		glVertex3f(o.getX() + v.getX(), o.getY() + v.getY(), o.getZ() + v.getZ());
	glEnd();
}

void drawLine(Point p1, Point p2) {
	glBegin(GL_LINES);
		glVertex3f(p1.getX(), p1.getY(), p1.getZ());
		glVertex3f(p2.getX(), p2.getY(), p2.getZ());
	glEnd();

}

void drawCurve(vector<Point> p) {
	glBegin(GL_LINE_STRIP);
		for(int i=0; i<p.size(); i++) {
			glVertex3f(p[i].getX(), p[i].getY(), p[i].getZ());
			//cout << p[i].getX() << " " << p[i].getY() << endl;
		}
	glEnd();
}

void drawCurveSpe(vector<Point> * p, int size) {
	if((p[0]).size() > 1) {
		for(int i=0;i<size;i++) {
			drawCurve(p[i]);
		}
	}
	else {
		vector<Point> inter;
		for(int i=0;i<size;i++) {
			inter.push_back( (p[i])[0] );
		}
		drawCurve(inter);
	}
}

vector<Point> hermiteCubicCurve(Point p0, Point p1, Vector v0, Vector v1, unsigned int nbU) {
	vector<Point> p;
	p.push_back( hermiteCubicPoint(p0, p1, v0, v1, 0) );
	for(unsigned int i=1; i<=nbU; i++) {
		p.push_back( hermiteCubicPoint(p0, p1, v0, v1, (double)i / (double)nbU) );
	}
	return p;
}

Point hermiteCubicPoint(Point p0, Point p1, Vector v0, Vector v1, double u) {
	double x, y, z;

	double f1 = F1(u);
	double f2 = F2(u);
	double f3 = F3(u);
	double f4 = F4(u);

	x = f1 * p0.getX() + f2 * p1.getX() + f3 * v0.getX() + f4 * v1.getX();
	y = f1 * p0.getY() + f2 * p1.getY() + f3 * v0.getY() + f4 * v1.getY();
	z = f1 * p0.getZ() + f2 * p1.getZ() + f3 * v0.getZ() + f4 * v1.getZ();
	
	//cout << "x:" << x << " y:" << y << " z:" << z << " u:" << u << endl;
	return Point(x, y, z);
}

double F1(double u) {
	return 2.0 * u * u * u - 3.0 * u * u + 1.0;
}
double F2(double u) {
	return - 2.0 * u * u * u + 3.0 * u * u;
}
double F3(double u) {
	return u * u * u - 2.0 * u * u + u;
}
double F4(double u) {
	return u * u * u - u * u;
}

int fact(int n) {
	if(n <= 1) { return 1; }
	return n * fact(n - 1);
}

double polynomeBernstein(unsigned int n, unsigned int i, double u) {
	return (fact(n) / ( fact(i) * fact(n-i) )) * pow(u, i) * pow(1.0 - u, n - (double)i);
}

Point pointBernstein(vector<Point> ptsControle, double u) {
	double x = 0, y = 0, z = 0;

	for(int i=0;i<ptsControle.size();i++) {
		x += polynomeBernstein(ptsControle.size()-1, i, u) * ptsControle[i].getX();
		y += polynomeBernstein(ptsControle.size()-1, i, u) * ptsControle[i].getY();
		z += polynomeBernstein(ptsControle.size()-1, i, u) * ptsControle[i].getZ();
	}

	//cout << "x:" << x << " y:" << y << " z:" << z << " u:" << u << endl;
	return Point(x, y, z);
}

vector<Point> bezierCurveByBernstein( vector<Point> ptsControle, unsigned int nbU) {
	vector<Point> p;
	for(unsigned int i=0; i<=nbU; i++) {
		p.push_back( pointBernstein(ptsControle, (double)i / nbU) );
	}
	return p;
}




void selectMouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		double dx = toGraphX(x);
		double dy = toGraphY(y);
		//cout << dx << " - " << dy << endl;
		_select = -1;
		for(int i=0; i<_pts.size();i++) {
			if(dx > _pts[i].getX() - 0.2 && dx < _pts[i].getX() + 0.2  && dy > _pts[i].getY() - 0.2 && dy < _pts[i].getY() + 0.2) {
				//cout << "p" << endl;
				_select = i;
				break;
			}
		}
	}
}


void move(int x, int y) {
	if(_select > -1) {
		_pts[_select].setX(toGraphX(x));
		_pts[_select].setY(toGraphY(y));

		window_display();
	}
}

double toGraphY(int y) {
	return ((double)y / (double)HEIGHT * 8.0 - 4.0) * -1;	
}

double toGraphX(int x) {
	return (double)x / (double)WIDTH * 8.0 - 4.0;	
}

Point pointAt(Point p1, Point p2, double u) {
	if(u == 0.0) { return p1; }

	double x = (p2.getX() - p1.getX()) * u + p1.getX();
	double y = (p2.getY() - p1.getY()) * u + p1.getY();
	double z = (p2.getZ() - p1.getZ()) * u + p1.getZ();

	return Point(x, y, z);
}

vector<Point> * pointCurveByCasteljau(vector<Point> pControl, int nbU, int e)
{
	//nbC = pControl.size() > nbC ? nbC : pControl.size();
	vector<Point> p = pControl;
	vector<Point> * n = new vector<Point>[nbU+1];
	for(int i=0;i<=nbU;i++) {
		for(int j=0;j<p.size()-1;j++) {
			n[i].push_back( Point( pointAt(p[j], p[j+1], (double)i / (double)nbU) ) );
		}
	}

	for(int i=0;i<=nbU;i++) {
		int k = e;
		vector<Point> t;
		while( n[i].size() > 1 && k > 1 ) {
			for(int j=0;j<n[i].size() - 1;j++) {
				t.push_back( Point( pointAt((n[i])[j], (n[i])[j+1], (double)i / (double)nbU) ) );
			}
			n[i] = t;
			t.clear();
			k--;
		}
	}

	return n;
}


vector<Point> surface( vector<Point> p1, vector<Point> p2, int nbU)
{
	nbU = _e;
	int nbV = _u;

	p1 = bezierCurveByBernstein(p1, nbU);
	p2 = bezierCurveByBernstein(p2, nbU);

	vector<Point> tri;

	for(int u=0; u<nbU; u++) {
		for(int v=0; v<nbV; v++) {
			Point t[3];
			tri.push_back( pointAt(p1[u], p2[u], (double)v / (double)nbV) );
			tri.push_back( pointAt(p1[u], p2[u], (double)(v+1) / (double)nbV) );
			tri.push_back( pointAt(p1[u+1], p2[u+1], (double)v / (double)nbV) );

			tri.push_back( pointAt(p1[u], p2[u], (double)(v+1) / (double)nbV) );
			tri.push_back( pointAt(p1[u+1], p2[u+1], (double)(v+1) / (double)nbV) );
			tri.push_back( pointAt(p1[u+1], p2[u+1], (double)v / (double)nbV) );
			
		}
	}

	return tri;
}


struct fCylindre facettiseCylindre(double r, double h, int m, Vector norme)
{
	//norme.normalize();

	cout << norme.getX() << endl;
	cout << norme.getY() << endl;
	cout << norme.getZ() << endl;

	vector<Point> ps;

	for(int i = 0; i<m; i++) {
		double x, y;
		x = cos(2 * PI / m * i) * r;
		y = sin(2 * PI / m * i) * r;
		ps.push_back( Point(x, y, 0) );
	}

	for(int i = 0; i<m; i++) {
		double x, y;
		x = cos(2 * PI / m * i) * r;
		y = sin(2 * PI / m * i) * r;
		ps.push_back( Point(x, y, 2) );
	}

	for(int i=0;i<ps.size();i++) {
		ps[i].setX( ps[i].getX() * norme.getX() );
		ps[i].setY( ps[i].getY() * norme.getY() );
		ps[i].setZ( ps[i].getZ() * norme.getZ() );
	}


	struct fCylindre c;
	c.r = r;
	c.h = h;
	c.m = m;
	c.p = ps;
	return c;
}


void drawCylindre(struct fCylindre c)
{
	int m = c.m;
	
	glBegin(GL_QUADS);
	for(int i=0;i<m;i++) {
		glVertex3f( c.p[i].getX(), c.p[i].getY(), c.p[i].getZ() );
		glVertex3f( c.p[(i+1)%m].getX(), c.p[(i+1)%m].getY(), c.p[(i+1)%m].getZ() );
		
		glVertex3f( c.p[(i+1)%m + m].getX(), c.p[(i+1)%m + m].getY(), c.p[(i+1)%m + m].getZ() );
		glVertex3f( c.p[i + m].getX(), c.p[i + m].getY(), c.p[i + m].getZ() );
	}
	glEnd();
/*
	glBegin(GL_POLYGON);
	for(int i=0; i<c.m;i++) {
		glVertex3f( c.p[i].getX(), c.p[i].getY(), c.p[i].getZ() );	
	cout << " ------ " << endl;
	cout << c.p[i].getX() << endl;
	cout << c.p[i].getY() << endl;
	cout << c.p[i].getZ() << endl;


	}
	glEnd();
	cout << " ------ " << endl;
*/
	glBegin(GL_POLYGON);
	for(int i=0; i<c.m;i++) {
		glVertex3f( c.p[i+m].getX(), c.p[i+m].getY(), c.p[i+m].getZ() );	
	cout << " ------ " << endl;
	cout << c.p[i+m].getX() << endl;
	cout << c.p[i+m].getY() << endl;
	cout << c.p[i+m].getZ() << endl;
	}
	glEnd();

}








