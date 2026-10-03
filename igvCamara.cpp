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
 * rota p0 alrededor del objeto
 * @param num grado de rotacion (hay que pasarlo a radianes)
 * @note la formula es P' = r+R*(P-r)
 *    donde P' es el nuevo punto, P el antiguo, r el punto de referencia y R la matriz de Rotación
 */
void igvCamara::orbita(double num)
{
   //1º convertir el angulo en radianes
   double rad = num * (M_PI / 180);

   //2º calcular vector rP (P-r) --> lo llamaremos "d" (vector de distancia opuesto a el normal)
   double dx = P0[X] - r[X];
   double dy = P0[Y] - r[Y];
   double dz = P0[Z] - r[Z];

   //3º multiplicamos por la matriz de Rotacion en y

   double matdx = cos(rad) * dx + sin(rad) * dz;
   double matdy = dy;
   double matdz = -sin(rad) * dx + cos(rad) * dz;

   //4º sumarle de nuevo la posicion para dejar la camara en su sitio (+r)
   P0[X] = matdx + r[X];
   P0[Y] = matdy + r[Y];
   P0[Z] = matdz + r[Z];
}

/**
 * inclina hacia arriba o abajo sobre el eje local "x" de la camara, lo llamamos eje "u" o  eje derecha
 * @param num grado de rotacion (hay que pasarlo a radianes)
 * @note la formula es r' = p0+Ru*(r-P)
 *    donde P' es el nuevo punto, P el antiguo, r el punto de referencia y Ru la matriz de Rotación
 */
void igvCamara::cabeceo(double num)
{
   // 1º calculo el angulo en radianes
   double rad = num * (M_PI / 180);

   // 2º calculo (r-P) (vector distancia, esta vez al reves, al igual que va a pasar en rotacionY)
   double dx = r[X] - P0[X];
   double dy = r[Y] - P0[Y];
   double dz = r[Z] - P0[Z];

   // 3. Eje "derecha" local u = d x V (Producto vectorial)
   double ux = dy * V[Z] - dz * V[Y];
   double uy = dz * V[X] - dx * V[Z];
   double uz = dx * V[Y] - dy * V[X];

   // --- PROTECCIÓN DE SINGULARIDAD (Aviso del PDF) ---
   // El módulo de u es |d||V|sin(theta). Si tiende a 0, d y V son casi paralelos (+-90º)
   double mod_u = sqrt(ux * ux + uy * uy + uz * uz);
   if (mod_u < 0.5) {
      return; // Detiene el cabeceo si se acerca demasiado a la vertical
   }

   // Normalizar u
   ux /= mod_u;
   uy /= mod_u;
   uz /= mod_u;

   // 4º Vector "arriba local" w = u x d (segundo eje del plano 2D local)
   double wx = uy * dz - uz * dy;
   double wy = uz * dx - ux * dz;
   double wz = ux * dy - uy * dx;

   // 5º Calcular la dirección PROPUESTA: d' = d * cos(rad) + w * sin(rad)
   double dx_rot = dx * cos(rad) + wx * sin(rad);
   double dy_rot = dy * cos(rad) + wy * sin(rad);
   double dz_rot = dz * cos(rad) + wz * sin(rad);

   // 6º VALIDACIÓN PREVENTIVA: Comprobar el eje u_prop que resultaría del giro
   double u_propx = dy_rot * V[Z] - dz_rot * V[Y];
   double u_propy = dz_rot * V[X] - dx_rot * V[Z];
   double u_propz = dx_rot * V[Y] - dy_rot * V[X];

   double mod_u_prop = sqrt(u_propx * u_propx + u_propy * u_propy + u_propz * u_propz);

   // Si el giro propuesto se acerca demasiado a la vertical, se rechaza
   if (mod_u_prop < 0.5) {
      return; // No modifica r, impidiendo entrar en la zona muerta
   }

   // 6º Actualizar el punto de mira r sumando el pivote P0: r = P0 + d'
   r[X] = P0[X] + dx_rot;
   r[Y] = P0[Y] + dy_rot;
   r[Z] = P0[Z] + dz_rot;

}

/**
 * P0 fijo; la dirección gira a izquierda o derecha
 * @param num grado de rotacion (hay que pasarlo a radianes)
 * @note la formula es r' = P + Ry*(r-P)
 *    donde P' es el nuevo punto, P el antiguo, r el punto de referencia y Ru la matriz de Rotación
 */
void igvCamara::rotacionY(double num)
{
   // 1º calcular el angulo en radianes
   double rad = num * (M_PI / 180);

   //2º calculamos (r - P)
   double dx = r[X] - P0[X];
   double dy = r[Y] - P0[Y];
   double dz = r[Z] - P0[Z];

   // 3º multiplicamos por la matriz de Rotacion en Y
   double matdx = cos(rad) * dx + sin(rad) * dz;
   double matdy = dy;
   double matdz = -sin(rad) * dx + cos(rad) * dz;

   //4º sumarle de nuevo la posicion
   r[X] = P0[X] + matdx;
   r[Y] = P0[Y] + matdy;
   r[Z] = P0[Z] + matdz;
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
