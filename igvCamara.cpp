#include <math.h>

#include "igvCamara.h"

// Métodos constructores

/**
 * Constructor parametrizado
 * @param _tipo Tipo de cámara (IGV_PARALELA, IGV_FRUSTUM o IGV_PERSPECTIVA)
 * @param _P0 Posición de la cámara (punto de visión)
 * @param _r Punto al que mira la cámara (punto de referencia)
 * @param _V Vector que indica la vertical
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post Los atributos de la nueva cámara serán iguales a los parámetros que se
 *       le pasan
 */
igvCamara::igvCamara ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
   , igvPunto3D _V ): P0 ( _P0 ), r ( _r ), V ( _V )
                      , tipo ( _tipo )
{ }

// Métodos públicos
/**
 * Define la posición de la cámara
 * @param _P0 Posición de la cámara (punto de visión)
 * @param _r Punto al que mira la cámara (punto de referencia)
 * @param _V Vector que indica la vertical
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post Los atributos de la cámara cambian a los valores pasados como parámetro
 */
void igvCamara::set ( igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V )
{  P0 = _P0;
   r  = _r;
   V  = _V;
}

/**
 * Define una cámara de tipo paralela o frustum
 * @param _tipo Tipo de la cámara (IGV_PARALELA o IGV_FRUSTUM)
 * @param _P0 Posición de la cámara
 * @param _r Punto al que mira la cámara
 * @param _V Vector que indica la vertical
 * @param _xwmin Coordenada X mínima del frustum
 * @param _xwmax Coordenada X máxima del frustum
 * @param _ywmin Coordenada Y mínima del frustum
 * @param _ywmax Coordenada Y máxima del frustum
 * @param _znear Distancia de la cámara al plano Z near
 * @param _zfar Distancia de la cámara al plano Z far
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post Los atributos de la cámara cambian a los valores pasados como parámetro
 */
void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
                      , igvPunto3D _V, double _xwmin, double _xwmax, double _ywmin
                      , double _ywmax, double _znear, double _zfar )
{  tipo = _tipo;

   P0 = _P0;
   r = _r;
   V = _V;

   xwmin = _xwmin;
   xwmax = _xwmax;
   ywmin = _ywmin;
   ywmax = _ywmax;
   znear = _znear;
   zfar = _zfar;
}

/**
 * Define una cámara de tipo perspectiva
 * @param _tipo Tipo de la cámara (IGV_PERSPECTIVA)
 * @param _P0 Posición de la cámara
 * @param _r Punto al que mira la cámara
 * @param _V Vector que indica la vertical
 * @param _angulo Ángulo de apertura
 * @param _raspecto Razón de aspecto
 * @param _znear Distancia de la cámara al plano Z near
 * @param _zfar Distancia de la cámara al plano Z far
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post Los atributos de la cámara cambian a los valores que se pasan como
 *       parámetros
 */
void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
                      , igvPunto3D _V, double _angulo, double _raspecto
                      , double _znear, double _zfar )
{  tipo = _tipo;

   P0 = _P0;
   r = _r;
   V = _V;

   angulo = _angulo;
   raspecto = _raspecto;
   znear = _znear;
   zfar = _zfar;
}

/**
 * Aplica a los objetos de la escena la transformación de visión y la
 * transformación de proyección asociadas a los parámetros de la cámara
 */
void igvCamara::aplicar ()
{  glMatrixMode ( GL_PROJECTION );
   glLoadIdentity ();

   if ( tipo == IGV_PARALELA )
   {
      glOrtho ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
   }
   if ( tipo == IGV_FRUSTUM )
   {
      glFrustum ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
   }
   if ( tipo == IGV_PERSPECTIVA )
   {
      gluPerspective ( angulo, raspecto, znear, zfar );
   }

   glMatrixMode ( GL_MODELVIEW );
   glLoadIdentity ();
   gluLookAt ( P0[X], P0[Y], P0[Z], r[X], r[Y], r[Z], V[X], V[Y], V[Z] );
}

/**
 * Realiza un zoom sobre la cámara
 * @param factor Factor (en tanto por 100) que se aplica al zoom. Si el valor es
 *        positivo, se aumenta el zoom. Si es negativo, se reduce.
 * @pre Se asume que el parámetro tiene un valor válido
 */
void igvCamara::zoom ( double factor )
{  // TODO: apartado C
}

bool igvCamara::isCamara() const {
   return camara;
}

//Si es true, las transformaciones mueven la cámara
void igvCamara::setCamara(bool camara) {
   igvCamara::camara = camara;
}

void igvCamara::orbitar(float angulo) {
   //Convertimos el ángulo a radianes
   float rangulo = (angulo * M_PI)/180;

   //Hacemos el módulo del vector PR
   double prx = P0[X] - r[X];
   double prz = P0[Z] - r[Z];
   double modulopr = sqrt((prx*prx)+(prz*prz));

   double radio = modulopr;

   //Calculamos el ángulo actual de la cámara para que no se quede atascada
   double angActual = atan2(prx, prz);
   float nuevoAng = angActual + rangulo;

   P0[X] = r[X] + radio * sin(nuevoAng);
   P0[Z] = r[Z] + radio * sin(nuevoAng);

   //CAMARA
   /*
    * eyex = centerx + radius * sen(ang)
    * eyez = centerz + radius * cos(ang)
    * */


}

void igvCamara::cabeceo(float angulo) {


}

void igvCamara::rotacionY(float angulo) {
   //Convertimos el ángulo a radianes
   float rangulo = (angulo * M_PI)/180;

   //Hacemos el módulo del vector PR
   double prx = P0[X] - r[X];
   double prz = P0[Z] - r[Z];
   double modulopr = sqrt((prx*prx)+(prz*prz));

   double radio = modulopr;

   //Calculamos el ángulo actual de la cámara para que no se quede atascada
   double angActual = atan2(prx, -prz);
   float nuevoAng = angActual + rangulo;

   r[X] = P0[X] + radio * sin(nuevoAng);
   r[Z] = P0[Z] + -radio * sin(nuevoAng);

}


/* VIEWPORT
 * glViewPort(x, y, width, height)
 * (x,y): Esquina inferior izq (normalmente 0,0)
 * width (ancho) y height (largo): dimensiones pixeles
 *
 * -------------------------------
 * |               |              |
 * |               |              |
 * h2------------------------------
 * |               |              |
 * |               |              |
 * |               |              |
 * ---------------w2----------------
 *
 * I1 (abajo izq) = I1(0,0,w2,h2)
 * D1 (abajo derecha) = D1(w2,0,w2,h2)
 * I2 (arriba izq) = I2(0,h2,w2,h2)
 * D2 (arriba der) = D2(w2, h2, w2, h2)
 * mitad izq = I(0,0, w2, height)
 * mitad der = D(w2,0, w2, height)
 *
 * displayFunc(){
 *    glViewPort(...) //I1
 *    instancia -> actualiza_vista();
 *    instancia -> ecena.visualiza();
 *
 *    glViewPort(...) //I2
 * */