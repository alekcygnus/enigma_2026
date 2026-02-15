#include "enigma.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <set> //VALIO LETRAS UNICAS 

// Reflector
std::string reflector = "YRUHQSLDPXNGOKMIEBFZCWVJAT";

// --- FUNCION DE VALIDACION ---
bool esPermutacionValida(const std::string& wiring) {
    if (wiring.size() != 26) return false;
    
    std::set<char> letras;
    for (char c : wiring) {
        // Solo permitimos A-Z mayúsculas
        if (c < 'A' || c > 'Z') return false;
        letras.insert(c);
    }
    // Si el set tiene 26 elementos, es que no hay repetidos
    return letras.size() == 26;
}

Rotor loadRotorFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string wiring;
    char notch = 'Z'; // Valor por defecto

    if (!file.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir " << filename << std::endl;
        return Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, notch);
    }

    std::getline(file, wiring);
    
    // VALIDACION: Comprobar que el cableado es correcto y único
    if (!esPermutacionValida(wiring)) {
        std::cerr << "[ERROR] " << filename << ": permutacion incorrecta (deben ser 26 letras unicas A-Z)" << std::endl;

        //AQUI A PROPOSITO HAGO QUE RETORNE A UN ROTOR NEUTRO PARA QUE EL PROGRAMA NO CRASHEE, PERO EL ERROR SE AVISA  
        return Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, notch);
    }

    std::string notchLine;
    if (std::getline(file, notchLine)) {
        if (!notchLine.empty() && notchLine[0] >= 'A' && notchLine[0] <= 'Z') {
            notch = notchLine[0];
        }
    }

    return Rotor(wiring, 0, notch);
}

char reflejar(char c) {
    return reflector[charToIndex(c)];
}

bool rotorNotchTriggered(char notch, int position) {
    return position == charToIndex(notch);
}

// // LOGICA DEL CIFRADO (Camino adelante: 1 -> 2 -> 3)
std::string encodeMessage(const std::string& missatge, Rotor& r1, Rotor& r2, Rotor& r3) {
    std::string resultado;

    for (char c : missatge) {
        if (c < 'A' || c > 'Z') continue;

        // 1. AVANZAR ROTORES (Efecto odómetro)
        r1.rotate();
        if (rotorNotchTriggered(r1.notch, r1.position)) {
            r2.rotate();
            if (rotorNotchTriggered(r2.notch, r2.position)) {
                r3.rotate();
            }
        }

        // 2. CAMINO HACIA ADELANTE (R1 -> R2 -> R3)
        char letra = r1.encode(c);
        letra = r2.encode(letra);
        letra = r3.encode(letra);

        resultado += letra;
    }
    return resultado;
}

// --- LOGICA DE DESCIFRADO (Camino inverso: 3 -> 2 -> 1)
// 
// Nota:A AQUI LA ROTACION ES IGUAL AL CIFRAR Y LAS POSICIONES COINCIDEN 
std::string decodeMessage(const std::string& missatge, Rotor& r1, Rotor& r2, Rotor& r3) {
    std::string resultado;

    for (char c : missatge) {
        if (c < 'A' || c > 'Z') continue;

        // 1. AVANZO ROTORES IGUAL QUE AL CIFRAR 
        r1.rotate();
        if (rotorNotchTriggered(r1.notch, r1.position)) {
            r2.rotate();
            if (rotorNotchTriggered(r2.notch, r2.position)) {
                r3.rotate();
            }
        }

        // 2. PARA DAR MARCHA ATRAS USO LA FUNCION DE DECODE DEL ROTOR  (R3 -> R2 -> R1)
        char letra = r3.decode(c);
        letra = r2.decode(letra);
        letra = r1.decode(letra);

        resultado += letra;
    }
    return resultado;
}

std::string prepareMessageToSaveOnFile(const std::string& missatge) {
    std::string finalWord;
    int cnt = 0;

    for (char c : missatge) {
        finalWord += c;
        cnt++;
        if (cnt % 5 == 0) {
            finalWord += ' ';
        }
    }
    return finalWord;
}

void mostrarMensaje(const std::string& nombreArchivo, const std::string& mensaje) {
    int totalLletres = mensaje.length();
    int grups = (totalLletres + 4) / 5; // CLCULO REDONDEANDO HACIA ARRIBA
    std::cout << "[OK] Missatge processat a \"" << nombreArchivo << "\" (" << totalLletres << " lletres, " << grups << " grups de 5)\n";
}