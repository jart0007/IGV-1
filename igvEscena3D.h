/**
*@authors Juan Antonio Rosell Torres
 *          Daniel Payer Castro
 */
#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

struct transformaciones
{
    float tx=0,ty=0,tz=0;
    float rx=0, ry=0, rz=0;
    float s=1; ///< factor de escalado homogeneo
};

/**
 * Los objetos de esta clase representan escenas 3D para su visualización
 */
class igvEscena3D
{
public:

private:
    // Atributos
    bool ejes = true; ///< Indica si hay que dibujar los _ejes coordenados o no
    transformaciones transformaciones;


public:
    // Constructores por defecto y destructor
    /// Constructor por defecto
    igvEscena3D() = default;
    /// Destructor
    ~igvEscena3D() = default;




    // método con las llamadas OpenGL para visualizar la escena
    void visualizar();

    bool get_ejes();
    void set_ejes(bool _ejes);
    void trasladar(float x, float y, float z);
    void rotar(float x, float y, float z);
    void escalar(float factor);



private:
    void pintar_ejes();
    void pintaArbol();
};

#endif   // __IGVESCENA3D
