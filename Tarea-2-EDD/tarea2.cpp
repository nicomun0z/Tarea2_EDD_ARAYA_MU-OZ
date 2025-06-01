#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Estructuras para Enemigos
struct Enemigo {
    string nombre;
    int vida;
    int ataque;
    float precision;
    float probabilidad;
    
    Enemigo() : vida(0), ataque(0), precision(0.0), probabilidad(0.0) {}
    Enemigo(string n, int v, int a, float p, float prob)
        : nombre(n), vida(v), ataque(a), precision(p), probabilidad(prob) {}
};

struct NodoEnemigo {
    Enemigo* enemigo;
    NodoEnemigo* siguiente;
    
    NodoEnemigo(Enemigo* e) : enemigo(e), siguiente(nullptr) {}
};

class ListaEnemigos {
private:
    NodoEnemigo* cabeza;
    
public:
    ListaEnemigos() : cabeza(nullptr) {}
    
    ~ListaEnemigos() {
        while (cabeza != nullptr) {
            NodoEnemigo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp->enemigo;
            delete temp;
        }
    }
    
    void agregar(Enemigo* enemigo) {
        NodoEnemigo* nuevo = new NodoEnemigo(enemigo);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    
    void mostrar() {
        NodoEnemigo* actual = cabeza;
        while (actual != nullptr) {
            cout << "Enemigo: " << actual->enemigo->nombre 
                 << ", Vida: " << actual->enemigo->vida
                 << ", Ataque: " << actual->enemigo->ataque
                 << ", Precision: " << actual->enemigo->precision
                 << ", Probabilidad: " << actual->enemigo->probabilidad << endl;
            actual = actual->siguiente;
        }
    }

    NodoEnemigo* getCabeza() { return cabeza; }
};

// Estructuras para Eventos
struct Efecto {
    string descripcion;
    string modificador; // ej: "+10 Vida", "-0.2 PRECISION"
    
    Efecto() {}
    Efecto(string desc, string mod) : descripcion(desc), modificador(mod) {}
};

struct Opcion {
    string enunciado;
    Efecto efecto;
    
    Opcion() {}
    Opcion(string e, Efecto ef) : enunciado(e), efecto(ef) {}
};

struct Evento {
    string nombre;
    float probabilidad;
    string descripcion;
    Opcion opcionA;
    Opcion opcionB;
    
    Evento() : probabilidad(0.0) {}
};

struct NodoEvento {
    Evento* evento;
    NodoEvento* siguiente;
    
    NodoEvento(Evento* e) : evento(e), siguiente(nullptr) {}
};

class ListaEventos {
private:
    NodoEvento* cabeza;
    
public:
    ListaEventos() : cabeza(nullptr) {}
    
    ~ListaEventos() {
        while (cabeza != nullptr) {
            NodoEvento* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp->evento;
            delete temp;
        }
    }
    
    void agregar(Evento* evento) {
        NodoEvento* nuevo = new NodoEvento(evento);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    
    void mostrar() {
        NodoEvento* actual = cabeza;
        while (actual != nullptr) {
            cout << "Evento: " << actual->evento->nombre 
                 << ", Probabilidad: " << actual->evento->probabilidad << endl;
            cout << "  Descripcion: " << actual->evento->descripcion << endl;
            cout << "  Opcion A: " << actual->evento->opcionA.enunciado << endl;
            cout << "  Opcion B: " << actual->evento->opcionB.enunciado << endl;
            actual = actual->siguiente;
        }
    }
    NodoEvento* getCabeza() { return cabeza; }
};

// Estructuras para Habitaciones (Árbol Ternario)
struct Habitacion {
    int id;
    string nombre;
    string tipo;
    string descripcion;
    Habitacion* izquierda;
    Habitacion* centro;
    Habitacion* derecha;
    
    Habitacion() : id(-1), izquierda(nullptr), centro(nullptr), derecha(nullptr) {}
    Habitacion(int i, string n, string t, string d) 
        : id(i), nombre(n), tipo(t), descripcion(d), 
          izquierda(nullptr), centro(nullptr), derecha(nullptr) {}
};

// Lista temporal para almacenar habitaciones antes de construir el árbol
struct NodoHabitacion {
    Habitacion* habitacion;
    NodoHabitacion* siguiente;
    
    NodoHabitacion(Habitacion* h) : habitacion(h), siguiente(nullptr) {}
};

class ListaHabitaciones {
private:
    NodoHabitacion* cabeza;
    
public:
    ListaHabitaciones() : cabeza(nullptr) {}
    
    ~ListaHabitaciones() {
        while (cabeza != nullptr) {
            NodoHabitacion* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;  // No eliminamos la habitación aquí, se usa en el árbol
        }
    }
    
    void agregar(Habitacion* habitacion) {
        NodoHabitacion* nuevo = new NodoHabitacion(habitacion);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    
    Habitacion* buscarPorId(int id) {
        NodoHabitacion* actual = cabeza;
        while (actual != nullptr) {
            if (actual->habitacion->id == id) {
                return actual->habitacion;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void mostrar() {
        NodoHabitacion* actual = cabeza;
        while (actual != nullptr) {
            cout << "Habitacion " << actual->habitacion->id 
                 << ": " << actual->habitacion->nombre 
                 << " (" << actual->habitacion->tipo << ")" << endl;
            cout << "  " << actual->habitacion->descripcion << endl;
            actual = actual->siguiente;
        }
    }

    NodoHabitacion* getCabeza() { return cabeza; } // <-- AGREGA ESTA LÍNEA
};

// Estructura para almacenar arcos
struct Arco {
    int origen;
    int destino;
    
    Arco(int o, int d) : origen(o), destino(d) {}
};

struct NodoArco {
    Arco* arco;
    NodoArco* siguiente;
    
    NodoArco(Arco* a) : arco(a), siguiente(nullptr) {}
};

class ListaArcos {
private:
    NodoArco* cabeza;
    
public:
    ListaArcos() : cabeza(nullptr) {}
    
    ~ListaArcos() {
        while (cabeza != nullptr) {
            NodoArco* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp->arco;
            delete temp;
        }
    }
    
    void agregar(Arco* arco) {
        NodoArco* nuevo = new NodoArco(arco);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    
    void mostrar() {
        NodoArco* actual = cabeza;
        while (actual != nullptr) {
            cout << "Arco: " << actual->arco->origen 
                 << " -> " << actual->arco->destino << endl;
            actual = actual->siguiente;
        }
    }
    
    NodoArco* getCabeza() { return cabeza; }
};

// Lista para mejoras de combate
struct NodoMejora {
    string mejora;
    NodoMejora* siguiente;
    
    NodoMejora(string m) : mejora(m), siguiente(nullptr) {}
};

class ListaMejoras {
private:
    NodoMejora* cabeza;
    
public:
    ListaMejoras() : cabeza(nullptr) {}
    
    ~ListaMejoras() {
        while (cabeza != nullptr) {
            NodoMejora* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
    
    void agregar(string mejora) {
        NodoMejora* nuevo = new NodoMejora(mejora);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    
    void mostrar() {
        NodoMejora* actual = cabeza;
        while (actual != nullptr) {
            cout << "Mejora: " << actual->mejora << endl;
            actual = actual->siguiente;
        }
    }
    NodoMejora* getCabeza() { return cabeza; }
};

// Variables globales para almacenar los datos del juego
ListaHabitaciones* habitaciones = nullptr;
ListaArcos* arcos = nullptr;
ListaEnemigos* enemigos = nullptr;
ListaEventos* eventos = nullptr;
ListaMejoras* mejoras = nullptr;
Habitacion* raizArbol = nullptr;

/*****
* void eliminarEspacios
******
* Elimina espacios en blanco al inicio y final de una cadena
******
* Input:
* string& str : Referencia a la cadena a procesar
******
* Returns:
* void
*****/
void eliminarEspacios(string& str) {
    // Eliminar espacios al inicio
    size_t inicio = str.find_first_not_of(" \t\r\n");
    if (inicio == string::npos) {
        str = "";
        return;
    }
    
    // Eliminar espacios al final
    size_t fin = str.find_last_not_of(" \t\r\n");
    str = str.substr(inicio, fin - inicio + 1);
}

/*****
* void leerHabitaciones
******
* Lee la sección HABITACIONES del archivo .map
******
* Input:
* ifstream& archivo : Referencia al archivo abierto
* int cantidad : Número de habitaciones a leer
******
* Returns:
* void
*****/
void leerHabitaciones(ifstream& archivo, int cantidad) {
    habitaciones = new ListaHabitaciones();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            
            // Formato: ID Nombre (TIPO)
            stringstream ss(linea);
            string idStr, nombre, tipo;
            
            ss >> idStr;
            int id = atoi(idStr.c_str());
            
            // Buscar el tipo entre paréntesis
            size_t inicioTipo = linea.find('(');
            size_t finTipo = linea.find(')');
            
            if (inicioTipo != string::npos && finTipo != string::npos) {
                tipo = linea.substr(inicioTipo + 1, finTipo - inicioTipo - 1);
                // El nombre está entre el primer espacio después del ID y el '('
                size_t inicioNombre = linea.find_first_not_of(" ", idStr.length());
                nombre = linea.substr(inicioNombre, inicioTipo - inicioNombre);
                eliminarEspacios(nombre);
                eliminarEspacios(tipo);
            }
            
            // Leer la descripción (siguiente línea)
            string descripcion;
            if (getline(archivo, descripcion)) {
                eliminarEspacios(descripcion);
            }
            
            Habitacion* nueva = new Habitacion(id, nombre, tipo, descripcion);
            habitaciones->agregar(nueva);
        }
    }
}

/*****
* void leerArcos
******
* Lee la sección ARCOS del archivo .map
******
* Input:
* ifstream& archivo : Referencia al archivo abierto
* int cantidad : Número de arcos a leer
******
* Returns:
* void
*****/
void leerArcos(ifstream& archivo, int cantidad) {
    arcos = new ListaArcos();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            
            // Formato: origen -> destino
            size_t flecha = linea.find("->");
            if (flecha != string::npos) {
                string origenStr = linea.substr(0, flecha);
                string destinoStr = linea.substr(flecha + 2);
                
                eliminarEspacios(origenStr);
                eliminarEspacios(destinoStr);
                
                int origen = atoi(origenStr.c_str());
                int destino = atoi(destinoStr.c_str());
                
                Arco* nuevo = new Arco(origen, destino);
                arcos->agregar(nuevo);
            }
        }
    }
}

/*****
* void leerEnemigos
******
* Lee la sección ENEMIGOS del archivo .map
******
* Input:
* ifstream& archivo : Referencia al archivo abierto
* int cantidad : Número de enemigos a leer
******
* Returns:
* void
*****/
void leerEnemigos(ifstream& archivo, int cantidad) {
    enemigos = new ListaEnemigos();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            
            // Formato: Nombre | Vida X | Ataque Y | Precision Z | Probabilidad W
            stringstream ss(linea);
            string parte;
            string nombre;
            int vida = 0, ataque = 0;
            float precision = 0.0, probabilidad = 0.0;
            
            // Dividir por '|'
            int contador = 0;
            while (getline(ss, parte, '|')) {
                eliminarEspacios(parte);
                
                if (contador == 0) {
                    nombre = parte;
                } else if (contador == 1) {
                    // "Vida X"
                    stringstream vidaSS(parte);
                    string temp;
                    vidaSS >> temp >> vida;
                } else if (contador == 2) {
                    // "Ataque Y"
                    stringstream ataqueSS(parte);
                    string temp;
                    ataqueSS >> temp >> ataque;
                } else if (contador == 3) {
                    // "Precision Z"
                    stringstream precisionSS(parte);
                    string temp;
                    precisionSS >> temp >> precision;
                } else if (contador == 4) {
                    // "Probabilidad W"
                    stringstream probabilidadSS(parte);
                    string temp;
                    probabilidadSS >> temp >> probabilidad;
                }
                contador++;
            }
            
            Enemigo* nuevo = new Enemigo(nombre, vida, ataque, precision, probabilidad);
            enemigos->agregar(nuevo);
        }
    }
}

/*****
* void leerEventos
******
* Lee la sección EVENTOS del archivo .map
******
* Input:
* ifstream& archivo : Referencia al archivo abierto
* int cantidad : Número de eventos a leer
******
* Returns:
* void
*****/
void leerEventos(ifstream& archivo, int cantidad) {
    eventos = new ListaEventos();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        // Leer separador '&'
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            if (linea != "&") {
                continue; // Buscar el separador correcto
            }
        }
        
        Evento* nuevo = new Evento();
        
        // Leer nombre del evento
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->nombre = linea;
        }
        
        // Leer probabilidad
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            if (linea.find("Probabilidad") != string::npos) {
                stringstream ss(linea);
                string temp;
                ss >> temp >> nuevo->probabilidad;
            }
        }
        
        // Leer descripción del evento
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->descripcion = linea;
        }
        
        // Leer opción A
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            if (linea.find("A:") != string::npos) {
                nuevo->opcionA.enunciado = linea.substr(2);
                eliminarEspacios(nuevo->opcionA.enunciado);
            }
        }
        
        // Leer efecto de opción A
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->opcionA.efecto.descripcion = linea;
        }
        
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->opcionA.efecto.modificador = linea;
        }
        
        // Leer opción B
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            if (linea.find("B:") != string::npos) {
                nuevo->opcionB.enunciado = linea.substr(2);
                eliminarEspacios(nuevo->opcionB.enunciado);
            }
        }
        
        // Leer efecto de opción B
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->opcionB.efecto.descripcion = linea;
        }
        
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->opcionB.efecto.modificador = linea;
        }
        
        eventos->agregar(nuevo);
    }
}

/*****
* void leerMejoras
******
* Lee la sección MEJORAS DE COMBATE del archivo .map
******
* Input:
* ifstream& archivo : Referencia al archivo abierto
******
* Returns:
* void
*****/
void leerMejoras(ifstream& archivo) {
    mejoras = new ListaMejoras();
    string linea;
    
    // Leer hasta el final del archivo o hasta "FIN DE ARCHIVO"
    while (getline(archivo, linea)) {
        eliminarEspacios(linea);
        if (linea == "FIN DE ARCHIVO" || linea.empty()) {
            break;
        }
        mejoras->agregar(linea);
    }
}

/*****
* void construirArbol
******
* Construye el árbol ternario usando las habitaciones y arcos leídos
******
* Input:
* void
******
* Returns:
* void
*****/
void construirArbol() {
    if (habitaciones == nullptr || arcos == nullptr) {
        return;
    }
    
    // Encontrar la habitación inicial (tipo INICIO)
    // Buscar manualmente en la lista
    NodoHabitacion* actual = habitaciones->getCabeza();
    while (actual != nullptr) {
        if (actual->habitacion->tipo == "INICIO") {
            raizArbol = actual->habitacion;
            break;
        }
        actual = actual->siguiente;
    }
    
    // Construir conexiones usando los arcos
    NodoArco* arcoActual = arcos->getCabeza();
    while (arcoActual != nullptr) {
        Habitacion* origen = habitaciones->buscarPorId(arcoActual->arco->origen);
        Habitacion* destino = habitaciones->buscarPorId(arcoActual->arco->destino);
        
        if (origen != nullptr && destino != nullptr) {
            // Asignar a la primera posición disponible (izquierda, centro, derecha)
            if (origen->izquierda == nullptr) {
                origen->izquierda = destino;
            } else if (origen->centro == nullptr) {
                origen->centro = destino;
            } else if (origen->derecha == nullptr) {
                origen->derecha = destino;
            }
        }
        
        arcoActual = arcoActual->siguiente;
    }
}

/*****
* void cargarMapa
******
* Función principal que lee el archivo .map y carga todas las estructuras
******
* Input:
* const char* nombreArchivo : Nombre del archivo .map a leer
******
* Returns:
* void
*****/
void cargarMapa(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }
    
    string linea;
    
    // Buscar el inicio del archivo
    while (getline(archivo, linea)) {
        eliminarEspacios(linea);
        if (linea == "INICIO DE ARCHIVO") {
            break;
        }
    }
    
    // Procesar cada sección
    while (getline(archivo, linea)) {
        eliminarEspacios(linea);
        
        if (linea == "HABITACIONES") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore(); // Ignorar el salto de línea
            leerHabitaciones(archivo, cantidad);
            
        } else if (linea == "ARCOS") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore();
            leerArcos(archivo, cantidad);
            
        } else if (linea == "ENEMIGOS") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore();
            leerEnemigos(archivo, cantidad);
            
        } else if (linea == "EVENTOS") {
            int cantidad;
            archivo >> cantidad;
            archivo.ignore();
            leerEventos(archivo, cantidad);
            
        } else if (linea == "MEJORAS DE COMBATE") {
            leerMejoras(archivo);
            
        } else if (linea == "FIN DE ARCHIVO") {
            break;
        }
    }
    
    archivo.close();
    
    // Construir el árbol después de cargar habitaciones y arcos
    construirArbol();
    
    cout << "Archivo cargado exitosamente!" << endl;
}


/*****
* void liberarMemoria
******
* Libera toda la memoria dinámica utilizada
******
* Input:
* void
******
* Returns:
* void
*****/
void liberarMemoria() {
    delete habitaciones;
    delete arcos;
    delete enemigos;
    delete eventos;
    delete mejoras;
    
    // Note: Las habitaciones se liberan automáticamente cuando se destruye la lista
    // pero el árbol mantiene punteros a ellas, por lo que no las eliminamos dos veces
}





/*****
* int main
******
* Función principal para probar el lector de archivos .map
******
* Input:
* void
******
* Returns:
* int : Código de salida del programa
*****/
// Estructura del Jugador
class Jugador {
private:
    int vida;
    int ataque;
    float precision;
    int recuperacion;
    
public:
    Jugador(int v = 30, int a = 7, float p = 0.8, int r = 3) 
        : vida(v), ataque(a), precision(p), recuperacion(r) {}
    
    // Getters
    int getVida() const { return vida; }
    int getAtaque() const { return ataque; }
    float getPrecision() const { return precision; }
    int getRecuperacion() const { return recuperacion; }
    
    // Setters
    void setVida(int v) { vida = v; }
    void setAtaque(int a) { ataque = a; }
    void setPrecision(float p) { precision = p; }
    void setRecuperacion(int r) { recuperacion = r; }
    
    // Métodos de modificación
    void curar(int cantidad) { vida += cantidad; }
    void recibirDano(int dano) { vida -= dano; if (vida < 0) vida = 0; }
    void aumentarAtaque(int cantidad) { ataque += cantidad; }
    void aumentarPrecision(float cantidad) { precision += cantidad; if (precision > 1.0) precision = 1.0; }
    void aumentarRecuperacion(int cantidad) { recuperacion += cantidad; }
    
    bool estaVivo() const { return vida > 0; }
};

// TDA Cola para combate
struct NodoCombate {
    Enemigo* enemigo;
    NodoCombate* siguiente;
    
    NodoCombate(Enemigo* e) : enemigo(e), siguiente(nullptr) {}
};

class ColaCombate {
private:
    NodoCombate* frente;
    NodoCombate* final;
    
public:
    ColaCombate() : frente(nullptr), final(nullptr) {}
    
    ~ColaCombate() {
        while (!estaVacia()) {
            desencolar();
        }
    }
    
    void encolar(Enemigo* enemigo) {
        // Crear una copia del enemigo para el combate
        Enemigo* enemigoComba = new Enemigo(*enemigo);
        NodoCombate* nuevo = new NodoCombate(enemigoComba);
        
        if (final == nullptr) {
            frente = final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
    }
    
    void desencolar() {
        if (frente == nullptr) return;
        
        NodoCombate* temp = frente;
        frente = frente->siguiente;
        if (frente == nullptr) final = nullptr;
        
        delete temp->enemigo;
        delete temp;
    }
    
    Enemigo* obtenerFrente() {
        return (frente != nullptr) ? frente->enemigo : nullptr;
    }
    
    bool estaVacia() const {
        return frente == nullptr;
    }
    
    void mostrarEnemigos() {
        NodoCombate* actual = frente;
        while (actual != nullptr) {
            if (actual->enemigo->vida > 0) {
                cout << actual->enemigo->nombre << " | ";
            } else {
                cout << "X | ";
            }
            actual = actual->siguiente;
        }
    }
    
    void eliminarMuertos() {
        while (frente != nullptr && frente->enemigo->vida <= 0) {
            desencolar();
        }
        
        if (frente != nullptr) {
            NodoCombate* actual = frente;
            while (actual->siguiente != nullptr) {
                if (actual->siguiente->enemigo->vida <= 0) {
                    NodoCombate* temp = actual->siguiente;
                    actual->siguiente = temp->siguiente;
                    if (temp == final) final = actual;
                    delete temp->enemigo;
                    delete temp;
                } else {
                    actual = actual->siguiente;
                }
            }
        }
    }

    NodoCombate* getCabeza() { return frente; } // <-- AGREGA ESTA LÍNEA
};

/*****
* void seleccionarEnemigosAleatorios
******
* Selecciona enemigos aleatorios para el combate basado en sus probabilidades
******
* Input:
* ColaCombate& cola : Cola donde se almacenarán los enemigos seleccionados
******
* Returns:
* void
*****/
void seleccionarEnemigosAleatorios(ColaCombate& cola) {
    if (enemigos == nullptr) return;
    
    // Determinar cantidad de enemigos (1-3)
    int cantidadEnemigos = (rand() % 3) + 1;
    
    NodoEnemigo* actual = enemigos->getCabeza();
    if (actual == nullptr) return;
    
    for (int i = 0; i < cantidadEnemigos; i++) {
        // Seleccionar enemigo basado en probabilidad
        float probabilidadTotal = 0.0;
        NodoEnemigo* temp = actual;
        while (temp != nullptr) {
            probabilidadTotal += temp->enemigo->probabilidad;
            temp = temp->siguiente;
        }
        
        float seleccion = ((float)rand() / RAND_MAX) * probabilidadTotal;
        float acumulado = 0.0;
        
        temp = actual;
        while (temp != nullptr) {
            acumulado += temp->enemigo->probabilidad;
            if (seleccion <= acumulado) {
                cola.encolar(temp->enemigo);
                break;
            }
            temp = temp->siguiente;
        }
    }
}

/*****
* bool realizarCombate
******
* Ejecuta un combate completo entre el jugador y los enemigos
******
* Input:
* Jugador& jugador : Referencia al jugador
******
* Returns:
* bool : true si el jugador sobrevive, false si muere
*****/
bool realizarCombate(Jugador& jugador) {
    ColaCombate cola;
    seleccionarEnemigosAleatorios(cola);
    
    if (cola.estaVacia()) {
        cout << "No hay enemigos en esta habitación." << endl;
        return true;
    }
    
    cout << "¡Encuentras enemigos!" << endl;
    
    while (!cola.estaVacia() && jugador.estaVivo()) {
        // Mostrar estado actual
        cout << "Jugador | ";
        cola.mostrarEnemigos();
        cout << endl;
        cout << jugador.getVida() << " | ";
        
        NodoCombate* temp = cola.getCabeza();
        while (temp != nullptr) {
            if (temp->enemigo->vida > 0) {
                cout << temp->enemigo->vida << " | ";
            } else {
                cout << "X | ";
            }
            temp = temp->siguiente;
        }
        cout << endl;
        
        // Turno del jugador
        Enemigo* objetivo = cola.obtenerFrente();
        if (objetivo != nullptr && objetivo->vida > 0) {
            float probabilidadGolpe = (float)rand() / RAND_MAX;
            if (probabilidadGolpe <= jugador.getPrecision()) {
                objetivo->vida -= jugador.getAtaque();
                cout << "Jugador golpea a " << objetivo->nombre 
                     << " por " << jugador.getAtaque() << " de daño!" << endl;
                if (objetivo->vida <= 0) {
                    objetivo->vida = 0;
                    cout << objetivo->nombre << " ha sido derrotado!" << endl;
                }
            } else {
                cout << "¡Jugador falla!" << endl;
            }
        }
        
        // Eliminar enemigos muertos
        cola.eliminarMuertos();
        
        if (cola.estaVacia()) break;
        
        // Turno de los enemigos
        NodoCombate* enemigoActual = cola.getCabeza();
        while (enemigoActual != nullptr && jugador.estaVivo()) {
            if (enemigoActual->enemigo->vida > 0) {
                float probabilidadGolpe = (float)rand() / RAND_MAX;
                if (probabilidadGolpe <= enemigoActual->enemigo->precision) {
                    jugador.recibirDano(enemigoActual->enemigo->ataque);
                    cout << enemigoActual->enemigo->nombre << " golpea a Jugador por " 
                         << enemigoActual->enemigo->ataque << " de daño!" << endl;
                } else {
                    cout << enemigoActual->enemigo->nombre << " falla!" << endl;
                }
            }
            enemigoActual = enemigoActual->siguiente;
        }
    }
    
    if (jugador.estaVivo()) {
        cout << "¡Has sobrevivido el combate!" << endl;
        jugador.curar(jugador.getRecuperacion());
        cout << "Recuperas " << jugador.getRecuperacion() << " de vida tras el combate." << endl;
        return true;
    } else {
        cout << "¡Has sido derrotado! GAME OVER" << endl;
        return false;
    }
}

/*****
* void aplicarEfecto
******
* Aplica el efecto de un evento al jugador
******
* Input:
* Jugador& jugador : Referencia al jugador
* const Efecto& efecto : Efecto a aplicar
******
* Returns:
* void
*****/
void aplicarEfecto(Jugador& jugador, const Efecto& efecto) {
    string modificador = efecto.modificador;
    
    if (modificador.find("Ninguna consecuencia") != string::npos) {
        cout << "No hay consecuencias." << endl;
        return;
    }
    
    // Parsear el modificador
    if (modificador.find("+") != string::npos) {
        if (modificador.find("Vida") != string::npos) {
            int cantidad = 0;
            sscanf(modificador.c_str(), "+%d Vida", &cantidad);
            jugador.curar(cantidad);
            cout << "Ganas " << cantidad << " de vida!" << endl;
        } else if (modificador.find("Ataque") != string::npos) {
            int cantidad = 0;
            sscanf(modificador.c_str(), "+%d Ataque", &cantidad);
            jugador.aumentarAtaque(cantidad);
            cout << "Tu ataque aumenta en " << cantidad << "!" << endl;
        } else if (modificador.find("Precision") != string::npos || 
                   modificador.find("PRECISION") != string::npos) {
            float cantidad = 0.0;
            sscanf(modificador.c_str(), "+%f", &cantidad);
            jugador.aumentarPrecision(cantidad);
            cout << "Tu precisión aumenta!" << endl;
        } else if (modificador.find("Recuperacion") != string::npos) {
            int cantidad = 0;
            sscanf(modificador.c_str(), "+%d Recuperacion", &cantidad);
            jugador.aumentarRecuperacion(cantidad);
            cout << "Tu recuperación aumenta en " << cantidad << "!" << endl;
        }
    } else if (modificador.find("-") != string::npos) {
        if (modificador.find("Vida") != string::npos) {
            int cantidad = 0;
            sscanf(modificador.c_str(), "-%d Vida", &cantidad);
            jugador.recibirDano(cantidad);
            cout << "Pierdes " << cantidad << " de vida!" << endl;
        } else if (modificador.find("PRECISION") != string::npos || 
                   modificador.find("Precision") != string::npos) {
            float cantidad = 0.0;
            sscanf(modificador.c_str(), "-%f", &cantidad);
            float nuevaPrecision = jugador.getPrecision() - cantidad;
            if (nuevaPrecision < 0) nuevaPrecision = 0;
            jugador.setPrecision(nuevaPrecision);
            cout << "Tu precisión disminuye!" << endl;
        }
    }
}

/*****
* Evento* seleccionarEventoAleatorio
******
* Selecciona un evento aleatorio basado en las probabilidades
******
* Input:
* void
******
* Returns:
* Evento* : Puntero al evento seleccionado, nullptr si no hay eventos
*****/
Evento* seleccionarEventoAleatorio() {
    if (eventos == nullptr) return nullptr;
    
    // Calcular probabilidad total
    float probabilidadTotal = 0.0;
    NodoEvento* actual = eventos->getCabeza();
    while (actual != nullptr) {
        probabilidadTotal += actual->evento->probabilidad;
        actual = actual->siguiente;
    }
    
    if (probabilidadTotal <= 0) return nullptr;
    
    // Seleccionar evento
    float seleccion = ((float)rand() / RAND_MAX) * probabilidadTotal;
    float acumulado = 0.0;
    
    actual = eventos->getCabeza();
    while (actual != nullptr) {
        acumulado += actual->evento->probabilidad;
        if (seleccion <= acumulado) {
            return actual->evento;
        }
        actual = actual->siguiente;
    }
    
    return nullptr;
}

/*****
* bool manejarEvento
******
* Maneja la ejecución de un evento
******
* Input:
* Jugador& jugador : Referencia al jugador
******
* Returns:
* bool : true si el jugador continúa, false si muere
*****/
bool manejarEvento(Jugador& jugador) {
    Evento* evento = seleccionarEventoAleatorio();
    if (evento == nullptr) {
        cout << "No hay eventos disponibles." << endl;
        return true;
    }
    
    cout << evento->descripcion << endl;
    cout << "A: " << evento->opcionA.enunciado << endl;
    cout << "B: " << evento->opcionB.enunciado << endl;
    cout << "Elige tu opción (A/B): ";
    
    char opcion;
    cin >> opcion;
    
    if (opcion == 'A' || opcion == 'a') {
        cout << evento->opcionA.efecto.descripcion << endl;
        aplicarEfecto(jugador, evento->opcionA.efecto);
    } else {
        cout << evento->opcionB.efecto.descripcion << endl;
        aplicarEfecto(jugador, evento->opcionB.efecto);
    }
    
    return jugador.estaVivo();
}

/*****
* void mostrarMejoras
******
* Muestra las mejoras disponibles después de un combate
******
* Input:
* void
******
* Returns:
* void
*****/
void mostrarMejoras() {
    if (mejoras == nullptr) return;
    
    cout << "Elige una mejora:" << endl;
    int contador = 1;
    NodoMejora* actual = mejoras->getCabeza();
    while (actual != nullptr) {
        cout << contador << ". " << actual->mejora << endl;
        actual = actual->siguiente;
        contador++;
    }
}

/*****
* void aplicarMejora
******
* Aplica una mejora seleccionada al jugador
******
* Input:
* Jugador& jugador : Referencia al jugador
* int opcion : Opción seleccionada por el jugador
******
* Returns:
* void
*****/
void aplicarMejora(Jugador& jugador, int opcion) {
    if (mejoras == nullptr) return;
    
    NodoMejora* actual = mejoras->getCabeza();
    int contador = 1;
    
    while (actual != nullptr && contador < opcion) {
        actual = actual->siguiente;
        contador++;
    }
    
    if (actual == nullptr) return;
    
    string mejora = actual->mejora;
    
    if (mejora.find("Vida") != string::npos) {
        int cantidad = 0;
        sscanf(mejora.c_str(), "+%d Vida", &cantidad);
        jugador.curar(cantidad);
        cout << "Recuperaste " << cantidad << " de vida!" << endl;
    } else if (mejora.find("Precision") != string::npos) {
        float cantidad = 0.0;
        sscanf(mejora.c_str(), "+%f Precision", &cantidad);
        jugador.aumentarPrecision(cantidad);
        cout << "Tu precisión aumentó!" << endl;
    } else if (mejora.find("Ataque") != string::npos) {
        int cantidad = 0;
        sscanf(mejora.c_str(), "+%d Ataque", &cantidad);
        jugador.aumentarAtaque(cantidad);
        cout << "Tu ataque aumentó en " << cantidad << "!" << endl;
    } else if (mejora.find("Recuperacion") != string::npos) {
        int cantidad = 0;
        sscanf(mejora.c_str(), "+%d Recuperacion", &cantidad);
        jugador.aumentarRecuperacion(cantidad);
        cout << "Tu recuperación aumentó en " << cantidad << "!" << endl;
    }
}

/*****
* void mostrarOpcionesHabitacion
******
* Muestra las opciones de habitaciones disponibles desde la actual
******
* Input:
* Habitacion* actual : Habitación actual
******
* Returns:
* void
*****/
void mostrarOpcionesHabitacion(Habitacion* actual) {
    if (actual == nullptr) return;
    
    cout << "¿A dónde quieres ir?" << endl;
    int opcion = 1;
    
    if (actual->izquierda != nullptr) {
        cout << opcion << ". " << actual->izquierda->nombre << endl;
        opcion++;
    }
    if (actual->centro != nullptr) {
        cout << opcion << ". " << actual->centro->nombre << endl;
        opcion++;
    }
    if (actual->derecha != nullptr) {
        cout << opcion << ". " << actual->derecha->nombre << endl;
    }
}

/*****
* Habitacion* seleccionarSiguienteHabitacion
******
* Permite al jugador seleccionar la siguiente habitación
******
* Input:
* Habitacion* actual : Habitación actual
******
* Returns:
* Habitacion* : Puntero a la habitación seleccionada
*****/
Habitacion* seleccionarSiguienteHabitacion(Habitacion* actual) {
    if (actual == nullptr) return nullptr;
    
    mostrarOpcionesHabitacion(actual);
    
    int opcion;
    cout << "Ingresa tu opción: ";
    cin >> opcion;
    
    int contador = 1;
    if (actual->izquierda != nullptr) {
        if (contador == opcion) return actual->izquierda;
        contador++;
    }
    if (actual->centro != nullptr) {
        if (contador == opcion) return actual->centro;
        contador++;
    }
    if (actual->derecha != nullptr) {
        if (contador == opcion) return actual->derecha;
    }
    
    return nullptr;
}

/*****
* void jugar
******
* Función principal del juego que maneja el flujo completo
******
* Input:
* void
******
* Returns:
* void
*****/
void jugar() {
    if (raizArbol == nullptr) {
        cout << "Error: No se pudo cargar el mapa del juego." << endl;
        return;
    }
    
    Jugador jugador;
    Habitacion* habitacionActual = raizArbol;
    
    cout << "=== BIENVENIDO AL JUEGO ===" << endl;
    
    while (habitacionActual != nullptr && jugador.estaVivo()) {
        // Mostrar habitación actual
        cout << "\n--- " << habitacionActual->nombre << " ---" << endl;
        cout << habitacionActual->descripcion << endl;
        
        // Verificar si es habitación final
        if (habitacionActual->tipo == "FIN") {
            cout << "\n¡Has llegado al final de tu aventura!" << endl;
            break;
        }
        
        bool continuar = true;
        
        // Manejar tipo de habitación
        if (habitacionActual->tipo == "COMBATE") {
            continuar = realizarCombate(jugador);
            if (continuar) {
                // Ofrecer mejoras después del combate
                mostrarMejoras();
                int opcionMejora;
                cout << "Elige tu mejora: ";
                cin >> opcionMejora;
                aplicarMejora(jugador, opcionMejora);
            }
        } else if (habitacionActual->tipo == "EVENTO") {
            continuar = manejarEvento(jugador);
        }
        
        if (!continuar) {
            break;
        }
        
        // Mostrar estado del jugador
        cout << "\nEstado actual - Vida: " << jugador.getVida() 
             << ", Ataque: " << jugador.getAtaque() 
             << ", Precisión: " << jugador.getPrecision() 
             << ", Recuperación: " << jugador.getRecuperacion() << endl;
        
        // Seleccionar siguiente habitación
        if (habitacionActual->izquierda != nullptr || 
            habitacionActual->centro != nullptr || 
            habitacionActual->derecha != nullptr) {
            habitacionActual = seleccionarSiguienteHabitacion(habitacionActual);
        } else {
            cout << "No hay más caminos disponibles." << endl;
            break;
        }
    }
    
    if (!jugador.estaVivo()) {
        cout << "\n¡GAME OVER!" << endl;
    } else {
        cout << "\n¡Gracias por jugar!" << endl;
    }
}
int main() {
    srand(time(nullptr));
    
    cout << "=== LECTOR DE ARCHIVO .MAP ===" << endl;
    cout << "Cargando archivo data.map..." << endl;
    
    // Cargar el archivo
    cargarMapa("data.map");
    
    // Ejecutar el juego
    jugar();
    
    // Liberar memoria
    liberarMemoria();
    
    return 0;
}

