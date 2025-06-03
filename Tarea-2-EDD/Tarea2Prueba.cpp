#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Enemigo {
    string nombre;
    int vida, ataque;
    float precision, probabilidad;
    
    Enemigo() : vida(0), ataque(0), precision(0.0), probabilidad(0.0) {}
    Enemigo(string n, int v, int a, float p, float prob)
        : nombre(n), vida(v), ataque(a), precision(p), probabilidad(prob) {}
};

struct Efecto {
    string descripcion, modificador;
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
    string nombre, descripcion;
    float probabilidad;
    Opcion opcionA, opcionB;
    Evento() : probabilidad(0.0) {}
};

struct Habitacion {
    int id;
    string nombre, tipo, descripcion;
    Habitacion* izquierda;
    Habitacion* centro;
    Habitacion* derecha;
    
    Habitacion() : id(-1), izquierda(nullptr), centro(nullptr), derecha(nullptr) {}
    Habitacion(int i, string n, string t, string d) 
        : id(i), nombre(n), tipo(t), descripcion(d), 
          izquierda(nullptr), centro(nullptr), derecha(nullptr) {}
};

struct Arco {
    int origen, destino;
    Arco(int o, int d) : origen(o), destino(d) {}
};

template<typename T>
struct Nodo {
    T* dato;
    Nodo* siguiente;
    Nodo(T* d) : dato(d), siguiente(nullptr) {}
};

template<typename T>
class Lista {
private:
    Nodo<T>* cabeza;
    
public:
    Lista() : cabeza(nullptr) {}
    
    ~Lista() {
        while (cabeza) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp->dato;
            delete temp;
        }
    }
    
    void agregar(T* elemento) {
        Nodo<T>* nuevo = new Nodo<T>(elemento);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    
    Nodo<T>* getCabeza() { return cabeza; }
    
    T* buscarPorId(int id) {
        for (Nodo<T>* actual = cabeza; actual; actual = actual->siguiente) {
            if (actual->dato->id == id) return actual->dato;
        }
        return nullptr;
    }
};

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
        while (cabeza) {
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
    
    NodoMejora* getCabeza() { return cabeza; }
};

struct NodoCombate {
    Enemigo* enemigo;
    NodoCombate* siguiente;
    NodoCombate(Enemigo* e) : enemigo(new Enemigo(*e)), siguiente(nullptr) {}
};

class ColaCombate {
private:
    NodoCombate* frente;
    NodoCombate* final;
    
public:
    ColaCombate() : frente(nullptr), final(nullptr) {}
    
    ~ColaCombate() {
        while (!estaVacia()) desencolar();
    }
    
    void encolar(Enemigo* enemigo) {
        NodoCombate* nuevo = new NodoCombate(enemigo);
        if (!final) {
            frente = final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
    }
    
    void desencolar() {
        if (!frente) return;
        NodoCombate* temp = frente;
        frente = frente->siguiente;
        if (!frente) final = nullptr;
        delete temp->enemigo;
        delete temp;
    }
    
    Enemigo* obtenerFrente() { return frente ? frente->enemigo : nullptr; }
    bool estaVacia() const { return !frente; }
    NodoCombate* getCabeza() { return frente; }
    
    void mostrarEnemigos() {
        for (NodoCombate* actual = frente; actual; actual = actual->siguiente) {
            cout << (actual->enemigo->vida > 0 ? actual->enemigo->nombre : "X") << " | ";
        }
    }
    
    void eliminarMuertos() {
        while (frente && frente->enemigo->vida <= 0) 
            desencolar();
        
        if (frente) {
            for (NodoCombate* actual = frente; actual->siguiente;) {
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
};

class Jugador {
private:
    int vida, ataque, recuperacion;
    float precision;
    
public:
    Jugador(int v = 30, int a = 7, float p = 0.8, int r = 3) 
        : vida(v), ataque(a), recuperacion(r), precision(p) {} 
    
    int getVida() const { return vida; }
    int getAtaque() const { return ataque; }
    float getPrecision() const { return precision; }
    int getRecuperacion() const { return recuperacion; }
    
    void curar(int cantidad) { vida += cantidad; }
    void recibirDano(int dano) { vida = max(0, vida - dano); }
    void aumentarAtaque(int cantidad) { ataque += cantidad; }
    void aumentarPrecision(float cantidad) { precision = min(1.0f, precision + cantidad); }
    void aumentarRecuperacion(int cantidad) { recuperacion += cantidad; }
    void setPrecision(float p) { precision = max(0.0f, p); }
    
    bool estaVivo() const { return vida > 0; }
    
    void mostrarEstado() const {
        cout << "Estado - Vida: " << vida << ", Ataque: " << ataque 
             << ", Precisión: " << precision << ", Recuperación: " << recuperacion << endl;
    }
};

Lista<Habitacion>* habitaciones = nullptr;
Lista<Arco>* arcos = nullptr;
Lista<Enemigo>* enemigos = nullptr;
Lista<Evento>* eventos = nullptr;
ListaMejoras* mejoras = nullptr;
Habitacion* raizArbol = nullptr;

struct NodoPila {
    Habitacion* habitacion;
    NodoPila* siguiente;
    NodoPila(Habitacion* h) : habitacion(h), siguiente(nullptr) {}

};
class Pila{
private:
    NodoPila* tope;
public:
    Pila() : tope(nullptr) {}
    ~Pila() {
        while (tope){
           NodoPila* t = tope;
           tope = tope->siguiente;
           delete t;
        }
    }
    void push(Habitacion* h){
        NodoPila* nuevo = new NodoPila(h);
        nuevo->siguiente = tope;
        tope = nuevo;
    }
    void pop(){
        if (tope) {
            NodoPila* temp = tope;
            tope = tope->siguiente;
            delete temp;
        }
    }
    Habitacion* top(){
        if(tope == nullptr){
            return nullptr;
        }
        return tope->habitacion;

    }
    Habitacion* abuelo(){
        if(tope && tope->siguiente && tope->siguiente->siguiente){
            return tope->siguiente->siguiente->habitacion;
        }
        return nullptr;
    }
};
/* ****
* void eliminarEspacios
****
* Elimina espacios y caracteres de control al inicio y final de una cadena
****
* Input: (string& str) Referencia a la cadena que se va a modificar
****
* Returns: 
* void, modifica la cadena pasada por referencia 
**** */
void eliminarEspacios(string& str) {
    size_t inicio = str.find_first_not_of(" \t\r\n");
    if (inicio == string::npos) {
        str = "";
        return;
    }
    size_t fin = str.find_last_not_of(" \t\r\n");
    str = str.substr(inicio, fin - inicio + 1);
}
/* ****
* void dividirLinea
****
* Separa una cadena en partes usando un delimitador especifico
****
* Input:
* const string& linea: cadena a ser dividida
*char delimitador: caracter a ser usado como separador de cadenas
*string partes[]: arreglo donde se almacenan las partes
*int& cantidad: referencia a donde se guardara el numero de partes encontradas
****
* Returns: 
* void, modifica el arreglo partes, y la variable cantidad
**** */
void dividirLinea(const string& linea, char delimitador, string partes[], int& cantidad) {
    stringstream ss(linea);
    string parte;
    cantidad = 0;
    while (getline(ss, parte, delimitador)) {
        eliminarEspacios(parte);
        partes[cantidad++] = parte;
    }
}
/* ****
* void leerHabitaciones
****
* Lee cada habitacion (ID, Nombre, tipo, descripcion) y la agrega a la listaHabitaciones
****
* Input: 
* ifstream &archivo: referencia al archivo donde se leeran las habitaciones
* int cantidad: entero con la cantidad de habitaciones
****
* Returns: 
* void, modifica la lista habitaciones y agrega lo asociado a estas
*** */
void leerHabitaciones(ifstream& archivo, int cantidad) {
    habitaciones = new Lista<Habitacion>();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        if (!getline(archivo, linea)) break;
        eliminarEspacios(linea);
        
        stringstream ss(linea);
        string idStr;
        ss >> idStr;
        int id = atoi(idStr.c_str());
        
        size_t inicioTipo = linea.find('(');
        size_t finTipo = linea.find(')');
        
        string nombre, tipo;
        if (inicioTipo != string::npos && finTipo != string::npos) {
            tipo = linea.substr(inicioTipo + 1, finTipo - inicioTipo - 1);
            size_t inicioNombre = linea.find_first_not_of(" ", idStr.length());
            nombre = linea.substr(inicioNombre, inicioTipo - inicioNombre);
            eliminarEspacios(nombre);
            eliminarEspacios(tipo);
        }
        
        string descripcion;
        if (getline(archivo, descripcion)) eliminarEspacios(descripcion);
        
        habitaciones->agregar(new Habitacion(id, nombre, tipo, descripcion));
    }
}
/* ****
* void leerArcos
****
* Lee conexiones entre habitaciones en el archivo
****
* Input: 
*ifstream& archivo: referencia al archivo que sera modificado
int cantidad: entero con la cantidad de arcos por leer
****
* Returns:
* void, crea una lista con las conexiones entre las habitaciones 
**** */
void leerArcos(ifstream& archivo, int cantidad) {
    arcos = new Lista<Arco>();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        if (!getline(archivo, linea)) break;
        eliminarEspacios(linea);
        
        size_t flecha = linea.find("->");
        if (flecha != string::npos) {
            string origenStr = linea.substr(0, flecha);
            string destinoStr = linea.substr(flecha + 2);
            eliminarEspacios(origenStr);
            eliminarEspacios(destinoStr);
            
            arcos->agregar(new Arco(atoi(origenStr.c_str()), atoi(destinoStr.c_str())));
        }
    }
}
/* ****
* void leerEnemigos
****
* Lee enemigos y sus estadisticas
****
* Input:
* ifstream& archivo: Referencia al archivo que se leera
* int cantidad: entero con la cantidad de enemigos 
****
* returns:
* void, crea una nueva lista que almacena a los enemigos con sus estadisticas correspondientes
**** */
void leerEnemigos(ifstream& archivo, int cantidad) {
    enemigos = new Lista<Enemigo>();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        if (!getline(archivo, linea)) break;
        eliminarEspacios(linea);
        
        string partes[5];
        int numPartes;
        dividirLinea(linea, '|', partes, numPartes);
        
        if (numPartes >= 5) {
            string nombre = partes[0];
            int vida, ataque;
            float precision, probabilidad;
            
            sscanf(partes[1].c_str(), "Vida %d", &vida);
            sscanf(partes[2].c_str(), "Ataque %d", &ataque);
            sscanf(partes[3].c_str(), "Precision %f", &precision);
            sscanf(partes[4].c_str(), "Probabilidad %f", &probabilidad);
            
            enemigos->agregar(new Enemigo(nombre, vida, ataque, precision, probabilidad));
        }
    }
}
/* ****
* void leerEventos
****
* Lee los eventos del archivo, opciones y efectos
****
* Input: 
* ifstream& archivo: referencia al archivo que sera leido
* int cantidad: entero con la cantidad de eventos
**** 
Returns:
* void, crea una lista de los eventos con las opciones y efectos correspondientes de cada uno
**** */
void leerEventos(ifstream& archivo, int cantidad) {
    eventos = new Lista<Evento>();
    string linea;
    
    for (int i = 0; i < cantidad; i++) {
        while (getline(archivo, linea)) {
            eliminarEspacios(linea);
            if (linea == "&") break;
        }
        
        Evento* nuevo = new Evento();
        
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->nombre = linea;
        }
        
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            sscanf(linea.c_str(), "Probabilidad %f", &nuevo->probabilidad);
        }
        
        if (getline(archivo, linea)) {
            eliminarEspacios(linea);
            nuevo->descripcion = linea;
        }
        
        string opciones[6];
        for (int j = 0; j < 6; j++) {
            if (getline(archivo, linea)) {
                eliminarEspacios(linea);
                opciones[j] = linea;
            }
        }
        
        nuevo->opcionA.enunciado = opciones[0].substr(2);
        nuevo->opcionA.efecto.descripcion = opciones[1];
        nuevo->opcionA.efecto.modificador = opciones[2];
        nuevo->opcionB.enunciado = opciones[3].substr(2);
        nuevo->opcionB.efecto.descripcion = opciones[4];
        nuevo->opcionB.efecto.modificador = opciones[5];
        
        eventos->agregar(nuevo);
    }
}
/* ****
* void leerMejoras
****
* Lee mejoras disponibles en el archivo
****
* Input:
* ifstream& archivo: referencia al archivo por leer
****
* Returns: 
* void, crea una lista de mejoras y agrega las mejoras leidas en la lista creada
**** */
void leerMejoras(ifstream& archivo) {
    mejoras = new ListaMejoras();
    string linea;
    
    while (getline(archivo, linea)) {
        eliminarEspacios(linea);
        if (linea == "FIN DE ARCHIVO" || linea.empty()) break;
        mejoras->agregar(linea);
    }
}
/* ****
* void construirArbol
****
* Contruye el arbol ternario usando las conexiones leidas en el archivo
****
* Input: 
* Ninguno (utiliza las listas de habitaciones y arcos)
****
* Returns:
* void, establece las conexiones y define la raiz 
**** */
void construirArbol() {
    if (!habitaciones || !arcos) return;
    
    for (Nodo<Habitacion>* actual = habitaciones->getCabeza(); actual; actual = actual->siguiente) {
        if (actual->dato->tipo == "INICIO") {
            raizArbol = actual->dato;
            break;
        }
    }
    
    for (Nodo<Arco>* arcoActual = arcos->getCabeza(); arcoActual; arcoActual = arcoActual->siguiente) {
        Habitacion* origen = habitaciones->buscarPorId(arcoActual->dato->origen);
        Habitacion* destino = habitaciones->buscarPorId(arcoActual->dato->destino);
        
        if (origen && destino) {
            if (!origen->izquierda) origen->izquierda = destino;
            else if (!origen->centro) origen->centro = destino;
            else if (!origen->derecha) origen->derecha = destino;
        }
    }
}
/* ****
* void cargarMapa
****
* Carga el archivo y detecta cada seccion, cada una se lee y se almacenan las variables correspondientes a la seccion
****
* Input:
* const char* nombreArchivo: puntero con el nombre del archivo
****
* Returns:
* void, filtra por seccion y llama a las funciones de leer para almacenar en las listas correspondientes para luego
  llamar a construirArbol para enlazar las habitaciones con los arcos
**** */
void cargarMapa(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }
    
    string linea;
    while (getline(archivo, linea)) {
        eliminarEspacios(linea);
        if (linea == "INICIO DE ARCHIVO") break;
    }
    
    while (getline(archivo, linea)) {
        eliminarEspacios(linea);
        int cantidad;
        
        if (linea == "HABITACIONES") {
            archivo >> cantidad; archivo.ignore();
            leerHabitaciones(archivo, cantidad);
        } else if (linea == "ARCOS") {
            archivo >> cantidad; archivo.ignore();
            leerArcos(archivo, cantidad);
        } else if (linea == "ENEMIGOS") {
            archivo >> cantidad; archivo.ignore();
            leerEnemigos(archivo, cantidad);
        } else if (linea == "EVENTOS") {
            archivo >> cantidad; archivo.ignore();
            leerEventos(archivo, cantidad);
        } else if (linea == "MEJORAS DE COMBATE") {
            leerMejoras(archivo);
        } else if (linea == "FIN DE ARCHIVO") {
            break;
        }
    }
    
    archivo.close();
    construirArbol();
    cout << "Archivo cargado exitosamente!" << endl;
}
/* ****
* void selecionarEnemigosAleatorios
****
* selecciona enemigos aleatorios para un combate segun su probabilidad de aparecer
****
* Input:
* ColaCombate& cola: referencia a una Cola donde se agregaran los enemigos seleccionados para el combate
****
* Returns:
* void, modifica la colaCombate agregando los enemigos seleccionados
**** */
void seleccionarEnemigosAleatorios(ColaCombate& cola) {
    if (!enemigos) return;
    
    int cantidadEnemigos = (rand() % 3) + 1;
    
    for (int i = 0; i < cantidadEnemigos; i++) {
        float probabilidadTotal = 0.0;
        for (Nodo<Enemigo>* temp = enemigos->getCabeza(); temp; temp = temp->siguiente) {
            probabilidadTotal += temp->dato->probabilidad;
        }
        
        float seleccion = ((float)rand() / RAND_MAX) * probabilidadTotal;
        float acumulado = 0.0;
        
        for (Nodo<Enemigo>* temp = enemigos->getCabeza(); temp; temp = temp->siguiente) {
            acumulado += temp->dato->probabilidad;
            if (seleccion <= acumulado) {
                cola.encolar(temp->dato);
                break;
            }
        }
    }
}
/* ****
* bool realizarCombate
****
* Crea una cola, la que luego llama a la funcion seleccionarEnemigosAleatorios()
  y ejecuta el combate imprimiendo los ataques y la vida correspondiente
****
* Input: 
* Jugador& jugador: referencia al objeto Jugador, que sera modificado durante el combate
****
* Returns: 
* True: si el jugador sobrevive
* False: si el jugador muere
**** */
bool realizarCombate(Jugador& jugador) {
    ColaCombate cola;
    seleccionarEnemigosAleatorios(cola);
    
    if (cola.estaVacia()) {
        cout << "No hay enemigos en esta habitación." << endl;
        return true;
    }
    
    cout << "¡Encuentras enemigos!" << endl;
    
    while (!cola.estaVacia() && jugador.estaVivo()) {
        cout << "Jugador | ";
        cola.mostrarEnemigos();
        cout << "\n" << jugador.getVida() << " | ";
        
        for (NodoCombate* temp = cola.getCabeza(); temp; temp = temp->siguiente) {
            cout << (temp->enemigo->vida > 0 ? temp->enemigo->vida : 0) << " | ";
        }
        cout << endl;
        
        Enemigo* objetivo = cola.obtenerFrente();
        if (objetivo && objetivo->vida > 0) {
            if (((float)rand() / RAND_MAX) <= jugador.getPrecision()) {
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
        
        cola.eliminarMuertos();
        if (cola.estaVacia()) break;
        
        for (NodoCombate* enemigo = cola.getCabeza(); enemigo && jugador.estaVivo(); enemigo = enemigo->siguiente) {
            if (enemigo->enemigo->vida > 0) {
                if (((float)rand() / RAND_MAX) <= enemigo->enemigo->precision) {
                    jugador.recibirDano(enemigo->enemigo->ataque);
                    cout << enemigo->enemigo->nombre << " golpea por " 
                         << enemigo->enemigo->ataque << " de daño!" << endl;
                } else {
                    cout << enemigo->enemigo->nombre << " falla!" << endl;
                }
            }
        }
    }
    
    if (jugador.estaVivo()) {
        cout << "¡Has sobrevivido el combate!" << endl;
        jugador.curar(jugador.getRecuperacion());
        cout << "Recuperas " << jugador.getRecuperacion() << " de vida." << endl;
        return true;
    } else {
        cout << "¡Has sido derrotado! GAME OVER" << endl;
        return false;
    }
}
/* ****
* void aplicarEfecto
****
* Analiza el modificador del efecto y segun este aplica el efecto al jugador
****
* Input:
* Jugador& jugador: referencia al jugador, que sera modificado durante esta funcion
* const Efecto& efecto: estructura que coontiene la descripcion y el modificador
****
* Returns: 
* void, solo modifica al jugador
**** */
void aplicarEfecto(Jugador& jugador, const Efecto& efecto) {
    string mod = efecto.modificador;
    
    if (mod.find("Ninguna consecuencia") != string::npos) {
        cout << "No hay consecuencias." << endl;
        return;
    }
    
    int cantidad = 0;
    float cantidadFloat = 0.0;
    
    if (mod.find("+") != string::npos) {
        if (mod.find("Vida") != string::npos) {
            sscanf(mod.c_str(), "+%d Vida", &cantidad);
            jugador.curar(cantidad);
            cout << "Ganas " << cantidad << " de vida!" << endl;
        } else if (mod.find("Ataque") != string::npos) {
            sscanf(mod.c_str(), "+%d Ataque", &cantidad);
            jugador.aumentarAtaque(cantidad);
            cout << "Tu ataque aumenta en " << cantidad << "!" << endl;
        } else if (mod.find("Precision") != string::npos || mod.find("PRECISION") != string::npos) {
            sscanf(mod.c_str(), "+%f", &cantidadFloat);
            jugador.aumentarPrecision(cantidadFloat);
            cout << "Tu precisión aumenta!" << endl;
        } else if (mod.find("Recuperacion") != string::npos) {
            sscanf(mod.c_str(), "+%d Recuperacion", &cantidad);
            jugador.aumentarRecuperacion(cantidad);
            cout << "Tu recuperación aumenta en " << cantidad << "!" << endl;
        }
    } else if (mod.find("-") != string::npos) {
        if (mod.find("Vida") != string::npos) {
            sscanf(mod.c_str(), "-%d Vida", &cantidad);
            jugador.recibirDano(cantidad);
            cout << "Pierdes " << cantidad << " de vida!" << endl;
        } else if (mod.find("PRECISION") != string::npos || mod.find("Precision") != string::npos) {
            sscanf(mod.c_str(), "-%f", &cantidadFloat);
            jugador.setPrecision(jugador.getPrecision() - cantidadFloat);
            cout << "Tu precisión disminuye!" << endl;
        }
    }
}
/* ****
* Evento *seleccionarEventoAleatorio 
****
* Suma las probabilidades de cada evento y genera un numero aleatorio entre 0 y la suma total de probabilidades 
  y recorre la lista de eventos para luego seleccionar uno segun su probabilidad acumulada
****
* Input: 
* Ninguno. no recibe parametros
****
* Returns: 
* Retorna un puntero al evento seleccionado aleatoriamente , si no hay evento o la probabilidad total es 0 returna nullptr
**** */
Evento* seleccionarEventoAleatorio() {
    if (!eventos) return nullptr;
    
    float probabilidadTotal = 0.0;
    for (Nodo<Evento>* actual = eventos->getCabeza(); actual; actual = actual->siguiente) {
        probabilidadTotal += actual->dato->probabilidad;
    }
    
    if (probabilidadTotal <= 0) return nullptr;
    
    float seleccion = ((float)rand() / RAND_MAX) * probabilidadTotal;
    float acumulado = 0.0;
    
    for (Nodo<Evento>* actual = eventos->getCabeza(); actual; actual = actual->siguiente) {
        acumulado += actual->dato->probabilidad;
        if (seleccion <= acumulado) return actual->dato;
    }
    
    return nullptr;
}
/* ****
* bool manejar Evento
**** 
* muestra la descripcion del evento y las opciones A/B, pide al usuario que eliga una opcion, aplica el efecto de la opcion al jugador
****
* Input:
* Jugador& jugador: Referencia al jugador que sera modificado
**** 
* Returns:
* True: si el jugador sigue vivo tras el evento
* False: si el jugador muere tras el evento
**** */
bool manejarEvento(Jugador& jugador) {
    Evento* evento = seleccionarEventoAleatorio();
    if (!evento) {
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
/* ****
* void mostrarYAplicarMejora
****
* Muestra todas las mejoras disponibles, recorriendo la listaMejoras y pide al usuario que eliga una de estas para luego aplicar su efecto a el usuario
**** 
* Input: 
* Jugador& jugador: Referencia al jugador, que sera modificado durante esta funcion
****
* Returns: 
* void, Modifica al jugador y muestra mensajes
**** */
void mostrarYAplicarMejora(Jugador& jugador) {
    if (!mejoras) return;
    
    cout << "Elige una mejora:" << endl;
    int contador = 1;
    for (NodoMejora* actual = mejoras->getCabeza(); actual; actual = actual->siguiente) {
        cout << contador << ". " << actual->mejora << endl;
        contador++;
    }
    
    int opcion;
    cout << "Elige tu mejora: ";
    cin >> opcion;
    
    contador = 1;
    for (NodoMejora* actual = mejoras->getCabeza(); actual; actual = actual->siguiente) {
        if (contador == opcion) {
            string mejora = actual->mejora;
            int cantidad = 0;
            float cantidadFloat = 0.0;
            
            if (mejora.find("Vida") != string::npos) {
                sscanf(mejora.c_str(), "+%d Vida", &cantidad);
                jugador.curar(cantidad);
                cout << "Recuperaste " << cantidad << " de vida!" << endl;
            } else if (mejora.find("Precision") != string::npos) {
                sscanf(mejora.c_str(), "+%f Precision", &cantidadFloat);
                jugador.aumentarPrecision(cantidadFloat);
                cout << "Tu precisión aumentó!" << endl;
            } else if (mejora.find("Ataque") != string::npos) {
                sscanf(mejora.c_str(), "+%d Ataque", &cantidad);
                jugador.aumentarAtaque(cantidad);
                cout << "Tu ataque aumentó en " << cantidad << "!" << endl;
            } else if (mejora.find("Recuperacion") != string::npos) {
                sscanf(mejora.c_str(), "+%d Recuperacion", &cantidad);
                jugador.aumentarRecuperacion(cantidad);
                cout << "Tu recuperación aumentó en " << cantidad << "!" << endl;
            }
            break;
        }
        contador++;
    }
}
/* ****
* Habitacion* seleccionarSiguienteHabitacion
****
* Muestra las habitaciones conectadas a la actual y pide al usuario que eliga a cual ir
****
* Input: 
* Habitacion* actual: puntero que apunta a la Habitacion actual
**** 
* Returns: 
* Puntero a la habitacion elegida por el usuario, si no es valida nullptr
**** */
Habitacion* seleccionarSiguienteHabitacion(Habitacion* actual) {
    if (!actual) return nullptr;
    
    cout << "¿A dónde quieres ir?" << endl;
    int opcion = 1;
    
    if (actual->izquierda) {
        cout << opcion << ". " << actual->izquierda->nombre << endl;
        opcion++;
    }
    if (actual->centro) {
        cout << opcion << ". " << actual->centro->nombre << endl;
        opcion++;
    }
    if (actual->derecha) {
        cout << opcion << ". " << actual->derecha->nombre << endl;
    }
    
    int seleccion;
    cout << "Ingresa tu opción: ";
    cin >> seleccion;
    
    int contador = 1;
    if (actual->izquierda && contador == seleccion) return actual->izquierda;
    if (actual->izquierda) contador++;
    if (actual->centro && contador == seleccion) return actual->centro;
    if (actual->centro) contador++;
    if (actual->derecha && contador == seleccion) return actual->derecha;
    
    return nullptr;
}
/* ****
* void jugar 
**** 
* Inicializa el jugador y la habitacion actual(raiz), Usa una pila para guardar el camino recorrido
  En cada Iteracion: Muestra el nombre y descripcion de la habitacion actual, si es de tipo "FIN" termina el juego, "COMBATE" realiza el combate y aplica mejora si sobrevive
  , "EVENTO" ejecuta un evento aleatorio, muestra el estado del jugador, si hay habitaciones conectadas te da la opcion de elegir la siguiente
  si no hay mas caminos termina el juego
**** 
* INPUT: 
* no recibe ningun parametro
**** 
* Returns: 
* void, controla el flujo del juego
**** */
void jugar() {
    if (!raizArbol) {
        cout << "Error: No se pudo cargar el mapa del juego." << endl;
        return;
    }
    
    Jugador jugador;
    Habitacion* habitacionActual = raizArbol;
    Pila camino;
    
    cout << "=== BIENVENIDO AL JUEGO ===" << endl;
    int cont = 0;
    while (habitacionActual && jugador.estaVivo()) {
        camino.push(habitacionActual);
        cout << "\n--- " << habitacionActual->nombre << " ---" << endl;
        cout << habitacionActual->descripcion << endl;
        if (habitacionActual->tipo == "FIN") {
            cout << "\n¡Has llegado al final de tu aventura!" << endl;
            break;
        }
        bool continuar = true;

        if (habitacionActual->tipo == "COMBATE") {
            continuar = realizarCombate(jugador);
            if (continuar){
                mostrarYAplicarMejora(jugador);
                cont ++;
            }
        } else if (habitacionActual->tipo == "EVENTO") {
            continuar = manejarEvento(jugador);
            cont ++;
        }

        if (!continuar) break;

        jugador.mostrarEstado();
        if( cont>= 2 && (rand() % 100 < 10)){
            cont = 0;
            cout << "\n¡Una puerta aparece!\n";
            Habitacion* abuelo = camino.abuelo();
            if(abuelo){
                cout << "¿Entras por la puerta? " << abuelo->nombre << " ? (SI/NO): ";
                string eleccion;
                cin >> eleccion;
                for (char& c : eleccion){
                    c = toupper(c);
                }
                if(eleccion == "SI"){
                    Habitacion* aux = abuelo;
                    cout << "\n¡Has viajado en el tiempo y has regresado a la habitación: " << aux->nombre << "!\n";
                    habitacionActual = aux;
                    continue;
                }else{
                    cout << "Decides no volver...\n";
                }
            }

        }
        if (habitacionActual->izquierda || habitacionActual->centro || habitacionActual->derecha) {
            habitacionActual = seleccionarSiguienteHabitacion(habitacionActual);
        } else {
            cout << "No hay más caminos disponibles." << endl;
            break;
        }
        
    }

    
    cout << (jugador.estaVivo() ? "\n¡Gracias por jugar!" : "\n¡GAME OVER!") << endl;
}

void liberarMemoria() {
    delete habitaciones;
    delete arcos;
    delete enemigos;
    delete eventos;
    delete mejoras;
}

int main() {
    srand(time(nullptr));
    
    cout << "=== LECTOR DE ARCHIVO .MAP ===" << endl;
    cout << "Cargando archivo data.map..." << endl;
    
    cargarMapa("data.map");
    jugar();
    liberarMemoria();
    
    return 0;
}