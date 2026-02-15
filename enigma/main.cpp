#include "enigma.h"
#include <iostream>
#include <cctype>
#include <string>
#include <fstream>

// Convierte letra 'A'-'Z' a índice 0-25, si inválida devuelve -1
int letraAIndice(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    return -1;
}

// Limpia y prepara el mensaje: mayúsculas, solo A-Z
std::string prepararMensaje(const std::string& msg) {
    std::string resultado;
    for (char c : msg) {
        if (std::isalpha(c)) {
            resultado += std::toupper(c);
        }
    }
    return resultado;
}

// Pide posiciones iniciales como letras (ejemplo: A C B)
void pedirPosicionesIniciales(int& pos1, int& pos2, int& pos3) {
    std::string entrada;
    bool valido = false;

    while (!valido) {
        std::cout << "Introduce las 3 letras de posicion inicial (Ejemplo: A B C): ";
        std::getline(std::cin, entrada);

        int count = 0;
        char letras[3];
        for (char c : entrada) {
            if (std::isalpha(c)) {
                letras[count++] = std::toupper(c);
                if (count == 3) break;
            }
        }
        if (count != 3) {
            std::cout << "[ERROR] Debes ingresar 3 letras.\n";
            continue;
        }

        pos1 = letraAIndice(letras[0]);
        pos2 = letraAIndice(letras[1]);
        pos3 = letraAIndice(letras[2]);
        valido = true;
    }
}

void mostrarMenu() {
    std::cout << "\n==== MAQUINA ENIGMA ====\n";
    std::cout << "1. cifrar mensaje\n";
    std::cout << "2. Descifrar mensaje\n";
    std::cout << "3. Editar rotores\n";
    std::cout << "4. Salir\n";
    std::cout << "Selecciona una opcion: ";
}

int main() {
    // Carga inicial de archivos
    Rotor r1 = loadRotorFromFile("rotor1.txt");
    Rotor r2 = loadRotorFromFile("rotor2.txt");
    Rotor r3 = loadRotorFromFile("rotor3.txt");

    int opcion = 0;
    int pos1 = 0, pos2 = 0, pos3 = 0;

    do {
        mostrarMenu();
        std::string entrada;
        std::getline(std::cin, entrada);
        try {
            opcion = std::stoi(entrada);
        }
        catch (...) {
            opcion = 0;
        }

        if (opcion == 1 || opcion == 2) { 
            pedirPosicionesIniciales(pos1, pos2, pos3);

            r1.position = pos1;
            r2.position = pos2;
            r3.position = pos3;

            std::cout << "Escribe el mensaje: ";
            std::string mensaje;
            std::getline(std::cin, mensaje);

            std::string limpio = prepararMensaje(mensaje);

            // Copias temporales para el proceso (la rotación modifica el objeto)
            Rotor temp1 = r1;
            Rotor temp2 = r2;
            Rotor temp3 = r3;

            std::string resultado;
            std::string nombreArchivo;

            if (opcion == 1) {
                // cifrar: Camino adelante (1->2->3)
                resultado = encodeMessage(limpio, temp1, temp2, temp3);
                nombreArchivo = "xifrat.txt";
            } else {
                // descifrar: Camino inverso (3->2->1 usando decode)
                resultado = decodeMessage(limpio, temp1, temp2, temp3);
                nombreArchivo = "desxifrat.txt";
            }

            std::cout << "Resultado: " << resultado << "\n";

            std::ofstream archivo(nombreArchivo);
            archivo << prepareMessageToSaveOnFile(resultado) << std::endl;
            archivo.close();

            mostrarMensaje(nombreArchivo, resultado);

        }
        else if (opcion == 3) {
            std::string num, nouWiring;
            std::cout << "Que rotor quieres editar (1, 2, 3)? ";
            std::getline(std::cin, num);
            std::cout << "introduce la nueva permutacion (26 letras unicas de A-Z): ";
            std::getline(std::cin, nouWiring);
            
            // Uso la validacion de letras unicas antes de guardar
            if (esPermutacionValida(nouWiring)) {
                std::string filename = "rotor" + num + ".txt";
                std::ofstream fitxer(filename);
                fitxer << nouWiring << "\nZ" << std::endl; 
                fitxer.close();

                // actualizo el rotor en la memoria del programa
                if (num == "1") r1 = loadRotorFromFile("rotor1.txt");
                else if (num == "2") r2 = loadRotorFromFile("rotor2.txt");
                else if (num == "3") r3 = loadRotorFromFile("rotor3.txt");

                std::cout << "[OK] Rotor " << num << " actualizado y cargado a la memoria\n";
            } else {
                std::cout << "[ERROR] Permutacion no valida (recuerda son 26 letras unicas A-Z).\n";
            }
        }
        else if (opcion == 4) {
            std::cout << "Saliendo...\n";
        }
        else {
            std::cout << "[ERROR] Opcion invalida.\n";
        }

    } while (opcion != 4);

    return 0;
}