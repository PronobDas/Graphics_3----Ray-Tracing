#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#include <iostream>
#include <fstream>
#include <vector>

#include "classes.h"
#include "bitmap_image.hpp"
using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle1, angle2, angle3, angle4;

//Object *obj;
vector<Object*> objects;
vector<Light*> lights;


point pos;
point u, r, l;
int levelOfRec, pixelsDimBoth, nObjects, nLightSrc;
// pixelsDimBoth == imageWidth

double dotMul(point a, point b)
{
    double result;
    result = a.x * b.x + a.y * b.y + a.z * b.z;
    return  result;
}


point crossMul(point a, point b)
{
    point result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}


void drawAxes()
{
	if(drawaxes==1)
	{

		glBegin(GL_LINES);{
		    glColor3f(1.0, 0, 0);
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

			glColor3f(0, 1.0, 0);
			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);

			glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}


void capture()
{
    cout << "capture called." << endl;
    // Initializing image buffer and Background->black.
    bitmap_image image(pixelsDimBoth, pixelsDimBoth);
    for(int i = 0; i < pixelsDimBoth; i++)
    {
        for (int j = 0; j < pixelsDimBoth; j++)
            image.set_pixel(i,j,0, 0, 0);
    }

    double viewAngle = (pi/180)*80; //fovY = 80
    double planeDistance = (500.0/2) / tan(viewAngle/2);
    //cout << planeDistance<<endl;
    point topLeft = pos + l * planeDistance - r * (500/2) + u * (500/2);
    //topLeft.print();
    double du = 500.0 / pixelsDimBoth;
    double dv = 500.0 / pixelsDimBoth;

    topLeft = topLeft + r * (0.5 * du) - u * (0.5 * dv);

    for ( int i = 0; i < pixelsDimBoth; i++ )
    {
        for (int j = 0; j < pixelsDimBoth; j++)
        {
            point corner = topLeft + r * (i * du) - u * (j * dv);

            Vector eye(pos.x, pos.y, pos.z);
            Vector d( (corner-pos).x , (corner-pos).y, (corner-pos).z ); // direction vector
            d.normalize();

            Ray ray(eye, d);

            int nearest;
            double t, tMin = 99999999;
            double *color = new double[3];
            nearest = -1;

            for (int k = 0; k < objects.size(); k++ )
            {
                t = objects[k]->intersect(ray, color, 0);
               // cout << "t: " << t << endl;
                if (t <= 0)
                    continue;
                if (t < tMin)
                {
                    tMin = t;
                    nearest = k;
                }
            }
            if (nearest != -1)
            {
                tMin = objects[nearest]->intersect(ray, color, 1);
            }

            image.set_pixel(i,j, color[0] * 255, color[1] * 255, color[2] * 255);

            //for (int i = 0; i < 3; i++)
            //    delete color;
        }
    }
    image.save_image("E:\\L 4 T 1\\CSE 409 & 410 (Graphics)\\Offlines\\Offline3\\out.bmp");


}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){
	    case '0':
            capture();
            cout << "captured." << endl;
            break;

		case '1':
			drawgrid=1-drawgrid;
			r = r * cos(pi/100) + crossMul(u,r)* sin(pi/100); //+ u*(dotMul(u,r)*(1-cos(pi/100)));
			l = l * cos(pi/100) + crossMul(u,l)* sin(pi/100); //+ u*(dotMul(u,r)*(1-cos(pi/100)));

			break;
        case '2':
            r = r * cos(pi/100) - crossMul(u,r)* sin(pi/100);
			l = l * cos(pi/100) - crossMul(u,l)* sin(pi/100);
            break;
        case '3':
            u = u * cos(pi/180) + crossMul(r,u)* sin(pi/180);
			l = l * cos(pi/180) + crossMul(r,l)* sin(pi/180);
            break;
        case '4':
            u = u * cos(pi/180) - crossMul(r,u)* sin(pi/180);
			l = l * cos(pi/180) - crossMul(r,l)* sin(pi/180);
            break;
        case '5':
            u = u * cos(pi/180) + crossMul(l,u)* sin(pi/180);
			r = r * cos(pi/180) + crossMul(l,r)* sin(pi/180);
            break;
        case '6':
            u = u * cos(pi/180) - crossMul(l,u)* sin(pi/180);
			r = r * cos(pi/180) - crossMul(l,r)* sin(pi/180);
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			pos = pos - l*1.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			pos = pos + l*1.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			pos = pos + r*1.0;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			pos = pos - r*1.0;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos = pos + u*1.0;
			break;
		case GLUT_KEY_PAGE_DOWN:
		    pos = pos - u*1.0;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x, pos.y+l.y, pos.z+l.z,	u.x,u.y,u.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//objects[0]->draw();
	for (int i = 0; i < objects.size(); i++){
        objects[i]->draw();
	}

	for (int j = 0; j < lights.size(); j++){
        lights[j]->draw();
	}

//	cout << obj->length;
	//drawGrid();
	//drawSphere(50, 50, 50);
	//drawSquare(100);

    //glColor3f(1,0,0);
    //drawCircle(85, 36);
    //drawBubble(15, 36);
    //drawBubbles();
    //drawCone(20,50,24);

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}


void init(){
	//codes for initialization
	drawgrid = 0;
	drawaxes = 1;
	cameraHeight = 150.0;
	cameraAngle = 1.0;
	angle1 = 0;
	angle2 = 0;
	angle3 = 0;
	angle4 = 0;

	u.x = 0;
	u.y = 0;
	u.z = 1;

	r.x = -1/sqrt(2);
	r.y = 1/sqrt(2);
	r.z = 0;

	l.x = -1/sqrt(2);
	l.y = -1/sqrt(2);
	l.z = 0;

	pos.x = 100;
	pos.y = 100;
	pos.z = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}


void loadData(){
    ifstream scene;
    scene.open("E:\\L 4 T 1\\CSE 409 & 410 (Graphics)\\Offlines\\Offline3\\scene.txt");

    scene >> levelOfRec >> pixelsDimBoth >> nObjects;


    while ( nObjects-- )
    {
        string cmd;
        scene >> cmd;
        if ( cmd == "sphere" )
        {
            cout << "sphere" << endl;
            double centerX, centerY, centerZ, radius;
            double R, G, B;
            double ambient, diffuse, specular, recRefCoef, shininess;
            scene >> centerX >> centerY >> centerZ >> radius;
            scene >> R >> G >> B;
            scene >> ambient >> diffuse >> specular >> recRefCoef >> shininess;
            //cout << shininess << endl;

            Object *obj = new Sphere(Vector(centerX, centerY, centerZ), radius);
            obj->setColor(R, G, B);
            obj->setCoEfficients(ambient, diffuse, specular, recRefCoef);
            obj->setShininess(shininess);
            objects.push_back(obj);

        }
        else if ( cmd == "triangle" )
        {
            cout << "triangle" << endl;
            double x1, y1, z1,  x2, y2, z2,  x3, y3, z3;
            double R, G, B;
            double ambient, diffuse, specular, recRefCoef, shininess;
            scene >> x1 >> y1 >> z1;
            scene >> x2 >> y2 >> z2;
            scene >> x3 >> y3 >> z3;
            scene >> R >> G >> B;
            scene >> ambient >> diffuse >> specular >> recRefCoef >> shininess;
            //cout << shininess << endl;

            Object *obj = new Triangle(Vector(x1, y1, z1), Vector(x2, y2, z2), Vector(x3, y3, z3));
            obj->setColor(R, G, B);
            obj->setCoEfficients(ambient, diffuse, specular, recRefCoef);
            obj->setShininess(shininess);
            objects.push_back(obj);

        }
        else if ( cmd == "general" )
        {
            cout << "general" << endl;
            double A, B, C, D, E, F, G, H, I, J;
            double crpX, crpY, crpZ, length, width, height;
            double cR, cG, cB;
            double ambient, diffuse, specular, recRefCoef, shininess;
            scene >> A >> B >> C >> D >> E >> F >> G >> H >> I >> J;
            scene >> crpX >> crpY >> crpZ >> length >> width >> height;
            scene >> cR >> cG >> cB;
            scene >> ambient >> diffuse >> specular >> recRefCoef >> shininess;
            //cout << shininess << endl;
            Vector crp(crpX, crpY, crpZ);
            Object *obj = new General(A, B, C, D, E, F, G, H, I, J);
            obj->setRefferencePoint(crp);
            obj->setHWL(height, width, length);
            obj->setColor(cR, cG, cB);
            obj->setCoEfficients(ambient, diffuse, specular, recRefCoef);
            obj->setShininess(shininess);
            objects.push_back(obj);
        }
    }


    scene >> nLightSrc;
    while ( nLightSrc-- )
    {
        double posX, posY, posZ;
        double R, G, B;

        scene >> posX >> posY >> posZ;
        scene >> R >> G >> B;

        Light *light= new Light(Vector(posX, posY, posZ), R, G, B);
        lights.push_back(light);
    }

    scene.close();

    //Floor object
    Object *floor = new Floor(100, 20);
    floor->setCoEfficients(0.4, 0.2, 0.2, 0.2);
    floor->setShininess(1);
    floor->setColor(0, 0.5, 0.5);
    objects.push_back(floor);
}


int main(int argc, char **argv){
    loadData();
    //testData();
    //cout << objects.size();
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Assignment 3");


	init();


	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	//memory management.
	for(int i = 0; i < objects.size(); i++)
        delete objects[i];
    for(int i = 0; i < lights.size(); i++)
        delete lights[i];

	return 0;
}


