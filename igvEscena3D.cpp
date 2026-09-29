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


// Métodos públicos

/**
 * Método con las llamadas OpenGL para visualizar la escena
 */
void igvEscena3D::visualizar(int objeto)
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

    // se pintan los objetos de la escena

    switch (objeto)
    {
    case 1:
        renderObjeto1();
        break;
    case 2:
        renderObjeto2();
        break;
    case 3:
        renderObjeto3();
        break;
    }


    glPopMatrix(); // restaura la matriz de modelado
}

/**
 *
 * @param ancho del triangulo
 * @param alto del prisma
 * @param largo
 */
void igvEscena3D::hacerPrismaRampa(float ancho, float alto, float largo) //largo -> eje z, profundidad
{
    //sacamos las coordenadas
    float x_min = - (ancho / 2.0f);
    float x_max = (ancho / 2.0f);

    float z_min = - (largo / 2.0f);
    float z_max = (largo / 2.0f);

    float y_min = - (alto / 2.0f);
    float y_max = (alto / 2.0f);

    // Calculamos el vector normal de la hipotenusa
    float longitud = std::sqrt(ancho * ancho + alto * alto);
    float nx = alto / longitud;
    float ny = ancho / longitud;

    //Tapas triangulares
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x_min, y_min, z_max);
    glVertex3f(x_min, y_max, z_max);
    glVertex3f(x_max, y_min, z_max);

    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x_min,y_min,z_min);
    glVertex3f(x_min,y_max,z_min);
    glVertex3f(x_max,y_min,z_min);
    glEnd();

    //Tapas rectangulares
    glBegin(GL_QUADS);
    //base
    glNormal3f(0,-1,0);
    glVertex3f(x_min,y_min,z_max);
    glVertex3f(x_min,y_min,z_min);
    glVertex3f(x_max,y_min,z_min);
    glVertex3f(x_max,y_min,z_max);

    //pared izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x_min,y_min,z_max);
    glVertex3f(x_min,y_min,z_min);
    glVertex3f(x_min,y_max,z_min);
    glVertex3f(x_min,y_max,z_max);

    //pared hipotenusa (inclinada)
    glNormal3f(nx, ny, 0.0f);
    glVertex3f(x_max,y_min,z_min);
    glVertex3f(x_max,y_min,z_max);
    glVertex3f(x_min,y_max,z_max);
    glVertex3f(x_min,y_max,z_min);
    glEnd();

}

/**
 * @brief funcion
 */
void igvEscena3D::renderObjeto1() //todo DUDA: glutSwapBuffers() y glFlush() QUE HACEN Y DIFERENCIAS
{   GLfloat gris[] = {0.2, 0.2, 0.2}; //< vector de color RGB (se puede poner 4 elemento para transparencia)
    GLfloat gris_oscuro[] = {0.01,0.01,0.01};

    glMaterialfv(GL_FRONT, GL_EMISSION, gris);

    //------------cuerpo------------------
    glPushMatrix();

    glScalef(1,0.5,3);
    glutSolidCube(1);

    glPopMatrix();

    // ------ prismas superiores--------
    //      (Escalado -> Rotacion -> Traslacion)  [abajo - arriba]

    //maletero
    glMaterialfv(GL_FRONT, GL_EMISSION, gris_oscuro);

    glPushMatrix();

    glTranslatef(0,0.5,-0.5);
    glRotatef(90,0,1,0);
    hacerPrismaRampa(2,0.5,1);

    glPopMatrix();

    //frente
    glMaterialfv(GL_FRONT, GL_EMISSION, gris);

    glPushMatrix();

    glTranslatef(0,0.5,1);
    glRotatef(-90,0,1,0);
    hacerPrismaRampa(1,0.5,1);

    glPopMatrix();

}

void igvEscena3D::renderObjeto2()
{
    glutSolidCube(2);
}

void igvEscena3D::renderObjeto3()
{
    glutSolidCube(3);
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
