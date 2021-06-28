#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))
#include <iostream>
using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle1, angle2, angle3, angle4;


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
};

point pos;
point u, r, l;

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
			glVertex3f( 400,0,0);
			glVertex3f(-400,0,0);

			glColor3f(0, 1.0, 0);
			glVertex3f(0,-400,0);
			glVertex3f(0, 400,0);

			glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 400);
			glVertex3f(0,0,-400);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawLine()
{
    glBegin(GL_LINES);{
        glVertex3f(10,20,0);
        glVertex3f(20,10,0);
    }glEnd();
}

void drawSquare(double a)
{
    //glColor3f(1.0,1.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();

	/*glBegin(GL_LINES);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);

		glVertex3f( a, a,2);
		glVertex3f(-a,a,2);

		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);

		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();*/
}

void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawBubble(double x, double y, double radius)
{
    int i;
    struct point points[100];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=36;i++)
    {
        points[i].x = x + radius*cos(((double)i/36)*2*pi);
        points[i].y = y + radius*sin(((double)i/36)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<36;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere1(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	int a = 1;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{
		    glColor3f(1-a,1-a,1-a);
		    a = 1 - a;
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                //glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSphere2(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	int a = 1;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    glColor3f(1-a,1-a,1-a);
		    a = 1 - a;
			glBegin(GL_QUADS);{
			    //upper hemisphere
				//glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSphere3(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	int a = 1;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		r = 2 *radius - r;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{
		    glColor3f(1-a,1-a,1-a);
		    a = 1 - a;
			glBegin(GL_QUADS);{
			    //upper hemisphere
				//glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				//glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				//glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				//glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawCylinder(double radius, double height, int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	int a = 1;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=height*i/stacks;// radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius ; //*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
		    glColor3f(1-a,1-a,1-a);
		    a = 1 - a;
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void myDraw()
{
    glPushMatrix();
    {
        glRotatef(angle1, 0,0,1);

        glColor3f(0, 1, 0.5);
        glRotatef(90, 1,0,0);
        drawSphere1(35, 100, 50); // left sphere of the gun

        glRotatef(angle2, 1,0,0);

        drawSphere2(35, 100, 50); // right sphere of the gun

        glTranslatef(0, 0, -50);

        glRotatef(angle3, 1,0,0);
        glRotatef(angle4, 0,0,1);
        drawSphere1(15, 36, 36);  // connector of sphere and cylinder

        glTranslatef(0, 0, -100);
        drawCylinder(15,100, 36, 36);

        drawSphere3(15, 36, 36);  // head of the cylinder
    }
    glPopMatrix();


    glTranslatef(0, 300, 0);
    glRotatef(90, 1,0,0);
    drawSquare(50);

}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle1,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle1,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle1,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle1,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle1,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle1,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

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
        case 'q':
            if (angle1 < 45)
                angle1 += 1;
            break;
        case 'w':
            if (angle1 > -45)
                angle1 -= 1;
            break;

        case 'e':
            if (angle2 < 45)
                angle2 += 1;
            break;
        case 'r':
            if (angle2 > -45)
                angle2 -= 1;
            break;
        case 'a':
            if (angle3 < 45)
                angle3 += 1;
            break;
        case 's':
            if (angle3 > -45)
                angle3 -= 1;
            break;
        case 'd':
            if (angle4 > -30)
                angle4 -= 1;
            break;
        case 'f':
            if (angle4 < 30)
                angle4 += 1;
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
//	cout << pos.x <<" "<<pos.y<<" "<<pos.z<<" "<<l.x<<" "<<l.y<<" "<<l.z<<" "<<u.x<<" "<<u.y<<" "<<u.z<< " "<<r.x<<" "<<r.y<<" "<<r.z << endl;

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	//drawGrid();

	//drawLine();
    glColor3f(0,1,0);
    //drawSquare(120);

    //drawSS();
    //drawSphere(30,24,20);
    myDraw();


    glColor3f(1,0,0);
    //drawCircle(85, 36);
    //drawBubble(15, 36);
    //drawBubbles();



    //drawCone(20,50,24);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


/*void speedController()
{
    for (int i = 0; i < 3; i++)
    {
        speed[i].x = (i+1)*0.02/5;
        speed[i].y = (i+1)*0.03/5;
    }

    speed[3].x = 0.01;
    speed[3].y = 0.02;

    speed[4].x = 0.02;
    speed[4].y = 0.01;
}*/

void animate(){
	//angle+=0.02;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
	//speedController();
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


int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Task 3");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL



	return 0;
}
