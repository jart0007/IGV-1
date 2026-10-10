/**
*@authors Juan Antonio Rosell Torres
 *          Daniel Payer Castro
 */
#include <math.h>

#include "igvCamara.h"


/* Notas
 *
 */

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
 * rota p0 alrededor del objeto
 * @param num grado de rotacion (hay que pasarlo a radianes)
 * @note la formula es P' = r+Ry*(P-r)
 *    donde P' es el nuevo punto, P el antiguo, r el punto de referencia y R la matriz de Rotación
 *
*    ---orbita
 * posCamarax = centerx + radio * sin
 * posCamaraz = centerz + radio * cos
 */
void igvCamara::orbita(double num)
{
   double dx = P0[X] - r[X];
   double dz = P0[Z] - r[Z];

   double ang_ant = atan2(dx,dz);

   double rad = num * (M_PI / 180);
   double nuevo_ang = ang_ant + rad;

   double radio = sqrt(dx*dx + dz*dz);

   P0[X] = r[X] + radio*sin(nuevo_ang);
   P0[Z] = r[Z] + radio*cos(nuevo_ang);
}


/**
 * P0 fijo; la dirección gira a izquierda o derecha
 * @param num grado de rotacion (hay que pasarlo a radianes)
 * @note la formula es r' = P + Ry*(r-P)
 *    donde P' es el nuevo punto, P el antiguo, r el punto de referencia y Ru la matriz de Rotación
 */
void igvCamara::rotacionY(double num)
{
   double rad = num * (M_PI / 180.0);

   double dx = r[X] - P0[X];
   double dy = r[Y] - P0[Y];
   double dz = r[Z] - P0[Z];

   double n_dx = dx * cos(rad) + dz * sin(rad);
   double n_dy = dy;
   double n_dz = (-dx*sin(rad)) + dz * cos(rad);

   r[X] = P0[X] + n_dx;
   r[Y] = P0[Y] + n_dy;
   r[Z] = P0[Z] + n_dz;
}

/**
 *
 * Vdist = r - p0
 *
 * r = p0 + vDist
 *
 * r = p0 + Ru(r-p0)      ///< "u" es el eje derecha de la camara, se consigue con el x y z
 */
void igvCamara::cabeceo(double dist)
{  double rad = dist * (M_PI / 180.0);

   double dx = r[X] - P0[X];
   double dz = r[Z] - P0[Z];
   double dy = r[Y] - P0[Y];

   double dxz = sqrt(dx*dx + dz*dz); //eje derecha

   double dxz_nuevo = dxz * cos(rad) - dy * sin(rad);
   double dy_nuevo = dxz * sin(rad) + dy * cos(rad);

   double escala = dxz_nuevo/dxz;

   if ((dxz_nuevo < IGV_EPSILON)) return;
   r[X] = P0[X] +dx*escala;
   r[Y] = P0[Y] + dy_nuevo;
   r[Z] = P0[Z] + dz*escala;

}

void igvCamara::moverZnear(double num)
{
   znear += num;
}

void igvCamara::moverZfar(double num)
{
   zfar += num;
}

/**
 * Realiza un zoom sobre la cámara
 * @param factor Factor (en tanto por 100) que se aplica al zoom. Si el valor es
 *        positivo, se aumenta el zoom. Si es negativo, se reduce.
 * @pre Se asume que el parámetro tiene un valor válido
 */
void igvCamara::zoom ( double factor )
{
   if (tipo == IGV_PARALELA)
   {
      xwmin *= factor;
      xwmax *= factor;
      ywmin *= factor;
      ywmax *= factor;
   }
   else if (tipo == IGV_PERSPECTIVA)
   {
      angulo*=factor;

      if (angulo < 1.0)   angulo = 1.0;
      if (angulo > 179.0) angulo = 179.0;
   }
}
