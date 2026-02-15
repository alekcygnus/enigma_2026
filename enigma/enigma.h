#pragma once
#include "Rotor.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

// Carga un rotor desde un archivo de texto
Rotor loadRotorFromFile(const std::string& filename);

// logica de cifrado (ida)
std::string encodeMessage(const std::string& missatge, Rotor& r1, Rotor& r2, Rotor& r3);

// logica de descifrado (vuelta) 
std::string decodeMessage(const std::string& missatge, Rotor& r1, Rotor& r2, Rotor& r3);

// validacion de letras unicass 
bool esPermutacionValida(const std::string& wiring);

char reflejar(char c); 
std::string prepareMessageToSaveOnFile(const std::string& missatge);
void mostrarMensaje(const std::string& nombreArchivo, const std::string& mensaje);