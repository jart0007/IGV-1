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

void igvEscena3D::trasladar(float x, float y, float z)
{
    obj[seleccionado].tx+=x;
    obj[seleccionado].ty+=y;
    obj[seleccionado].tz+=z;
}

void igvEscena3D::rotar(float drx, float dry, float drz)
{
    obj[seleccionado].rx+=drx;
    obj[seleccionado].ry+=dry;
    obj[seleccionado].rz+=drz;
}

void igvEscena3D::escalar(float factor)
{
    obj[seleccionado].s*=factor;
}



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

    /* ----------- PINTAR OBJETOS --------------
     *
     * 1. hago un bucle simple de 3 iteraciones para no tener que pintar los objetos uno a uno
     *
     * 2. cada objeto acumula sus propias iteraciones y se les aplica en orden inverso de: ROTACION --> ESCALADA --> TRASLACION
     *
     * 3. no olvidar el push y pop matrix para no liarla
     */

    for (int i=0; i<3; i++)
    {
        glPushMatrix();

        //traslaciones
        glTranslatef(obj[i].tx, obj[i].ty, obj[i].tz);

        //escalados (es uniforme, asi se explica en el ejemplo del pdf)
        glScalef(obj[i].s, obj[i].s, obj[i].s);

        //rotaciones
        glRotatef(obj[i].rx,1,0,0);
        glRotatef(obj[i].ry,0,1,0);
        glRotatef(obj[i].rz,0,0,1);

        //solo queda dibujar el objeto y el pop()

        switch (i)
        {
            case 0: renderObjeto1(); break;
            case 1: renderObjeto2(); break;
            case 2: renderObjeto3(); break;
        }

        glPopMatrix();
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
    //Color/255
    GLfloat marron[] {0.57, 0.27, 0};

    //Tabla
    glMaterialfv(GL_FRONT,GL_EMISSION, marron);
    glPushMatrix();

    glTranslatef(0,1.5,0);
    glScalef(2,0.25,1.5);
    glutSolidCube(1);

    glPopMatrix();


    //Patas
    glPushMatrix();
    glTranslatef(-0.875,0.75,0.625);  //se mueve el centro del objeto
    glScalef(0.25,1.5,0.25);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-0.875,0.75,-0.625);
    glScalef(0.25,1.5,0.25);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.875,0.75,0.625);
    glScalef(0.25,1.5,0.25);
    glutSolidCube(1);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0.875,0.75,-0.625);
    glScalef(0.25,1.5,0.25);
    glutSolidCube(1);
    glPopMatrix();
}

void igvEscena3D::renderObjeto3()
{
    GLfloat cono[] = {1,0.639,0};
    glMaterialfv(GL_FRONT,GL_EMISSION, cono);
    glPushMatrix();
    glTranslatef(0,1,0);
    glRotatef(90,1,0,0);
    glutSolidCone(0.5,2,20,1); //radio, altura
    glPopMatrix();


    GLfloat bola1[] = {0.459,0.302,0.016};
    glMaterialfv(GL_FRONT,GL_EMISSION, bola1);
    glPushMatrix();
    glTranslatef(-0.125,1.1875,0);
    glutSolidSphere(0.375,20,20); //radio
    glPopMatrix();


    GLfloat bola2[] = {1,0.949,0.863};
    glMaterialfv(GL_FRONT,GL_EMISSION, bola2);
    glPushMatrix();
    glTranslatef(0.125,1.1875,0);
    glutSolidSphere(0.375,20,20); //radio
    glPopMatrix();


    GLfloat bola3[] = {1,0,0};
    glMaterialfv(GL_FRONT,GL_EMISSION, bola3);
    glPushMatrix();
    glTranslatef(0,1.5625,0);
    glutSolidSphere(0.375,20,20); //radio
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

void igvEscena3D::set_seleccionado(const int seleccionado)
{
    this->seleccionado = seleccionado;
}
