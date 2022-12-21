#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// constante con el n� de casillas del tablero
const int NUM_CASILLAS = 63;

// constantes para los turnos de penalización
const int TURNOS_POSADA = 1;
const int TURNOS_CARCEL = 2;
const int TURNOS_POZO = 3;

// constante para el tipo de ejecución a usar
// (false) es sin hacer nada y (true) es tirando tu
const int MODO_DEBUG = true;

// constante para establecer el n� de jugadores (dar valor entre 2 y 4)
const int NUM_JUGADORES = 2;

// tipos
typedef enum {
    NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA, LABERINTO, DADO1, DADO2, CARCEL, CALAVERA
} tCasilla;

typedef tCasilla tTablero[NUM_CASILLAS];

struct tEstadoJugador {
    int casilla;
    int penalizaciones;
};

typedef tEstadoJugador tEstadoJugadores[NUM_JUGADORES];

struct tEstadoPartida {
    tEstadoJugadores estadoJug;
    int turno = -1;
    tTablero tablero;
};

const int MAX_PARTIDAS = 10;
typedef tEstadoPartida tArrayPartidas[MAX_PARTIDAS];

struct tListaPartidas {
    int cont;
    tArrayPartidas listaPartidas;
};

// Subprogramas que se heredan de la v1
int tirarDado();
int tirarDadoManual();
bool esMeta(int casilla);
int quienEmpieza();
void cambiaTurno(int &turno); // si no lo ten�is de la versi�n 1, implementadlo ahora
void actualizarTurno(int& turno, tEstadoJugadores estadoJug);

// Subprogramas para configurar el tablero heredados de la v2
bool cargaTablero(tTablero tablero);
void cargaTableroNormal(tTablero tablero);
void iniciaTablero(tTablero tablero);

// Subprogramas que sustituyen a otros implementados en la versi�n 1
// relacionados con la identificación de casillas especiales y con el efecto provocado por las mismas
bool esCasillaSalto(const tTablero tablero, int casilla);
int saltaACasilla(const tTablero tablero, int casillaActual);
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int &posicion);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int &posicion);
void efectoTirada(const tTablero tablero,tEstadoJugador &jugador);    // NUEVO PROTOTIPO EN V3; SUSTITUYE A    void efectoTirada(const tTablero tablero, int& casillaJ, int& penalizacionJ);

// Subprogramas para la lógica de las tiradas y de la partida
void iniciaJugadores(tEstadoJugadores jugadores);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void iniciaJugadores(tJugadores casillasJ, tJugadores penalizacionesJ);
void tirada(const tTablero tablero,tEstadoJugador &estadoJug);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A   void tirada(const tTablero tablero, int& casillaActual, int& penalizacion);
int partida(tEstadoPartida &estado);  // NUEVO PROTOTIPO EN V3; SUSTITUYE A   int partida(const tTablero tablero);
void juego(tEstadoPartida &estado);

// Subprogramas para la gestion de la lista de partidas abandonadas
bool cargaPartidas(tListaPartidas &partidas);
void cargarTablero(tTablero tablero, ifstream &archivo);
void cargaJugadores(tEstadoJugadores &jugadores, ifstream &archivo);
void eliminarPartida(tListaPartidas &partidas, int indice);
bool insertaNuevaPartida(tListaPartidas &partidas, const tEstadoPartida &partidaOca);
void guardaPartidas(const tListaPartidas &partidas);
void guardaTablero(const tTablero tablero, ofstream &archivo);
void guardaJugadores(const tEstadoJugadores jugadores, ofstream &archivo);
void menuNuevaPartida(tListaPartidas &partidas);
void menuCargarPartida(tListaPartidas &partidas);

// Subprograma para pintar el tablero...
void pintaTablero(const tEstadoPartida &partida);   // NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaTablero(const tTablero tablero, const tJugadores casillasJ);
// y sus auxiliares
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila); // NUEVO PROTOTIPO EN V3; SUSTITUYE A  void pintaJugadores(const tJugadores casillasJ, int fila, int casillasPorFila);
string casillaAstring(tCasilla casilla);


int main() {
    tListaPartidas partidas;
    char commando;
    bool abierto, salir = false;
    abierto = cargaPartidas(partidas);
    if(abierto){
        do{
            do {
                cout << "Estos son los comandos posibles: " << endl;
                cout << "N = Crear nueva partida" << endl << "E = Cargar partida existente" << endl << "S = Salir del programa y guardar las partidas en un fichero" << endl;
                cout << "Que deseas hacer?: ";
                cin >> commando;
                if(commando != 'N' && commando != 'E' && commando != 'S'){ cout << "Introduce una letra válida";}
            } while(commando != 'N' && commando != 'E' && commando != 'S');
            switch (commando) {
                case 'N':
                    menuNuevaPartida(partidas);
                    break;
                case 'E':
                    menuCargarPartida(partidas);
                    break;
                case 'S':
                    salir = true;
                    break;
                default:
                    break;
            }
        }while(!salir);
        guardaPartidas(partidas);
    }

    return 0;
}

//--------------------------------------
// CÓDIGO PROPORCIONADO CON LA PLANTILLA
//--------------------------------------

// Subprograma para pintar el tablero...
void pintaTablero(const tEstadoPartida &partida) {

    int numFilas = 3;
    int casillasPorFila = NUM_CASILLAS / numFilas; // Es 21


    cout << endl;
    for (int fila = 0; fila < numFilas; fila++) {
        pintaBorde(casillasPorFila);
        pintaNumCasilla(fila, casillasPorFila);
        pintaTipoCasilla(partida.tablero, fila, casillasPorFila);
        pintaJugadores(partida.estadoJug, fila, casillasPorFila);
    }

    pintaBorde(casillasPorFila);
    cout << endl;


}
// y sus auxiliares
void pintaBorde(int casillasPorFila) {
    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|====";
    cout << "|" << endl;
}
void pintaNumCasilla(int fila, int casillasPorFila) {
    for (int i = 1; i <= casillasPorFila; i++)
        cout << "| " << setw(2) << setfill('0') << i + fila * casillasPorFila << " ";
    cout << "|" << endl;
}
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila) {
    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|" << setw(4) << setfill(' ') << casillaAstring(tablero[i - 1 + fila * casillasPorFila]);
    cout << "|" << endl;
}
void pintaJugadores(const tEstadoJugadores estadosJ, int fila, int casillasPorFila) {
    int casilla;
    int blancos = 4 - NUM_JUGADORES;
    string b = "";
    for (int i = 0; i < blancos; i++) b = b + " ";
    cout << "|";
    for (int i = 1; i <= casillasPorFila; i++) {
        casilla = i - 1 + fila * casillasPorFila;
        for (int jug = 0; jug < NUM_JUGADORES; jug++) {
            if (estadosJ[jug].casilla == casilla)
                cout << jug + 1;
            else
                cout << " ";
        }
        cout << b;
        cout << "|";
    }
    cout << endl;

}
string casillaAstring(tCasilla casilla) {
    string cadena;
    switch (casilla) {
        case NORMAL:
            cadena = " ";
            break;
        case OCA:
            cadena = "OCA";
            break;
        case DADO1:
        case DADO2:
            cadena = "DADO";
            break;
        case PUENTE1:
        case PUENTE2:
            cadena = "PNTE";
            break;
        case POSADA:
            cadena = "PSDA";
            break;
        case CALAVERA:
            cadena = "MUER";
            break;
        case LABERINTO:
            cadena = "LBRN";
            break;
        case POZO:
            cadena = "POZO";
            break;
        case CARCEL:
            cadena = "CRCL";
            break;
    }
    return cadena;
}

//----------------------------------------------
// FIN DEL CóDIGO PROPORCIONADO CON LA PLANTILLA
//----------------------------------------------

int tirarDado() {
    return 1 + rand() % 6;
}
int tirarDadoManual() {
    int tirada;

    do {
        cout << "Introduce el numero del dado: ";
        cin >> tirada;
        if (tirada < 1 || tirada > 6) { cout << "Introduce un numero valido!!!"; }
    } while (tirada < 1 || tirada > 6);

    return tirada;
}
bool esMeta(int casilla) { return casilla == (NUM_CASILLAS - 1);}
int quienEmpieza() { return rand() % NUM_JUGADORES; }
void cambiaTurno(int &turno) {
    if (turno < NUM_JUGADORES - 1) {
        turno++;
    } else {
        turno = 0;
    }
}
void actualizarTurno(int& turno, tEstadoJugadores estadoJug) {
    if (turno == -1) {
        turno = quienEmpieza();
    } else if (estadoJug[turno].penalizaciones > 0) {
        cout << "Vuelves a tirar" << endl;
        estadoJug[turno].penalizaciones--;
    } else {
        cambiaTurno(turno);
        if (estadoJug[turno].penalizaciones < 0) {
            estadoJug[turno].penalizaciones++;
            cout << "El Jugador " << turno + 1 << " pierde la ronda y le quedan " << estadoJug[turno].penalizaciones << " turnos sin jugar";
            actualizarTurno(turno, estadoJug);
        }
    }
}

bool cargaTablero(tTablero tablero) {
    ifstream archivo;
    string nombreFichero;

    cout << "Introduce el nombre del archivo: ";
    cin >> nombreFichero;
    archivo.open(nombreFichero);
    if (archivo.is_open()) {
        string line;
        int valor = -1;
        tCasilla casilla;

        while (archivo >> valor && valor != 0) {
            if(valor != 0){
                archivo >> line;
                if(line == "OCA"){ casilla = OCA;}
                else if(line == "DADO1"){ casilla = DADO1;}
                else if(line == "DADO2"){ casilla = DADO2;}
                else if(line == "PUENTE1"){ casilla = PUENTE1;}
                else if(line == "PUENTE2"){ casilla = PUENTE2;}
                else if(line == "LABERINTO"){ casilla = LABERINTO;}
                else if(line == "CALAVERA"){ casilla = CALAVERA;}
                else if(line == "POSADA"){ casilla = POSADA;}
                else if(line == "CARCEL"){ casilla = CARCEL;}
                else if(line == "POZO"){ casilla = POZO;}
                else { casilla = NORMAL;}

                tablero[valor - 1] = casilla;
            }
        }
        archivo.close();
        return true;
    } else {
        cout << "ERROR DE APERTURA DE ARCHIVO " << nombreFichero << endl;
        return false;
    }
}

void iniciaTablero(tTablero tablero) {
    for (int i = 0; i < NUM_CASILLAS; i++) {
        if (i == NUM_CASILLAS - 1) {
            tablero[i] = OCA;
        } else {
            tablero[i] = NORMAL;
        }
    }
}

bool esCasillaSalto(const tTablero tablero, int casilla) {
    switch (tablero[casilla]) {
        case OCA:
        case PUENTE1:
        case PUENTE2:
        case DADO1:
        case DADO2:
        case LABERINTO:
        case CALAVERA:
            return true;
        default:
            return false;
    }
}
int saltaACasilla(const tTablero tablero, int casillaActual) {
    switch (tablero[casillaActual]) {
        case OCA:
            cout << "De OCA en OCA y tiro por que me toca" << endl;
            buscaCasillaAvanzando(tablero, tablero[casillaActual], casillaActual);
            break;
        case PUENTE1:
            cout << "De PUENTE en PUENTE y tiro por que me lleva la corriente" << endl;
            buscaCasillaAvanzando(tablero, tablero[casillaActual], casillaActual);
            break;
        case PUENTE2:
            cout << "De PUENTE en PUENTE y tiro por que me lleva la corriente" << endl;
            buscaCasillaRetrocediendo(tablero, tablero[casillaActual], casillaActual);
            break;
        case DADO1:
            cout << "De DADO a DADO y tiro por que me ha tocado" << endl;
            buscaCasillaAvanzando(tablero, tablero[casillaActual], casillaActual);
            break;
        case DADO2:
            cout << "De DADO a DADO y tiro por que me ha tocado" << endl;
            buscaCasillaRetrocediendo(tablero, tablero[casillaActual], casillaActual);
            break;
        case LABERINTO:
            cout << "Has caido en el LABERINTO, retrocedes 12 casillas" << endl;
            casillaActual -= 12;
            break;
        case CALAVERA:
            cout << "CALAVERA!!! y a volver a empezar" << endl;
            casillaActual = 0;
            break;
        default:
            break;
    }
    return casillaActual;
}
void buscaCasillaAvanzando(const tTablero tablero, tCasilla tipo, int &posicion) {
    tCasilla buscada;
    bool encontrada = false;
    int i = posicion;

    if (tipo == PUENTE1) { buscada = PUENTE2; }
    else if (tipo == DADO1) { buscada = DADO2; }
    else { buscada = OCA; }

    while (!encontrada && i < NUM_CASILLAS) {
        if (tablero[i] == buscada && i != posicion) {
            posicion = i;
            encontrada = true;
        }
        i++;
    }
}
void buscaCasillaRetrocediendo(const tTablero tablero, tCasilla tipo, int &posicion) {
    tCasilla buscada;
    bool encontrada = false;
    int i = posicion;

    if (tipo == PUENTE2) { buscada = PUENTE1; }
    else if (tipo == DADO1) { buscada = DADO2; }
    else if (tipo == DADO2) { buscada = DADO1; }

    while (!encontrada && i < NUM_CASILLAS) {
        if (tablero[i] == buscada && i != posicion) {
            posicion = i;
            encontrada = true;
        }
        i--;
    }
}
void efectoTirada(const tTablero tablero, tEstadoJugador &jugador) {
    tCasilla casilla = tablero[jugador.casilla];
    switch (casilla) {
        case OCA:
        case PUENTE1:
        case PUENTE2:
        case DADO1:
        case DADO2:
            jugador.penalizaciones += 1;
            break;
        case POSADA:
            cout << "Has caido en la POSADA" << endl;
            cout << "Pierdes 1 turno" << endl;
            jugador.penalizaciones -= TURNOS_POSADA;
            break;
        case CARCEL:
            cout << "Has caido en la CARCEL" << endl;
            cout << "Pierdes 2 turno" << endl;
            jugador.penalizaciones -= TURNOS_CARCEL;
            break;
        case POZO:
            cout << "Has caido en la POZO" << endl;
            cout << "Pierdes 3 turno" << endl;
            jugador.penalizaciones -= TURNOS_POZO;
            break;
        default:
            break;
    }
}

void iniciaJugadores(tEstadoJugadores jugadores) {
    for (int i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i].casilla = 0;
        jugadores[i].penalizaciones = 0;
    }
}
void tirada(const tTablero tablero, tEstadoJugador &estadoJug) { // Como depende del MODO_DEBUG y esta predefinido, siempre hay una parte del if/else que no se usa
    if (!MODO_DEBUG) {
        estadoJug.casilla += tirarDado();
    } else {
        estadoJug.casilla += tirarDadoManual();
    }
    efectoTirada(tablero, estadoJug);
}
int partida(tEstadoPartida &estado) {
    char commando;
    if(estado.turno == -1) {
        iniciaJugadores(estado.estadoJug);
        iniciaTablero(estado.tablero);
        bool exito = cargaTablero(estado.tablero);
        if (!exito) { return 0;}
        cout << "Se ha cargado correctamente el tablero" << endl;
    }
    do{
        do{
            cout << "Introduce que deseas hacer( T = Tirar, S = Salir de la partida): ";
            cin >> commando;
            if(commando != 'T' && commando != 'S'){ cout << "Introduce una letra válida";}
        } while(commando != 'T' && commando != 'S');

        switch (commando) {
            case 'T':
                juego(estado);
                break;
            case 'S':
                //salir de la partida
                return -1;
            default:
                break;
        }
    } while (!esMeta(estado.estadoJug[estado.turno].casilla));
    if (esMeta(estado.estadoJug[estado.turno].casilla)) {
        cout << "Enhorabuena!!!" << endl << "**** Gana el Jugador " << estado.turno + 1 << " ****" << endl;
    }

    return 0;
}
void juego(tEstadoPartida &estado){
    actualizarTurno(estado.turno, estado.estadoJug);
    pintaTablero(estado);
    cout << "**** Turno del Jugador: " << estado.turno + 1 << " ****" << endl;
    cout << "Estas en la posicion: " << estado.estadoJug[estado.turno].casilla + 1 << endl; // Se suma 1 porque nuestro array va de 0-62 pero las casillas del tablero del 1-63
    tirada(estado.tablero, estado.estadoJug[estado.turno]);
    if (esCasillaSalto(estado.tablero, estado.estadoJug[estado.turno].casilla)) {
        estado.estadoJug[estado.turno].casilla = saltaACasilla(estado.tablero,estado.estadoJug[estado.turno].casilla);
        cout << "Saltas a la casilla " << estado.estadoJug[estado.turno].casilla + 1 << endl;
    }
}

bool cargaPartidas(tListaPartidas &partidas){
    ifstream archivo;
    string nombreArchivo;
    int numPartidasCargar, turno;
    int i = 0;

    cout << "Dame el nombre del archivo que contiene las partidas: ";
    cin >> nombreArchivo;
    archivo.open(nombreArchivo);
    if(archivo.is_open()){
        archivo >> numPartidasCargar;
        if(numPartidasCargar != 0){
            while(i < numPartidasCargar){
                iniciaTablero(partidas.listaPartidas[i].tablero);
                cargarTablero(partidas.listaPartidas[i].tablero, archivo);
                archivo >> turno;
                partidas.listaPartidas[i].turno = turno - 1;
                cargaJugadores(partidas.listaPartidas[i].estadoJug, archivo);
                i++;
            }
        }
        partidas.cont = numPartidasCargar;
        archivo.close();
        cout << "Fichero cargado correctamente" << endl;
        return true;
    } else {
        cout << "No se ha podido abrir el archivo" << endl;
        return false;
    }

}
void cargarTablero(tTablero tablero, ifstream &archivo){
    string line;
    int valor = -1;
    tCasilla casilla;

    while (archivo >> valor && valor != 0) {
        if(valor != 0){
            archivo >> line;
            if(line == "OCA"){ casilla = OCA;}
            else if(line == "DADO1"){ casilla = DADO1;}
            else if(line == "DADO2"){ casilla = DADO2;}
            else if(line == "PUENTE1"){ casilla = PUENTE1;}
            else if(line == "PUENTE2"){ casilla = PUENTE2;}
            else if(line == "LABERINTO"){ casilla = LABERINTO;}
            else if(line == "CALAVERA"){ casilla = CALAVERA;}
            else if(line == "POSADA"){ casilla = POSADA;}
            else if(line == "CARCEL"){ casilla = CARCEL;}
            else if(line == "POZO"){ casilla = POZO;}
            else { casilla = NORMAL;}

            tablero[valor - 1] = casilla;
        }
    }
}
void cargaJugadores(tEstadoJugadores &jugadores, ifstream &archivo){
    int casilla;
    for(int i = 0; i < NUM_JUGADORES; i++){
        archivo >> casilla;
        jugadores[i].casilla = casilla - 1;
        archivo >> jugadores[i].penalizaciones;
    }

}
void eliminarPartida(tListaPartidas &partidas, int indice){
    tEstadoPartida partidaNull;
    for(int i = indice; i < partidas.cont; i++){
        if(i == partidas.cont - 1){
            partidas.listaPartidas[i] = partidaNull;
        } else {
            partidas.listaPartidas[i] = partidas.listaPartidas[i+1];
        }
    }
    partidas.cont--;
}
bool insertaNuevaPartida(tListaPartidas &partidas, const tEstadoPartida &partidaOca){
    if(partidas.cont < MAX_PARTIDAS){
        partidas.listaPartidas[partidas.cont] = partidaOca;
        partidas.cont++;
        return true;
    } else {
        return false;
    }
}
void guardaPartidas(const tListaPartidas &partidas){
    string nombreArchivo;
    ofstream archivo;

    cout << "Introdece el nombre del archivo donde quieres guardar las partidas: ";
    cin >> nombreArchivo;
    archivo.open(nombreArchivo.c_str(), fstream::out);
    if(archivo.is_open()){
        archivo << partidas.cont << endl;
        for(int i = 0; i < partidas.cont; i++){
            guardaTablero(partidas.listaPartidas[i].tablero, archivo);
            archivo << partidas.listaPartidas[i].turno + 1 << endl;
            guardaJugadores(partidas.listaPartidas[i].estadoJug, archivo);
        }
    } else {
        cout << "No se ha podido abrir el archivo" << endl;
    }

    archivo.close();
    cout << "Se ha escrito correctamente";

}
void guardaTablero(const tTablero tablero, ofstream &archivo){
    for(int i = 0; i < NUM_CASILLAS; i++){
        switch(tablero[i]){
            case OCA:
                archivo << i + 1 << " ";
                archivo << "OCA" << endl;
                break;
            case DADO1:
                archivo << i + 1 << " ";
                archivo << "DADO1" << endl;
                break;
            case DADO2:
                archivo << i + 1 << " ";
                archivo << "DADO2" << endl;
                break;
            case PUENTE1:
                archivo << i + 1 << " ";
                archivo << "PUENTE1" << endl;
                break;
            case PUENTE2:
                archivo << i + 1 << " ";
                archivo << "PUENTE2" << endl;
                break;
            case LABERINTO:
                archivo << i + 1 << " ";
                archivo << "LABERINTO" << endl;
                break;
            case CALAVERA:
                archivo << i + 1 << " ";
                archivo << "CALAVERA" << endl;
                break;
            case POSADA:
                archivo << i + 1 << " ";
                archivo << "POSADA" << endl;
                break;
            case CARCEL:
                archivo << i + 1 << " ";
                archivo << "CARCEL" << endl;
                break;
            case POZO:
                archivo << i + 1 << " ";
                archivo << "POZO" << endl;
                break;
            default:
                break;
        }
    }
    archivo << 0 << endl;
}
void guardaJugadores(const tEstadoJugadores jugadores, ofstream &archivo){
    int casilla;
    for(int i = 0; i < NUM_JUGADORES; i++){
        archivo << jugadores[i].casilla + 1 << " ";
        archivo << jugadores[i].penalizaciones << endl;
    }
}

void menuNuevaPartida(tListaPartidas &partidas){
    tEstadoPartida partidaAdd;
    int eliminar; // si vale 0, se elimina porque se habrá acabado la partida, si vale -1, significara que se ha dejado a medias y se guardará en la lista de partidas abandonadas
    bool insertado = insertaNuevaPartida(partidas, partidaAdd);
    if(insertado){
        cout << "Se ha insertado correctamente la partida" << endl;
        eliminar = partida(partidas.listaPartidas[partidas.cont - 1]);
        if(eliminar == 0){
            eliminarPartida(partidas, partidas.cont - 1);
        }
    } else {
        cout << "No se ha podido insertar la partida(Hay 10 partidas en juego)" << endl;
    }
}
void menuCargarPartida(tListaPartidas &partidas){
    int partidaCargar;

    if(partidas.cont == 0){
        cout << "No hay partidas en juego" << endl;
    } else {
        cout << "Introduce el número de partida que quieres cargar(1-" << partidas.cont << "): ";
        cin >> partidaCargar;
        if(partidaCargar > 0 && partidaCargar <= partidas.cont){
            int eliminar = partida(partidas.listaPartidas[partidaCargar - 1]);
            if(eliminar == 0){
                eliminarPartida(partidas, partidaCargar - 1);
            }
        } else {
            cout << "No existe esa partida" << endl;
        }
    }
}