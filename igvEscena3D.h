#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

struct Transformaciones
{
    float tx = 0, ty = 0, tz = 0; ///< traslacion acumulada
    float rx=0, ry=0, rz = 0; ///< ángulos acumulados (en grados)
    float s=1; ///< escala acumulada
};

/**
 * Los objetos de esta clase representan escenas 3D para su visualización
 */
class igvEscena3D
{
public:
    const int Objeto1 = 1; ///< Identificador interno del objeto 1
    const int Objeto2 = 2; ///< Identificador interno del objeto 2
    const int Objeto3 = 3; ///< Identificador interno del objeto 3

    const char* Nombre_EscenaA = "Escena A"; ///< Etiquetas que aparecen en el menú
    const char* Nombre_EscenaB = "Escena B";
    const char* Nombre_EscenaC = "Escena C";
private:
    // Atributos
    bool ejes = true; ///< Indica si hay que dibujar los _ejes coordenados o no

    Transformaciones obj[3];
    int seleccionado = 0;

public:
    // Constructores por defecto y destructor
    /// Constructor por defecto
    igvEscena3D() = default;
    /// Destructor
    ~igvEscena3D() = default;

    void trasladar(float x, float y, float z);
    void rotar(float drx, float dry, float drz);
    void escalar(float factor);

    // Métodos
    // método con las llamadas OpenGL para visualizar la escena
    void visualizar(int objeto);
    void hacerPrismaRampa(float ancho, float alto, float largo);

    bool get_ejes();

    void set_ejes(bool _ejes);

    void set_seleccionado(int seleccionado);

private:
    void renderObjeto1();
    void renderObjeto2();
    void renderObjeto3();
    void pintar_ejes();
};

#endif   // __IGVESCENA3D
