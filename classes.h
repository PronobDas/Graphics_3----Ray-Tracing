#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <GL/glut.h>

#include <iostream>
#include <fstream>

using namespace std;

struct point
{
    double x,y,z;

    point operator+(point p)
    {
        return {x+p.x, y+p.y, z+p.z};
    }
    point operator-(point p)
    {
        return {x-p.x, y-p.y, z-p.z};
    }
    point operator*(double n)
    {
        return {x*n, y*n, z*n};
    }
    void print()
    {
        cout << x << " " << y << " " << z << endl;
    }
};


class Vector
{
public:
    double x, y, z;

    Vector() {}

    Vector(double a, double b, double c)
    {
        x = a;
        y = b;
        z = c;
    }

    void normalize()
    {
        double r = sqrt(x*x + y*y + z*z);
        x = x/r;
        y = y/r;
        z = z/r;
    }

    Vector operator+( Vector v)
    {
        Vector temp = Vector();
        temp.x = x + v.x;
        temp.y = y + v.y;
        temp.z = z + v.z;
        return temp;
    }

    Vector operator-( Vector v)
    {
        Vector temp = Vector();
        temp.x = x - v.x;
        temp.y = y - v.y;
        temp.z = z - v.z;
        return temp;
    }

    // constant multiplication
    Vector operator*( double d)
    {
        Vector temp = Vector();
        temp.x = d*x;
        temp.y = d*y;
        temp.z = d*z;
        return temp;
    }

    static double dotMul(Vector a, Vector b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vector crossMul(Vector a, Vector b)
    {
        Vector temp = Vector();
        temp.x = a.y * b.z - b.y * a.z;
        temp.y = b.x * a.z - a.x * b.z;
        temp.z = a.x * b.y - b.x * a.y;
        return temp;
    }

    void print()
    {
        cout << x << "  " << y << "  " << z << endl;
    }
};


class Ray
{
public:
    Vector start;
    Vector direction;

    Ray(Vector s, Vector d)
    {
        start = s;
        direction = d;
    }
};


class Object
{
public:
    Vector referencePoint;
    double height, width, length;
    int shininess;
    double color[3];
    double coEfficients[4];

    Object() {}

    virtual void draw() {}

    virtual double intersect( Ray r, double *color, int level)
    {
        return -1.0;
    }

    void setRefferencePoint(Vector p)
    {
        referencePoint = p;
    }
    void setHWL(double h, double w, double l)
    {
        height = h;
        width = w;
        length = l;
    }

    void setColor(double r, double g, double b)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    void setShininess(int s)
    {
        shininess = s;
    }

    void setCoEfficients(double ambient, double diffuse, double specular, double recRefCoef)
    {
        coEfficients[0] = ambient;
        coEfficients[1] = diffuse;
        coEfficients[2] = specular;
        coEfficients[3] = recRefCoef;
    }
};


class Sphere : public Object
{
public:
    Sphere(Vector center, double radius)
    {
        referencePoint = center;
        length = radius;
    }

    void draw()
    {
        glPushMatrix();
        glColor3f(color[0], color[1], color[2]);
        glTranslatef(referencePoint.x, referencePoint.y, referencePoint.z);
        double radius = length;
        int slices = 36;
        int stacks = 36;
        struct point points[100][100];
        int i,j;
        double h,r;

        //generate points
        for(i=0; i<=stacks; i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0; j<=slices; j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0; i<stacks; i++)
        {
            for(j=0; j<slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }

    double intersect( Ray r, double *color, int level)
    {
        double t = -1;
        Vector r0 = r.start - referencePoint; // reference_point == center
        double radius = length;
        double a = Vector::dotMul(r.direction, r.direction);
        double b = 2 * Vector::dotMul(r0, r.direction);
        double c = Vector::dotMul(r0, r0) - radius*radius;

        double D = b*b - 4*a*c;

        if (D < 0)
            return -1.0;
        else
        {
            double t1 = (-b + sqrt(D)) / (2 * a);
            double t2 = (-b - sqrt(D)) / (2 * a);

            t =  min(t1, t2);
        }
        if (t <= 0)
            return -1;
        if (level == 0)
            return t;

        color[0] = this->color[0];
        color[1] = this->color[1];
        color[2] = this->color[2];

        return t;
    }
};


class Triangle : public Object
{
public:
    Vector v0, v1, v2;

    Triangle(Vector a, Vector b, Vector c)
    {
        v0 = a;
        v1 = b;
        v2 = c;
    }

    void draw()
    {
        glPushMatrix();
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v0.x, v0.y, v0.z);
        }
        glEnd();
        glPopMatrix();
    }

    double intersect( Ray r, double *color, int level)
    {
        double t = -1;
        double Eps = 0.0000001, a, f, u, v;
        Vector edge1, edge2, h, s, q;

        edge1 = v1 - v0;
        edge2 = v2 - v0;
        h = Vector::crossMul(r.direction, edge2);
        a = Vector::dotMul(edge1, h);

        if (a > -Eps && a < Eps)
            return -1.0;    // Ray is parallel to this triangle.

        f = 1.0/a;
        s = r.start - v0;
        u = f * Vector::dotMul(s, h);

        if (u < 0.0 || u > 1.0)
            return -1.0;

        q = Vector::crossMul(s, edge1);
        v = f * Vector::dotMul(r.direction, q);

        if (v < 0.0 || u + v > 1.0)
            return -1.0;

        t = f * Vector::dotMul(edge2, q);
        if (t > Eps) // ray intersection
        {
            if (level == 0)
                return t;
            else
            {
                color[0] = this->color[0];
                color[1] = this->color[1];
                color[2] = this->color[2];

                return t;
            }
        }
        else
            return -1.0;
    }
};


class General : public Object
{
public:
    double A, B, C, D, E, F, _G, H, I, J;

    General(double a, double b, double c, double d, double e, double f, double _g, double h, double i, double j)
    {
        A = a;
        B = b;
        C = c;
        D = d;
        E = e;
        F = f;
        _G = _g;
        H = h;
        I = i;
        J = j;
    }

    double intersect( Ray r, double *color, int level)
    {
        return -1.0;
    }

};


class Floor : public Object
{
public:
    Floor(double floorWidth, double tileWidth)
    {
        referencePoint = Vector(-floorWidth/2, -floorWidth/2, 0);
        length = tileWidth;
    }

    void draw()
    {
        glBegin(GL_QUADS);
        {
            for (int i = -30; i < 30; i++)
            {
                for (int j = -30; j < 30; j++)
                {
                    if ((i + j) % 2 == 0)
                        glColor3f(1, 1, 1);
                    else
                        glColor3f(0, 0, 0);
                    // length == tileWidth
                    glVertex3f(i * length, j * length, 0);
                    glVertex3f(i * length, j * length + length, 0);
                    glVertex3f(i * length + length, j * length + length, 0);
                    glVertex3f(i * length + length, j * length, 0);
                }
            }
        }
        glEnd();

    }


    double intersect( Ray r, double *color, int level)
    {
        double t = -1.0;

        if (r.direction.z == 0)
            return -1.0;
        t = - r.start.z / r.direction.z;


        if (t <= 0)
            return -1;
        if (level == 0)
            return t;

        Vector intersectPoint = r.start + r.direction * t;
        //cout << "inP:";
        //intersectPoint.print();
        int i = ceil( (intersectPoint.x - referencePoint.x)/length );
        int j = ceil( (intersectPoint.y - referencePoint.y)/length );

        if ( (i+j)%2 != 0 )
        {
            color[0] = 1;
            color[1] = 1;
            color[2] = 1;
        }
        else
        {
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }

        return t;
    }
};


class Light
{
public:
    Vector lightPos;
    double color[3];

    Light() {}

    Light(Vector lpos, double R, double G, double B)
    {
        lightPos = lpos;
        color[0] = R;
        color[1] = G;
        color[2] = B;
    }

    void draw()
    {
        glPushMatrix();
        glColor3f(color[0], color[1], color[2]);
        glTranslated(lightPos.x, lightPos.y, lightPos.z);

       /* glBegin(GL_QUADS);
        {
            glVertex3f(lightPos.x, lightPos.y, lightPos.z);
            glVertex3f(lightPos.x + 1, lightPos.y, lightPos.z);
            glVertex3f(lightPos.x, lightPos.y + 1, lightPos.z);
            glVertex3f(lightPos.x, lightPos.y, lightPos.z + 1);

        }
        glEnd();
        */
        double radius = 2;
        int slices = 10;
        int stacks = 10;
        struct point points[100][100];
        int i,j;
        double h,r;
        //generate points
        for(i=0; i<=stacks; i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0; j<=slices; j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        for(i=0; i<stacks; i++)
        {
            for(j=0; j<slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);

                }
                glEnd();
            }
        }

        glPopMatrix();
    }
};



