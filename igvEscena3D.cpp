/**
*@authors Juan Antonio Rosell Torres
 *          Daniel Payer Castro
 */
#include <cstdlib>
#include <stdio.h>
#include <cmath>

#include "igvEscena3D.h"

/**
 * Método para pintar los ejes coordenados llamando a funciones de OpenGL
 */
void igvEscena3D::pintar_ejes()
{
    GLfloat rojo[] = {1, 0, 0, 1.0};
    GLfloat verde[] = {0, 1, 0, 1.0};
    GLfloat azul[] = {0, 0, 1, 1.0};

    glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
    glBegin(GL_LINES);
    glVertex3f(1000, 0, 0);
    glVertex3f(-1000, 0, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, verde);
    glBegin(GL_LINES);
    glVertex3f(0, 1000, 0);
    glVertex3f(0, -1000, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, azul);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 1000);
    glVertex3f(0, 0, -1000);
    glEnd();
}

//metodos de transformacion geometrica

void igvEscena3D::trasladar(float x, float y, float z)
{
    transformaciones.tx+=x;
    transformaciones.ty+=y;
    transformaciones.tz+=z;
}

void igvEscena3D::rotar(float x, float y, float z)
{
    transformaciones.rx+=x;
    transformaciones.ry+=y;
    transformaciones.rz+=z;
}

void igvEscena3D::escalar(float factor)
{
    transformaciones.s *= factor;
}

// Métodos públicos
void igvEscena3D::pintaArbol()
{   GLfloat marron[] = {0.5,0.2,0,1};
    GLfloat verde[] = {0,2.0,0,1};

    glMaterialfv(GL_FRONT, GL_EMISSION, marron);

    GLUquadricObj *tubo;
    tubo = gluNewQuadric();
    gluQuadricDrawStyle(tubo,GLU_FILL);

    glPushMatrix();
    glScalef(0.75,1.5,1);
    glRotatef(-90,1,0,0);
    gluCylinder(tubo,0.25,0.25,1,15,15);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_EMISSION, verde);
    glPushMatrix();
    glTranslatef(0,2,0);
    glutSolidSphere(0.75,10,10);
    glPopMatrix();

}

/**
 * Método con las llamadas OpenGL para visualizar la escena
 */
void igvEscena3D::visualizar()
{
    // crear luces
    GLfloat luz0[] = {10, 8, 9, 1}; // luz puntual
    glLightfv(GL_LIGHT0, GL_POSITION, luz0);
    glEnable(GL_LIGHT0);

    // crear el modelo
    glPushMatrix(); // guarda la matriz de modelado

    // se pintan los ejes
    if (ejes)
    {
        pintar_ejes();
    }



    glPushMatrix();

    glTranslatef(transformaciones.tx,transformaciones.ty,transformaciones.tz);

    glScalef(transformaciones.s,transformaciones.s,transformaciones.s);

    glRotatef(transformaciones.rx,1,0,0);
    glRotatef(transformaciones.ry,0,1,0);
    glRotatef(transformaciones.rz,0,0,1);

    pintaArbol();
    glPopMatrix();

    glPopMatrix();
}

/**
 * Método para consultar si hay que dibujar los ejes o no
 * @retval true Si hay que dibujar los ejes
 * @retval false Si no hay que dibujar los ejes
 */
bool igvEscena3D::get_ejes()
{
    return ejes;
}

/**
 * Método para activar o desactivar el dibujado de los ejes
 * @param _ejes Indica si hay que dibujar los ejes (true) o no (false)
 * @post El estado del objeto cambia en lo que respecta al dibujado de ejes,
 *       de acuerdo al valor pasado como parámetro
 */
void igvEscena3D::set_ejes(bool _ejes)
{
    ejes = _ejes;
}
