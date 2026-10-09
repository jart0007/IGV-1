#include <cstdlib>
#include <stdio.h>
#include "igvInterfaz.h"

// Aplicación del patrón Singleton
igvInterfaz* igvInterfaz::_instancia = nullptr;


// Métodos públicos ----------------------------------------

/**
 * Método para acceder al objeto único de la clase, en aplicación del patrón de
 * diseño Singleton
 * @return Una referencia al objeto único de la clase
 */
igvInterfaz& igvInterfaz::getInstancia()
{
    if (!_instancia)
    {
        _instancia = new igvInterfaz;
    }

    return *_instancia;
}

/**
 * Crea el mundo que se visualiza en la ventana
 */
void igvInterfaz::crear_mundo()
{
    // r tiene valor por defecto (0,0,0)
    // crear cámaras

    //camara general principal
    p0 = igvPunto3D(3.0, 2.0, 4);
    r = igvPunto3D(0, 0, 0);
    V = igvPunto3D(0, 1.0, 0);

    _instancia->camara.set(IGV_PARALELA, p0, r, V, -1 * 3, 1 * 3, -1 * 3, 1 * 3, 1, 200);

    //Camara de perspectiva de planta
    p0p = igvPunto3D(0, 5, 0);
    rp = igvPunto3D(0, 0, 0);
    Vp = igvPunto3D(0, 0, -1);

    _instancia->camaraPlanta.set(IGV_PARALELA, p0p, rp, Vp, -1 * 3, 1 * 3, -1 * 3, 1 * 3, 1, 200);

    // Las cámaras se han creado con valores por defecto de 60 grados de apertura
    // y ratio de aspecto 1
}

/**
 * Inicializa todos los parámetros para crear una ventana de visualización
 * @param argc Número de parámetros por línea de comandos al ejecutar la
 *             aplicación
 * @param argv Parámetros por línea de comandos al ejecutar la aplicación
 * @param _ancho_ventana Ancho inicial de la ventana de visualización
 * @param _alto_ventana Alto inicial de la ventana de visualización
 * @param _pos_X Coordenada X de la posición inicial de la ventana de
 *               visualización
 * @param _pos_Y Coordenada Y de la posición inicial de la ventana de
 *               visualización
 * @param _titulo Título de la ventana de visualización
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post Cambia el alto y ancho de ventana almacenado en el objeto
 */
void igvInterfaz::configura_entorno(int argc, char** argv, int _ancho_ventana
                                    , int _alto_ventana, int _pos_X, int _pos_Y
                                    , std::string _titulo)
{
    // inicialización de los atributos de la interfaz
    ancho_ventana = _ancho_ventana;
    alto_ventana = _alto_ventana;

    // inicialización de la ventana de visualización
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_ancho_ventana, _alto_ventana);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_titulo.c_str());

    glEnable(GL_DEPTH_TEST); // activa el ocultamiento de superficies por z-buffer
    glClearColor(1.0, 1.0, 1.0, 0.0); // establece el color de fondo de la ventana

    glEnable(GL_LIGHTING); // activa la iluminacion de la escena
    glEnable(GL_NORMALIZE); // normaliza los vectores normales para calculo iluminacion

    crear_mundo(); // crea el mundo a visualizar en la ventana
}


/**
 * Método para visualizar la escena y esperar a eventos sobre la interfaz
 */
void igvInterfaz::inicia_bucle_visualizacion()
{
    glutMainLoop(); // inicia el bucle de visualización de GLUT
}

/**
 * Método para control de eventos del teclado
 * @param key Código de la tecla pulsada
 * @param x Coordenada X de la posición del cursor del ratón en el momento del
 *          evento de teclado
 * @param y Coordenada Y de la posición del cursor del ratón en el momento del
 *          evento de teclado
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post Los atributos de la clase pueden cambiar, dependiendo de la tecla pulsada
 */
void igvInterfaz::keyboardFunc(unsigned char key, int x, int y)
{
    /* IMPORTANTE: en la implementación de este método hay que cambiar convenientemente el estado
        de los objetos de la aplicación, pero no hacer llamadas directas a funciones de OpenGL */

    switch (key)
    {
    case 'e': // activa/desactiva la visualización de los ejes
    case 'E':
        _instancia->escena.set_ejes(!_instancia->escena.get_ejes());
        break;
    case 27: // tecla de escape para SALIR
        exit(1);
        break;
    case 'c': // Alternar entre Modo Objeto y Modo Cámara
    case 'C':
        _instancia->modoCamara = !_instancia->modoCamara;
        break;

    //practica 1: transformaciones geometricas
    case '1':
    case '2':
    case '3':
        _instancia->escena.set_seleccionado(key - '1'); ///<- "key" es ASCII, le resto "1" para transformalo
        break;
    case 'u':
        _instancia->escena.trasladar(0, 0.5f, 0);
        break;
    case 'U':
        _instancia->escena.trasladar(0, -0.5f, 0);
        break;

    // --- ROTACIÓN EN X ---
    case 'x':
        _instancia->escena.rotar(5.0f, 0, 0);
        break;
    case 'X':
        _instancia->escena.rotar(-5.0f, 0, 0);
        break;

    // --- ROTACIÓN EN Y (DEPENDE DEL MODO) ---
    case 'y':
        if (_instancia->modoCamara)
            _instancia->camara.rotacionY(5.0);
        else
            _instancia->escena.rotar(0, 5.0f, 0);
        break;
    case 'Y':
        if (_instancia->modoCamara)
            _instancia->camara.rotacionY(-5.0);
        else
            _instancia->escena.rotar(0, -5.0f, 0);
        break;

    // --- ROTACIÓN EN Z ---
    case 'z':
        _instancia->escena.rotar(0, 0, 5.0f);
        break;
    case 'Z':
        _instancia->escena.rotar(0, 0, -5.0f);
        break;

    // --- ESCALADO HOMOGÉNEO ---
    case 's':
        _instancia->escena.escalar(0.1f);
        break;
    case 'S':
        _instancia->escena.escalar(-0.1f);
        break;

    // --- RECORTE: PLANO CERCANO (front / near) ---
    case 'f':
        _instancia->camara.moverZnear(0.2);
        break;
    case 'F':
        _instancia->camara.moverZnear(-0.2);
        break;

    // --- RECORTE: PLANO LEJANO (back / far) ---
    case 'b':
        _instancia->camara.moverZfar(0.2);
        break;
    case 'B':
        _instancia->camara.moverZfar(-0.2);
        break;

    // --- ZOOM (Factores inversos 0.95 y 1/0.95) ---
    case '+':
        _instancia->camara.zoom(0.95);
        break;
    case '-':
        _instancia->camara.zoom(1.0 / 0.95);
        break;

    // --- CAMBIAR PROYECCIÓN (Paralela / Perspectiva) ---
    case 'p':
    case 'P':
        if (_instancia->camara.getTipo() == IGV_PARALELA)
            _instancia->camara.setTipo(IGV_PERSPECTIVA);
        else
            _instancia->camara.setTipo(IGV_PARALELA);
        break;
    }

    glutPostRedisplay();
}

/**
 * Control de eventos de teclas especiales (Cursores)
 */
void igvInterfaz::specialFunc(int key, int x, int y)
{
    if (_instancia->modoCamara)
    {
        // --- MODO CÁMARA ---
        switch (key)
        {
        case GLUT_KEY_LEFT:
            _instancia->camara.orbita(-5.0);
            break;
        case GLUT_KEY_RIGHT:
            _instancia->camara.orbita(5.0);
            break;
        case GLUT_KEY_UP:
            _instancia->camara.cabeceo(5.0);
            break;
        case GLUT_KEY_DOWN:
            _instancia->camara.cabeceo(-5.0);
            break;
        }
    }
    else
    {
        // --- MODO OBJETO ---
        switch (key)
        {
        case GLUT_KEY_LEFT:
            _instancia->escena.trasladar(0.5f, 0, 0);
            break;
        case GLUT_KEY_RIGHT:
            _instancia->escena.trasladar(-0.5f, 0, 0);
            break;
        case GLUT_KEY_UP:
            _instancia->escena.trasladar(0, 0, 0.5f);
            break;
        case GLUT_KEY_DOWN:
            _instancia->escena.trasladar(0, 0, -0.5f);
            break;
        }
    }

    glutPostRedisplay();
}

/**
 * Método que define la cámara de visión y el viewport. Se llama automáticamente
 * cuando se cambia el tamaño de la ventana.
 * @param w Nuevo ancho de la ventana
 * @param h Nuevo alto de la ventana
 * @pre Se asume que todos los parámetros tienen valores válidos
 */
void igvInterfaz::reshapeFunc(int w, int h)
{
    // dimensiona el viewport al nuevo ancho y alto de la ventana
    // guardamos valores nuevos de la ventana de visualizacion
    _instancia->set_ancho_ventana(w);
    _instancia->set_alto_ventana(h);

    // establece los parámetros de la cámara y de la proyección
    _instancia->camara.aplicar();
}

/**
 * Método para visualizar la escena
 *
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
 *
 */
void igvInterfaz::displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el Z-buffer

    // se establece el viewport
    glViewport(0, 0, _instancia->get_ancho_ventana(), _instancia->get_alto_ventana());

    //cargar las transformaciones de la camara
    _instancia->camara.aplicar();

    //visualiza la escena
    _instancia->escena.visualizar();



    glEnable ( GL_SCISSOR_TEST );
    // borrar solo el recuadro
    glScissor ( _instancia->get_ancho_ventana() - _instancia->get_ancho_ventana()/3, _instancia->get_alto_ventana() - _instancia->get_alto_ventana()/3, _instancia->get_ancho_ventana()/3, _instancia->get_alto_ventana()/3 );

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glDisable ( GL_SCISSOR_TEST );

    //viewport vista superior (planta) de la escena
    glViewport(_instancia->get_ancho_ventana()-(_instancia->get_ancho_ventana()/3), _instancia->get_alto_ventana()-(_instancia->get_alto_ventana()/3), _instancia->get_ancho_ventana()/3, _instancia->get_alto_ventana()/3);

    _instancia->camaraPlanta.aplicar();
    _instancia->escena.visualizar ();


    // refresca la ventana
    glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo


}

/**
 * Método para inicializar los callbacks GLUT
 */
void igvInterfaz::inicializa_callbacks()
{
    glutKeyboardFunc(keyboardFunc);
    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
    glutSpecialFunc(specialFunc);
}


/**
 * Método para consultar el ancho de la ventana de visualización
 * @return El valor almacenado como ancho de la ventana de visualización
 */
int igvInterfaz::get_ancho_ventana()
{
    return ancho_ventana;
}

/**
 * Método para consultar el alto de la ventana de visualización
 * @return El valor almacenado como alto de la ventana de visualización
 */
int igvInterfaz::get_alto_ventana()
{
    return alto_ventana;
}

/**
 * Método para cambiar el ancho de la ventana de visualización
 * @param _ancho_ventana Nuevo valor para el ancho de la ventana de visualización
 * @pre Se asume que el parámetro tiene un valor válido
 * @post El ancho de ventana almacenado en la aplicación cambia al nuevo valor
 */
void igvInterfaz::set_ancho_ventana(int _ancho_ventana)
{
    ancho_ventana = _ancho_ventana;
}

/**
 * Método para cambiar el alto de la ventana de visualización
 * @param _alto_ventana Nuevo valor para el alto de la ventana de visualización
 * @pre Se asume que el parámetro tiene un valor válido
 * @post El alto de ventana almacenado en la aplicación cambia al nuevo valor
 */
void igvInterfaz::set_alto_ventana(int _alto_ventana)
{
    alto_ventana = _alto_ventana;
}
