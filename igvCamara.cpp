#include <math.h>

#include "igvCamara.h"


/* Notas
 * vector =  fin - inicio --> fin = vector + inicio
 *
 *    objetivo = posCamara + vDir
 *
 * ---Pan (rotacionY)
 * x= sin --> dirx = sin(pan)
 * z= cos --> dirz = -cos(pan)
 *
 *    dir = (sin(pan),-cos(pan))
 *
 * ---Pan + Tilt (cabeceo?)
 * dirx = cos(tilt) * sin(pan)
 * diry = sin(tilt)
 * dirz = -cos(tilt) * cos(pan)
 *
 * ---orbita
 * posCamarax = centerx + radio * sin
 * posCamaraz = centerz + radio * cos
 *
 *
 *
 * -----------VIEWPORTS
 *
 * glViewPort(x,y,width,height)
 *
 *    (x,y)          -  Esquina inferior izquierda
 *    width,height   -  Dimensiones en pixeles
 *
 *    (en un examen se suele pedir que parta la pantalla o un viewport en alguna de las 4 esquinas)
 *
 *
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
 * inclina hacia arriba o abajo sobre el eje local "x" de la camara, lo llamamos eje "u" o  eje derecha
 * @param num grado de rotacion (hay que pasarlo a radianes)
 * @note la formula es r' = p0+Ru*(r-P)
 *    donde P' es el nuevo punto, P el antiguo, r el punto de referencia y Ru la matriz de Rotación
 *
*    ---Pan (cabeceo)
 * x= sin --> dirx = sin(pan)
 * z= cos --> dirz = -cos(pan)
 *
 *    dir = (sin(pan),-cos(pan))
 */
void igvCamara::cabeceo(double num)
{
   double rad = num * (M_PI / 180.0);

   // 1. Vector de visión 3D completo
   double dx = r[X] - P0[X];
   double dy = r[Y] - P0[Y];
   double dz = r[Z] - P0[Z];

   // 2. Distancia horizontal en el plano XZ y radio 3D total
   double dxz = sqrt(dx * dx + dz * dz); /// calculo del eje x local
   double radio = sqrt(dxz * dxz + dy * dy);

   // 3. Ángulo de elevación actual (Pitch) respecto al plano horizontal
   double ang_ant = atan2(dy, dxz);
   double ang_nuevo = ang_ant + rad;

   // 4. Nueva altura (Y) y nueva proyección horizontal (R_xz)
   double dy_nuevo = radio * sin(ang_nuevo);
   double dxz_nuevo = radio * cos(ang_nuevo);

   // 5. Mantenemos la dirección horizontal original escalando X y Z proporcionalmente
   // (evita que la cámara se desvíe a los lados)
   double factor_escala = dxz_nuevo / dxz;

   if (!(dxz_nuevo < IGV_EPSILON)) //otra forma de hacer la comparacion: (angulo_nuevo > (89.0*M_PI/180)) y lo mismo para menor que. Para que no pase de 90 grados en vertical el cabeceo de la camara
   {
      r[X] = P0[X] + dx * factor_escala;
      r[Y] = P0[Y] + dy_nuevo;
      r[Z] = P0[Z] + dz * factor_escala;
   }///< el if sirve para que si la camara mira en vertical, no se aplica
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

   // 1. Vector de visión d = r - P0
   double dx = r[X] - P0[X];
   double dz = r[Z] - P0[Z];

   double radio = sqrt(dx * dx + dz * dz);
   double ang_act = atan2(dx, -dz); // Ángulo  respecto al eje -Z
   double ang_nuevo = ang_act + rad;

   // dirx = radio * sin(pan), dirz = -radio * cos(pan)

   // objetivo = posCamara + vDir
   r[X] = P0[X] + radio * sin(ang_nuevo);
   r[Z] = P0[Z] + -radio * cos(ang_nuevo);
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
