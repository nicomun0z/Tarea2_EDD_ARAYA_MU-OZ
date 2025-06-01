#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;


class Jugador {
private:
    int vida;
    int ataque;
    float precision;
    int recuperacion;

public:
    Jugador(int v, int a, float p, int r)
        : vida(v), ataque(a), precision(p), recuperacion(r) {}

    int getVida() { return vida; }
    int getAtaque() { return ataque; }
    float getPrecision() { return precision; }
    int getRecuperacion() { return recuperacion; }

    void setVida(int v) { vida = v; }
    void setAtaque(int a) { ataque = a; }
    void setPrecision(float p) { precision = p; }
    void setRecuperacion(int r) { recuperacion = r; }

    void atacar(class Enemigo& enemigo);
    void curar() {
        vida += recuperacion;
        cout << "Vida recuperada. Vida actual: " << vida << endl;
    }

    void mostrarEstado() {
        cout << "Vida: " << vida << ", Ataque: " << ataque
             << ", Precisión: " << precision * 100 << "%, Recuperación: " << recuperacion << endl;
    }
};


class Enemigo {
private:
    string nombre;
    int vida;
    int ataque;
    float precision;
    float probabilidad;

public:
    Enemigo(string n, int v, int a, float p, float prob)
        : nombre(n), vida(v), ataque(a), precision(p), probabilidad(prob) {}

    string getNombre() { return nombre; }
    int getVida() { return vida; }
    int getAtaque() { return ataque; }
    float getPrecision() { return precision; }
    float getProbabilidad() { return probabilidad; }

    void setVida(int v) { vida = v; }
    void setAtaque(int a) { ataque = a; }
    void setPrecision(float p) { precision = p; }
    void setProbabilidad(float prob) { probabilidad = prob; }

    void atacar(Jugador& jugador) {
        if (rand() % 100 < precision * 100) {
            jugador.setVida(jugador.getVida() - ataque);
            cout << nombre << " golpea al jugador por " << ataque << " de daño!" << endl;
        } else {
            cout << nombre << " falla su ataque!" << endl;
        }
    }

    bool estaVivo() { return vida > 0; }

    void mostrarEstado() {
        cout << "Nombre: " << nombre << ", Vida: " << vida
             << ", Ataque: " << ataque << ", Precisión: " << precision * 100 << "%"
             << ", Probabilidad de ataque: " << probabilidad * 100 << "%" << endl;
    }
};


void Jugador::atacar(Enemigo& enemigo) {
    if (rand() % 100 < precision * 100) {
        enemigo.setVida(enemigo.getVida() - ataque);
        cout << "¡Jugador golpea a " << enemigo.getNombre() << " por " << ataque << " de daño!" << endl;
    } else {
        cout << "¡Jugador falla su ataque!" << endl;
    }
}


struct NodoEnemigo {
    Enemigo* enemigo;
    NodoEnemigo* siguiente;
    NodoEnemigo(Enemigo* e) : enemigo(e), siguiente(nullptr) {}
};

class ColaEnemigos {
private:
    NodoEnemigo* frente;
    NodoEnemigo* final;

public:
    ColaEnemigos() : frente(nullptr), final(nullptr) {}

    ~ColaEnemigos() {
        while (!estaVacia()) {
            desencolar();
        }
    }

    void encolar(Enemigo* enemigo) {
        NodoEnemigo* nuevo = new NodoEnemigo(enemigo);
        if (estaVacia()) {
            frente = final = nuevo;
        } else {
            final->siguiente = nuevo;
            final = nuevo;
        }
    }

    Enemigo* desencolar() {
        if (estaVacia()) return nullptr;
        NodoEnemigo* temp = frente;
        Enemigo* enemigo = temp->enemigo;
        frente = frente->siguiente;
        delete temp;
        if (frente == nullptr) final = nullptr;
        return enemigo;
    }

    Enemigo* obtenerFrente() {
        if (estaVacia()) return nullptr;
        return frente->enemigo;
    }

    bool estaVacia() {
        return frente == nullptr;
    }
};


class Habitacion {
private:
    string nombre;
    string descripcion;
    string tipo; 

public:
    Habitacion* izquierda;
    Habitacion* centro;
    Habitacion* derecha;

    Habitacion(string n, string d, string t)
        : nombre(n), descripcion(d), tipo(t),
          izquierda(nullptr), centro(nullptr), derecha(nullptr) {}

    void mostrarDescripcion() {
        cout << "-- " << nombre << " --" << endl;
        cout << descripcion << endl;
    }

    string getTipo() { return tipo; }
    string getNombre() { return nombre; }
};


