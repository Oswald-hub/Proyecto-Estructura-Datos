#ifndef MENU_HXX
#define MENU_HXX

#include <string>
#include <vector>

using namespace std;

void break_line();


class Secuencia {
    public:
        std::string nombre;   // nombre de la secuencia (>Identificador)
        std::string adn;      // secuencia como string

        Secuencia(const std::string &nom, const std::string &contenido) {
            nombre = nom;
            adn = contenido; // copiamos directamente
        }

        void imprimir() const {
            break_line();
            std::cout << ">" << nombre << "\n";
            std::cout << adn;
            break_line();
        }

        // Método extra: buscar subsecuencia en el ADN
        void buscarSubsecuencia(const std::string &sub) const {
            size_t pos = adn.find(sub);
            if (pos != std::string::npos) {
                std::cout << "Subsecuencia \"" << sub << "\" encontrada en posición " << pos << " en " << nombre << "\n";
            } else {
                std::cout << "Subsecuencia \"" << sub << "\" NO encontrada en " << nombre << "\n";
            }
        }
};

// Función principal del menú - controla el flujo del programa
void mostrarMenu();

vector<string> parsearComando(const string& linea);

// Retorna un número que corresponde al comando (1-10) o -1 si no existe
int identificarComando(const string& comando);

bool aperturaArchivo( ifstream &archivo, const string nombreArchivo);

vector<Secuencia> leerFasta(ifstream &archivo);

void contarBases(Secuencia &secuencia);

int contarSubsecuencias(vector<Secuencia> lista, string subSecuencia);

void enmascararSubsecuencia(vector<Secuencia> lista, string subSecuencia);

bool guardarArchivo(vector<Secuencia>& secuencias, string& nombre);

void histograma(const vector<Secuencia>& listaSecuencias, const string& nombre);


#endif