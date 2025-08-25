#ifndef MENU_HXX
#define MENU_HXX

#include <string>
#include <vector>

using namespace std;

// Función principal del menú - controla el flujo del programa
void mostrarMenu();

// Función para identificar el comando ingresado por el usuario
// Retorna un número que corresponde al comando (1-10) o -1 si no existe
int identificarComando(const string& comando);

// Función para parsear el comando completo y extraer todos los parámetros
// Retorna un vector donde el primer elemento es el comando y los siguientes son los parámetros
vector<string> parsearComando(const string& linea);

#endif