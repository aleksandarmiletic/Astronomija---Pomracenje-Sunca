#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif
#include<GL/glut.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

const double RADIAN = 0.01745329251994329576923690768489;
const double koef = 3.0;
static int year = 0, day = 0, moon = 0; // angle move

double x_moon, y_moon, z_moon;
double x_earth, y_earth, z_earth;
double x_sun = 0.0, y_sun = 0.0, z_sun = 0.0;

double sun_radius = 0.35; // 0.10
double earth_radius = 0.077; //0.087
double moon_radius = 0.04; //0.027
double zoom_angle = 60; //zoom angle 76.2
double default_zoom_angle = 76.2;
double n; // tmp
double zoom_koef = 1.0; //ZOOM

double az=2.0; //distance from Earth to Sun
int vz=0;
double vm;
double am=0.2; //distance from Moon to Earth
double relation=365.25636/27.321662;
double omega;
double n_omega=1.0;
double i_angle=30.0; // Moon tilt
int t = 0;
int s = 0;
int m = 0;

double num1 = 5.0;  // SHADOW KOEF.
double num2 = 5.0; // SHADOW KOEF.

void earth_moon(void);
void earth_sun(void);
void sun_default(void);
void moon_default(void);
void earth_default(void);
void init(void);
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);


int main(int argc, char **argv)
{
    printf("Unesite poluprecnik Meseca! Preporuceno 0.001-0.99. -1 za ugradjenu vrednost. \n");
    scanf("%lf", &n);
    if(n==-1) moon_default();
    else moon_radius = n;

    printf("Unesite poluprecnik Zemlje! Preporuceno 0.001-0.99. -1 za ugradjenu vrednost. \n");
    scanf("%lf", &n);
    if(n==-1) earth_default();
    else earth_radius = n;

    printf("Unesite poluprecnik Sunca! Preporuceno 0.001-0.99. -1 za ugradjenu vrednost. \n");
    scanf("%lf", &n);
    if(n==-1) sun_default();
    else sun_radius = n;

    printf("Unesite rastojanje od Zemlje do Sunca! Preporuceno 0.5-9. -1 za ugradjenu vrednost. \n");
    scanf("%lf", &n);
    if(n==-1) earth_sun();
    else az = n;

    printf("Unesite rastojanje od Meseca do Zemlje! Preporuceno 0.05-2. -1 za ugradjenu vrednost. \n");
    scanf("%lf", &n);
    if(n==-1) earth_moon();
    else am = n;

    printf("Camera: Normalno - Odozgo, ravan x-z. \n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(750,650);
    glutInitWindowPosition(280, 50);
    //glutFullScreen();
    glutCreateWindow("Pomracenje Sunca");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
void earth_sun(void)
{
    double az = 2.0;
}
void earth_moon(void)
{
    double am = 0.2;
}
void sun_default(void)
{
    double sun_radius = 0.35;
}
void moon_default(void)
{
    double moon_radius = 0.04;
}
void earth_default(void)
{
    double earth_radius = 0.077;
}
void init(void)
{
    GLfloat mat_specular[] = {0.0,0.0,0.0,1.0};
    GLfloat mat_shininess[] = {100.0};

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glMaterialfv (GL_FRONT ,GL_SPECULAR, mat_specular);
    glMaterialfv (GL_FRONT ,GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
}
void display(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(!t)
        gluLookAt(0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    else
        gluLookAt(0.0, -9.0, 0.0, 0.0, 0.0, 0.0, 0.0, -9.0, -9.0);

    glMatrixMode(GL_PROJECTION);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);

    // SUN
    glPushMatrix();
        glColor4f(2.0,2.0,0.0, 1.0);  //glColor3ub(237,247,60);
        if(!t)
            glRotatef((GLfloat) year, 0.0, 0.0, 1.0);

        if(!t)
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);

            glutSolidSphere(zoom_koef*sun_radius,20,16);
        }
        else
        {
            glutWireSphere(zoom_koef*sun_radius,20,16);
        }
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    if(t) //PROJECTION View
    {
    //SUN LIGHT
    glPushMatrix();
       // glRotatef((GLfloat) year, 0.0, 1.0, 0.0);
        glColor4f(0.0,1.75,5.0, 0.5);
        //glRotatef(90.0, 1.0, 0.0, 0.0);
        if(!s)
        {
            glBegin(GL_TRIANGLES);
                glVertex3f(0.0,0.0,0.0-sun_radius*zoom_koef);
                glVertex3f(0.0,0.0,0.0+sun_radius*zoom_koef);
                glVertex3f(-x_earth*zoom_koef,-y_earth*zoom_koef, z_earth*zoom_koef);
            glEnd();
        }
    glPopMatrix();
    }

    //Earth
    glPushMatrix();
        glColor4f(0.0,0.5,5.0, 0.5);  //glColor3ub(2,97,202);
        if(!t)
        {
            x_earth = az*cos(RADIAN*vz);
            y_earth = az*sin(RADIAN*vz);
            z_earth = 0.0;
        }
        else //PROJECTION View
        {
            x_earth = az*cos(RADIAN*-vz);
            y_earth = az*sin(RADIAN*-vz);
            z_earth = 0.0;
        }

        if(!t)
            glTranslatef(zoom_koef*x_earth, zoom_koef*y_earth, zoom_koef*z_earth);
        else
            glTranslatef(zoom_koef*x_earth, zoom_koef*z_earth, zoom_koef*y_earth);

        if(!t)
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);

            glutSolidSphere(zoom_koef*earth_radius,10,8);
        }
        else
        {
            glutWireSphere(zoom_koef*earth_radius,10,8);
        }

    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Coordinates!!!!!!!!!!!!!
    // Coordinates!!!!!!!!!!!!!

    double a_x = sun_radius*cos(RADIAN*(vz+90)) *zoom_koef;
    double a_y = sun_radius*sin(RADIAN*(vz+90)) *zoom_koef;
    double b_x = sun_radius*cos(RADIAN*(vz-90)) *zoom_koef;
    double b_y = sun_radius*sin(RADIAN*(vz-90)) *zoom_koef;


    if(!t) //NORMAL VIEW  //SUN LIGHT
    {
       glPushMatrix();
            //glRotatef((GLfloat) vz, 0.0, 0.0, 1.0);
            glColor4f(0.0,1.75,5.0, 0.5);
            //glRotatef(90.0, 1.0, 0.0, 0.0);

            if(!s)
            {
                glBegin(GL_TRIANGLES);

                    glVertex3f(a_x,a_y,0.0);
                    glVertex3f(b_x,b_y,0.0);
                    glVertex3f(x_earth *zoom_koef,y_earth *zoom_koef, 0.0);

                glEnd();
            }
        glPopMatrix();

    }
    //MOON
    glPushMatrix();
        glColor4f(2.0,2.0,2.0, 0.6);  //glColor3ub(211,208,215);

        vm = vz/1.0;
        omega = n_omega*vz;

        if(!t)
        {
            x_moon = am*cos(2*vm*RADIAN);
            y_moon = am*sin(2*vm*RADIAN);
            z_moon = 0.0;
        }
        else
        {
            x_moon = am*(cos(omega*RADIAN)*cos(vm*RADIAN)-sin(omega*RADIAN)*cos(i_angle*RADIAN)*sin(vm*RADIAN));
            y_moon = am*sin(i_angle*RADIAN)*sin(vm*RADIAN);
            z_moon = am*(sin(omega*RADIAN)*cos(vm*RADIAN)-cos(omega*RADIAN)*cos(i_angle*RADIAN)*sin(vm*RADIAN));
        }

        glTranslatef(zoom_koef*x_moon, zoom_koef*y_moon, zoom_koef*z_moon);

        if(!t)
            glTranslatef(zoom_koef*x_earth, zoom_koef*y_earth, zoom_koef*z_earth);
        else
            glTranslatef(zoom_koef*x_earth, zoom_koef*z_earth, zoom_koef*y_earth);


        if(!t)
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);

            glutSolidSphere(zoom_koef*moon_radius,10,5);
        }
        else
        {
            glutWireSphere(zoom_koef*moon_radius,10,5);
        }

    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Coordinates!!!!!!!!!!!!!
    // Coordinates!!!!!!!!!!!!!

    double g_x = 0.0;    //SUN
    double g_z = sun_radius;
    g_z *= zoom_koef;
    double h_x = 0.0;
    double h_z = 0.0 - sun_radius; //SUN
    h_z *= zoom_koef;

    double i_x = -x_moon - x_earth; //MOON
    i_x *= zoom_koef;
    double i_z = -y_moon + moon_radius + z_earth;
    i_z *= zoom_koef;
    double j_x = -x_moon - x_earth; //MOON
    j_x *= zoom_koef;
    double j_z = -y_moon - moon_radius + z_earth;
    j_z *= zoom_koef;

    double s1 = (h_x - g_x)*(i_z - h_z) - (h_z - g_z)*(i_x - h_x);
    double s2 = (j_x - g_x)*(i_z - h_z) - (j_z - g_z)*(i_x - h_x);
    double s0 = s1/s2;

    double f_x = g_x + s0*(j_x - g_x);  // Interception Point
    double f_z = g_z + s0*(j_z - g_z);

    double i1_x = f_x + num2*(i_x - f_x);
    double i1_z = f_z + num2*(i_z - f_z);

    double j1_x = f_x + num2*(j_x - f_x);
    double j1_z = f_z + num2*(j_z - f_z);

    if(t) //MOON LIGHT //PROJECTION View
    {
        if(!m)
         {
             glPushMatrix();
                glBegin(GL_TRIANGLES);
                glColor4f(1.0,0.6,0.2, 0.5);
                glVertex3f(g_x, 0.0, g_z);
                glVertex3f(h_x, 0.0, h_z);
                glVertex3f(f_x, 0.0, f_z);

                glEnd();
            glPopMatrix();

            glPushMatrix();
                glBegin(GL_TRIANGLES);
                glColor4f(1.0,0.6,0.2, 0.5);
                glVertex3f(i1_x, 0.0, i1_z);
                glVertex3f(j1_x, 0.0, j1_z);
                glVertex3f(f_x, 0.0, f_z);

                glEnd();
            glPopMatrix();
         }
    }

    // Coordinates!!!!!!!!!!!!!
    // Coordinates!!!!!!!!!!!!!

    double c_x = moon_radius*cos(RADIAN*(vz+90)) + x_moon + x_earth;
    c_x *=  zoom_koef;
    double c_y = moon_radius*sin(RADIAN*(vz+90)) + y_moon + y_earth;
    c_y *= zoom_koef;
    double d_x = moon_radius*cos(RADIAN*(vz-90)) + x_moon + x_earth;
    d_x *= zoom_koef;
    double d_y = moon_radius*sin(RADIAN*(vz-90)) + y_moon + y_earth;
    d_y *= zoom_koef;

    double t1 = (b_x-a_x)*(c_y-b_y) - (b_y - a_y)*(c_x - b_x);
    double t2 = (d_x - a_x)*(c_y - b_y) - (d_y - a_y)*(c_x - b_x);
    double t0 = t1/t2;

    double e_x = a_x + t0*(d_x - a_x); // Interception Point
    double e_y = a_y + t0*(d_y - a_y);


    if(!t) // NORMAL VIEW //MOON LIGHT
    {
        if(!m)
        {
            glPushMatrix();


            glBegin(GL_TRIANGLES);
                glColor4f(1.0,0.6,0.2, 0.5);
                glVertex3f(a_x,a_y,0.0);
                glVertex3f(b_x,b_y,0.0);
                glVertex3f(e_x,e_y, 0.0);

            glEnd();
            glPopMatrix();

            glPushMatrix();
                double c1_x = e_x + num1*(c_x - e_x);
                double c1_y = e_y + num1*(c_y - e_y);

                double d1_x = e_x + num1*(d_x - e_x);
                double d1_y = e_y + num1*(d_y - e_y);

                glBegin(GL_TRIANGLES);
                    glColor4f(1.0,0.6,0.2, 0.5);
                    glVertex3f(c1_x,c1_y,0.0);
                    glVertex3f(d1_x,d1_y,0.0);
                    glVertex3f(e_x,e_y, 0.0);

                glEnd();

            glPopMatrix();
        }
    }

    glutSwapBuffers();

    glFlush();
}

    void reshape(int w, int h)
    {
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //gluPerspective(zoom_angle, ((GLfloat) w)/((GLfloat) h), 1.0, 20.0);
        double view = 2.6;
        glOrtho(-view,view,-view*(GLfloat)h/(GLfloat)w,view*(GLfloat)h/(GLfloat)w,-100.0,100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }


    void keyboard(unsigned char key, int x, int y)
    {
        switch (key)
        {
            case 'a':
                vz = (vz + 1) % 360;
                year = vz; //rotacija
                glutPostRedisplay();
                break;
            case 'd':
                vz = (vz - 1) % 360;
                year = vz; //rotacija
                glutPostRedisplay();
                break;
            case 't':
                t = !t;
                if(!t)
                    printf("Camera: Normalno - Odozgo, ravan x-z.\n");
                else
                    printf("Camera: Projekcija na ravan x-y.\n");
                glutPostRedisplay();
                break;
            case 'p':
                {
                    double x11_earth = az*cos(RADIAN*-vz);
                    double y11_earth = az*sin(RADIAN*-vz);
                    double z11_earth = 0.0;

                    double x11_moon = am*(cos(omega*RADIAN)*cos(vm*RADIAN)-sin(omega*RADIAN)*cos(i_angle*RADIAN)*sin(vm*RADIAN));
                    double y11_moon = am*sin(i_angle*RADIAN)*sin(vm*RADIAN);
                    double z11_moon = am*(sin(omega*RADIAN)*cos(vm*RADIAN)-cos(omega*RADIAN)*cos(i_angle*RADIAN)*sin(vm*RADIAN));

                    printf("Koordinate Sunca: (0.0, 0.0, 0.0) \n");
                    printf("Koordinate Zemlje: (%lf, %.3lf, %lf) \n", x11_earth, y11_earth, z11_earth);
                    printf("Koordinate Meseca: (%lf, %lf, %lf) \n", x11_moon, y11_moon, z11_moon);
                }
                break;
            case 's':
                s = !s;
                glutPostRedisplay();
                break;
            case 'm':
                m = !m;
                glutPostRedisplay();
                break;
            case '+':
                zoom_koef+=0.05; //zoom
                glutPostRedisplay();
                break;
            case '-':
                zoom_koef-=0.05; //zoom
                glutPostRedisplay();
                break;
            case 27:
                exit(0);
                break;
            default:
                break;
        }
    }
