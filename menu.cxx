// menu.cxx
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "menu.hxx"
// cd "C:\Users\Elvia\Downloads\taller_1_complejidad_archivos\nuevo" && g++ -Wall -o programa *.cxx && programa

using namespace std;

void break_line()
{
    cout << endl
         << "--------------------------------------------------" << endl;
}

void mostrarMenu() // funcion para mostrar el menu
{
    cout << "\n=================================\n";
    cout << " SISTEMA DE SECUENCIA GENETICA \n";
    cout << "=================================\n";
    cout << "Escriba '$ayuda' para visualizar todos los comandos. \n";

    string linea; // variable que captura lo que escribe el usuario
    ifstream archivo;
    vector<Secuencia> secuenciasarchivo;
    while (true)
    {
        cout << "$";
        string comando;
        if (!getline(cin, linea))
        { // captura lo que escribe el usuario
            cout << "Error al leer el comando.\n";
            continue;
        }

        if (linea.empty())
        { // por si no escribe nada
            continue;
        }

        vector<string> parametros = parsearComando(linea); // vector que contiene el comando y los parametros
        if (parametros.empty())
        { // por si el vector quedo vacio
            cout << "No se ingreso ningun comando valido.\n";
            continue;
        }

        if (parametros.size() > 1)
        {
            comando = parametros[0]; // la primera posicion  siempre sera el comando
            string arg = parametros[1];
            cout << endl
                 << "Comando: " << comando << endl
                 << "Argumento: " << arg << endl;
        }
        else
        {
            comando = parametros[0]; // la primera posicion  siempre sera el comando
            cout << endl
                 << "Comando: " << comando << endl;
        }

        // Verificar si es comando de salida
        if (comando == "salir")
        {
            if (parametros.size() == 1)
            {
                break;
            }
            else
            {
                cout << "Error: El comando 'salir' solo acepta un parametro, intente de nuevo." << endl;
                cout << "Uso: 'salir'\n";
            }
        }
        int num = identificarComando(comando); // la funcion identifica dependiendo lo que escriba el usuario cual comando es
        switch (num)
        {
            case 1:
            {
                if (parametros.size() == 2)
                { // si el vector parametros tiene 2 posiciones quiere decir que tiene el comando y el parametro
                    bool apertura = aperturaArchivo(archivo, parametros[1]);
                    if (apertura)
                    {
                        secuenciasarchivo = leerFasta(archivo);
                        archivo.close();
                        if (secuenciasarchivo.empty())
                        {
                            break_line();
                            cout << endl
                                << "nombre_archivo no contiene ninguna secuencia." << endl;
                            break_line();
                        }
                        else
                        {
                            break_line();
                            cout << secuenciasarchivo.size() << " secuencias cargadas correctamente desde " << parametros[1];
                            break_line();
                        }
                    }
                    // cout << "nombre_archivo no se encuentra o no puede leerse."<<endl; // e la segunda posicion esta el parametro
                }
                else if (parametros.size() < 2)
                { // si tine 1 o menos parametros quiere decir que le falta el parametro
                    cout << "Error: El comando 'cargar' solo permite un parametro de entrada, nombre de archovo.\n";
                    cout << "Uso: cargar nombre_archivo.txt\n";
                }
                else
                {
                    cout << "Error: Demasiados parametros para el comando 'cargar'.\n"; // si hay mas de 2 paametros
                    cout << "Uso: cargar nombre_archivo.txt\n";
                }
                break;
            }
            case 2:
            {
                if (parametros.size() == 1)
                { // solo es un comando , no hay parametros
                    if (!secuenciasarchivo.empty())
                    {
                        for (Secuencia &sec : secuenciasarchivo)
                            contarBases(sec);
                        
                    }
                    else
                    {
                        cout << "(no hay secuencias cargadas) No hay secuencias cargadas en memorias" << endl;
                    }
                }
                else
                {
                    cout << "Error: El comando 'listar_secuencias' no acepta parametros.\n";
                    cout << "Uso: listar_secuencias\n";
                }
                break;
            }
            case 3:
            { // histograma
                if (parametros.size() == 2)
                { // contiene comando y parametro
                    if (!secuenciasarchivo.empty())
                    {
                        histograma(secuenciasarchivo, parametros[1]);
                    }
                    else
                    {
                        cout << "(no hay secuencias cargadas) No hay secuencias cargadas en memoria.\n";
                    }
                }
                else if (parametros.size() < 2)
                { // falta el parametro para histograma
                    cout << "Error: El comando 'histograma' requiere una descripcion de secuencia.\n";
                    cout << "Uso: histograma descripcion_secuencia\n";
                }
                else if (parametros.size() >= 3)
                {
                    cout << "Error: Demasiados parametros para el comando 'histograma'.\n";
                    cout << "Uso: histograma descripcion_secuencia\n";
                }
                break;
            }
            case 4:
            {   // es_subsecuencia
                // Si hay exactamente 2 parametros y las secuencias estan cargadas
                if (parametros.size() == 2 && !secuenciasarchivo.empty())
                {
                    string subsecuencia = parametros[1];
                    // Contamos cuantas veces aparece la subsecuencia
                    int total = contarSubsecuencias(secuenciasarchivo, subsecuencia);
                    // Si no existe
                    if (total == 0)
                    {
                        cout << "(la subsecuencia no existe) La subsecuencia \""
                            << subsecuencia << "\" no se encontro en las secuencias cargadas.\n";
                    }
                    else
                    {
                        // Si existe, mostramos el total
                        cout << "El total de subsecuencias \"" << subsecuencia
                            << "\" encontradas en las secuencias es: " << total << endl;
                    }
                }
                else if (secuenciasarchivo.empty())
                {
                    cout << "(no hay secuencias cargadas) No hay secuencias cargadas en memoria.\n";
                }
                else if (parametros.size() < 2)
                {
                    cout << "Error: El comando 'es_subsecuencia' requiere una subsecuencia.\n";
                    cout << "Uso: es_subsecuencia ACGT\n";
                }
                else
                {
                    cout << "Error: Demasiados parametros para el comando 'es_subsecuencia'.\n";
                    cout << "Uso: es_subsecuencia ACGT\n";
                }
                break;
            }
            case 5:
            { // enmascarar
                // Verificamos que tenga exactamente 2 parametros y que existan secuencias cargadas
                if (parametros.size() == 2 && !secuenciasarchivo.empty())
                {
                    string subsecuencia = parametros[1];
                    // Contamos cuantas veces aparece la subsecuencia
                    int total = contarSubsecuencias(secuenciasarchivo, subsecuencia);
                    if (total == 0)
                    {
                        cout << "(la subsecuencia no existe) La subsecuencia dada no existe dentro de las secuencias cargadas en memoria.\n";
                    }
                    else
                    {
                        // Llamamos a la funcion que enmascara la subsecuencia
                        enmascararSubsecuencia(secuenciasarchivo, subsecuencia);
                        cout << "La subsecuencia '" << subsecuencia
                            << "' fue encontrada " << total << " veces y ha sido enmascarada.\n";
                    }
                }
                else if (secuenciasarchivo.empty())
                {
                    // No hay secuencias cargadas
                    cout << "(no hay secuencias cargadas) No hay secuencias cargadas en memoria.\n";
                }
                else if (parametros.size() < 2)
                {
                    // Faltan parametros
                    cout << "Error: El comando 'enmascarar' requiere una subsecuencia.\n";
                    cout << "Uso: enmascarar ACGT\n";
                }
                else
                {
                    // Demasiados parametros
                    cout << "Error: Demasiados parametros para el comando 'enmascarar'.\n";
                    cout << "Uso: enmascarar ACGT\n";
                }

                break;
            }
            case 6:
            { // guardar
                if (parametros.size() == 2)
                {
                    if(!secuenciasarchivo.empty()){
                        bool guardar = guardarArchivo(secuenciasarchivo, parametros[1]);
                        if (guardar){
                            cout << "Secuencia guardada correctamente en:" << parametros[1] << endl;
                        }else{
                            cout << "(problemas en archivo) Error guardando en:" << parametros[1] << endl;
                        }
                    }
                }    
                else if (parametros.size() < 2){ // no posee un parametro correcto
                    cout << "Error: El comando 'guardar' requiere un nombre de archivo.\n";
                    cout << "Uso: guardar nombre_archivo.txt\n";
                }else{
                    cout << "Error: Demasiados parametros para el comando 'guardar'.\n";
                    cout << "Uso: guardar nombre_archivo.txt\n";
                }
                break;
            }
            case 7:{ //codificar
                if (parametros.size() == 2) {// recibe el respectivo parametro y comando
                    cout << "(mensaje de error) No se pueden guardar las secuencias cargadas en " << parametros[1] <<".fabin "<< endl;
                } else if (parametros.size() < 2) {// no obtiene los parametros necesarios
                    cout << "Error: El comando 'codificar' requiere un nombre de archivo.\n";
                    cout << "Uso: codificar archivo.fabin\n";
                } else {
                    cout << "Error: Demasiados parametros para el comando 'codificar'.\n";
                    cout << "Uso: codificar archivo.fabin\n";
                }
                break;
            }
            case 8:{//decodificar
                if (parametros.size() == 2) { // recibe parametros y comandos
                    cout << "(mensaje de error) No se pueden cargar las secuencias desde " << parametros[1] << ".fabin"<<endl;
                } else if (parametros.size() < 2) { // no recibe el parametro necesario
                    cout << "Error: El comando 'decodificar' requiere un nombre de archivo.\n";
                    cout << "Uso: decodificar archivo.fabin\n";
                } else {
                    cout << "Error: Demasiados parametros para el comando 'decodificar'.\n";
                    cout << "Uso: decodificar archivo.fabin\n";
                }
                break;
            }
            case 9:{//ruta mas corta
                    if (parametros.size() == 6) { // recibe los parametros y comandos
                        try {
                            string descripcion = parametros[1]; // se le asigna una posicion del array en [i] al atributo descripcion.
                            //se maneja la funcion stoi para convertir strings en enteros y asignarlos a cada parametro del array
                            int i = stoi(parametros[2]);
                            int j = stoi(parametros[3]);
                            int x = stoi(parametros[4]);
                            int y = stoi(parametros[5]);

                        cout << "(la secuencia no existe) La secuencia descripcion_secuencia no existe." << parametros[1]
                            << " " << i << " " << j << " " << x << " " << y << endl;
                    }
                    catch (const invalid_argument &e)
                    { // si se le pasan argumentos invalidos como un string en vez de un entero
                        cout << "Error: Los parametros i, j, x, y deben ser numeros enteros.\n";
                        cout << "Uso: ruta_mas_corta descripcion_secuencia i j x y\n";
                    }
                }
                else if (parametros.size() < 6)
                { // si los parametros son insuficientes muestra error
                    cout << "Error: El comando 'ruta_mas_corta' requiere 5 parametros.\n";
                    cout << "Uso: ruta_mas_corta descripcion_secuencia i j x y\n";
                }
                else
                { // exceso de parametros
                    cout << "Error: Demasiados parametros para el comando 'ruta_mas_corta'.\n";
                    cout << "Uso: ruta_mas_corta descripcion_secuencia i j x y\n";
                }
                break;
            }
            case 10:
            { // base_remota
                if (parametros.size() == 4)
                { // recibe el comando y los parametros
                    try
                    {
                        string descripcion = parametros[1]; // se le asigna una posicion del array a la variable 'descripcion'
                        int i = stoi(parametros[2]);        // convierte la variable de string a entero
                        int j = stoi(parametros[3]);        // convierte la variable de string a entero

                        cout << "(la secuencia no existe) La secuencia " << descripcion
                            << " " << i << " " << j << "no existe." << endl;
                    }
                    catch (const invalid_argument &e)
                    { // si i y j no son enteros muestra un error
                        cout << "Error: Los parametros i, j deben ser numeros enteros.\n";
                        cout << "Uso: base_remota descripcion_secuencia i j\n";
                    }
                }
                else if (parametros.size() < 4)
                { // no recibe los parametros suficientes y muestra error
                    cout << "Error: El comando 'base_remota' requiere 3 parametros.\n";
                    cout << "Uso: base_remota descripcion_secuencia i j\n";
                }
                else
                { // recibe exceso de parametros y muestra error
                    cout << "Error: Demasiados parametros para el comando 'base_remota'.\n";
                    cout << "Uso: base_remota descripcion_secuencia i j\n";
                }
                break;
            }
            case 11:
            { // Comando ayuda
                vector<string> ncomandos = {
                     "cargar", "listar_secuencias", "histograma", "es_subsecuencia", "enmascarar", 
                    "guardar", "codificar", "decodificar","ruta_mas_corta", "base_remota", "ayuda", "salir"
                };
                vector<string> descrip{
                    "Cargar archivo de secuencias", "Imprime secuencias de memoria", 
                    "Imprime el histograma de una secuencia (si existe)", "Determina si una subsecuencia existe dentro de otras", 
                    "Enmascara una subsecuencia dada por el usuario", "Guarda en un archivo las secuencias", 
                    "Codifica y guarda las secuencias en formato binario (Huffman)", 
                    "Decodifica y guarda secuencias desde un archivo binario", 
                    "Calcula la ruta mas corta entre dos posiciones en la secuencia", 
                    "Encuentra la base mas remota de una posicion en la secuencia", "Mostrar ayuda", "Salir"
                };
                vector<string> ejemplos{
                    "(cargar genetica.txt)", "(listar_secuencias)", "(histograma secuencia1)", 
                    "(es_subsecuencia ACGT)", "(enmascarar ACGT)", "(guardar genetica.txt)", 
                    "(codificar salida.txt)", "(decodificar datos.fabin)", "(ruta_mas_corta sec1 1 2 3 4)", 
                    "(base_remota sec1 2 3)", "(ayuda)", "(salir)"
                };
    
                if(parametros.size() == 1){ // Solo el comando ayuda
                    cout << "COMANDOS DISPONIBLES:" << endl;
                    for(size_t i = 0; i < ncomandos.size(); i++){
                        cout << ncomandos[i] << " = " << descrip[i] << ", ejemplo = " << ejemplos[i] << "\n";
                    }
                } else if (parametros.size() == 2){ // Busca comando específico
                    string buscado = parametros[1];
                    bool encontrado = false;
                    for (size_t i = 0; i < ncomandos.size(); i++){
                        if(ncomandos[i] == buscado){
                            cout << "\n" << ncomandos[i] << " = " << descrip[i] << "\nUso = " << ejemplos[i] << "\n";
                            encontrado = true;
                            break;
                        }
                    }
                    if(!encontrado){
                        cout << "Comando no encontrado: " << buscado << endl;
                    }
                }else {
                    cout << "Error, el comando ayuda solo permite 1 o 2 parametros.\n";
                    cout << "Uso: 'ayuda' o 'ayuda (comando)'\n";
                }
                break;
            }
            default:
                cout << "Comando no reconocido: '" << comando << "' Escriba el comando ayuda para visualizar los comandos. \n";
                break;
            }
    }
}

vector<string> parsearComando(const string &linea)
{                              // separa el comando por tokens por medio de vectores
    vector<string> parametros; // vector de tipo string como parametros
    istringstream iss(linea);  // leer linea por linea
    string palabra;            // variable para almacenar palabras temporalmente

    while (iss >> palabra)
    { // mientras hayan palabras
        if (!palabra.empty())
        {
            parametros.push_back(palabra); // si no hay palabras, a�adir una palabra al vector de parametros
        }
    }

    return parametros; // retorna el vector parametros    
}

int identificarComando(const string &comando)
{ // compara la primera palabra del string con la lista de comandos para reconocer si e svalido o no
    if (comando == "cargar")
        return 1; // compara la funcion cargar
    if (comando == "listar_secuencias")
        return 2; // compara la funcion listar secuencias
    if (comando == "histograma")
        return 3; // compara la funcion histograma
    if (comando == "es_subsecuencia")
        return 4; // compara la funcion es subsecuencia
    if (comando == "enmascarar")
        return 5; // compara la funcion enmascarar
    if (comando == "guardar")
        return 6; // compara la funcion guardar
    if (comando == "codificar")
        return 7; // compara la funcion codificar
    if (comando == "decodificar")
        return 8; // compara la funcion descodificar
    if (comando == "ruta_mas_corta")
        return 9; // compara la funcion ruta mas corta
    if (comando == "base_remota")
        return 10; // compara la funcion base remota
    if (comando == "ayuda")
        return 11; // compara la funcion ayuda.
    return -1;     // retorna -1 si no coincide el token con la lista de comandos
}

bool aperturaArchivo(ifstream &archivo, const string nombreArchivo)
{
    archivo.open(nombreArchivo);
    return archivo.is_open();
}

vector<Secuencia> leerFasta(ifstream &archivo)
{
    vector<Secuencia> lista;
    string linea, nombre, contenido;

    while (getline(archivo, linea))
    {
        // cout << linea << endl;
        if (linea.empty())
            continue;

        if (linea[0] == '>')
        {
            // Si ya había contenido, guardamos la secuencia anterior
            if (!nombre.empty())
            {
                lista.emplace_back(nombre, contenido);
                contenido.clear();
            }
            nombre = linea.substr(1); // quitar el '>'
        }
        else
        {
            contenido += linea; // concatenar secuencia
        }
    }
    if (!nombre.empty())
    {
        lista.emplace_back(nombre, contenido);
    }

    return lista;
}

bool guardarArchivo(vector<Secuencia>& secuencias, string& nombre)
{
    ofstream archivo(nombre);
    if(!archivo.is_open()){
    return false;
    }
    for(int i = 0; i < secuencias.size(); i++){
    archivo << secuencias[i].nombre << endl;
    archivo << secuencias[i].adn << endl;    
    }
    archivo.close();
    return true;
}
void contarBases(Secuencia &secuencia){
    bool contieneGuion = false;
    int cantidad_giones = 0;
    for (char c : secuencia.adn)
    {
        if (c == '-')
        {
            contieneGuion = true;
            cantidad_giones++;
        }
    }
    if (contieneGuion)
    {
        int numeroBases = secuencia.adn.length() - cantidad_giones;
        cout << "secuencia " << secuencia.nombre << " contiene al menos " << numeroBases << " bases." << endl;
    }
    else
    {
        cout << "secuencia " << secuencia.nombre << " contiene " << secuencia.adn.length() << " bases." << endl;
    }
}
// Funcion que cuenta cuantas veces aparece una subsecuencia en las secuencias cargadas
int contarSubsecuencias(vector<Secuencia> lista, string subSecuencia)
{
    int veces = 0;                                     // Contador de cuantas veces aparece la subsecuencia
    vector<Secuencia>::iterator itLst = lista.begin(); // Iterador para recorrer las secuencias

    // Recorremos todas las secuencias cargadas en la lista
    for (itLst; itLst != lista.end(); itLst++)
    {

        // Obtenemos el contenido de la secuencia actual
        string desc = itLst->getADN();
        string::iterator itdesc = desc.begin();

        // Recorremos cada posicion posible dentro de la secuencia
        // Solo llegamos hasta donde la subsecuencia pueda caber
        for (itdesc; itdesc + subSecuencia.size() <= desc.end(); itdesc++)
        {

            bool coincide = true; // Variable para verificar si hay coincidencia completa

            // Recorremos la subsecuencia caracter por caracter
            string::iterator itLss = subSecuencia.begin();
            for (itLss; itLss != subSecuencia.end(); itLss++)
            {

                // Comparamos el caracter correspondiente de la secuencia con el de la subsecuencia
                // Sumamos el desplazamiento (itLss - subSecuencia.begin()) para acceder al caracter correcto
                if (*(itdesc + (itLss - subSecuencia.begin())) != *itLss)
                {
                    coincide = false; // Si no coincide, cortamos la comprobacion
                    break;
                }
            }

            // Si encontramos coincidencia completa, incrementamos el contador
            if (coincide)
            {
                veces++;
            }
        }
    }
    return veces; // Retornamos el numero total de veces que aparece la subsecuencia
}

// Funcion que enmascara (reemplaza con 'X') las subsecuencias encontradas en todas las secuencias cargadas
void enmascararSubsecuencia(vector<Secuencia> &lista, string subSecuencia)
{
    vector<Secuencia>::iterator itLst = lista.begin(); // Iterador para recorrer las secuencias
    // Recorremos todas las secuencias cargadas
    for (itLst; itLst != lista.end(); itLst++)
    {
        // Obtenemos el contenido de la secuencia actual
        string desc = itLst->getADN();
        string::iterator itdesc = desc.begin();

        // Recorremos cada posicion de la secuencia donde puede caber la subsecuencia
        for (itdesc; itdesc + subSecuencia.size() <= desc.end(); itdesc++)
        {
            bool coincide = true; // coincide hasta q se demustre lo contrario

            // Comparamos caracter por caracter la subsecuencia con la secuencia actual
            string::iterator itLss = subSecuencia.begin();
            for (itLss; itLss != subSecuencia.end(); itLss++)
            {
                // Compara el caracter actual de la subsecuencia (*itLss)
                // con el caracter correspondiente en la secuencia.
                // (itLss - subSecuencia.begin()) calcula cuantas posiciones
                // avanzar desde itdesc para llegar al caracter que toca comparar
                if (*(itdesc + (itLss - subSecuencia.begin())) != *itLss)
                {
                    coincide = false; // Si no coincide, rompemos el ciclo
                    break;
                }
            }

            // Si encontramos una coincidencia completa, reemplazamos los caracteres por 'X'
            if (coincide)
            {
                string::iterator itMascara = itdesc;
                for (int i = 0; i < subSecuencia.size(); i++, itMascara++)
                {
                    *itMascara = 'X'; // Cambiamos cada letra por 'X'
                }
                // Movemos el iterador principal para saltar la subsecuencia q ya enmascaramos
                itdesc += subSecuencia.size() - 1;
            }
        }

        

        // Guardamos la secuencia modificada de vuelta en el objeto Secuencia
        itLst->setADN(desc);
        cout<<desc<<endl;  

    }
}


void histograma(const vector<Secuencia>& listaSecuencias, const string& nombre) {
    vector<char> bases = {'A','C','G','T','U','R','Y','K','M','S','W','B','D','H','V','N','X','-'};
    vector<int> conteo(bases.size(), 0);

    const Secuencia* encontrada = nullptr;
    for (Secuencia seq : listaSecuencias) {
        if (seq.getNombre() == nombre) {
            encontrada = &seq;
            break;
        }
    }

    if (!encontrada) {
        cout << "Secuencia inválida." << endl;
        return;
    }

    for (char base : encontrada->adn) {
        for (int j = 0; j < bases.size(); j++) {
            if (base == bases[j]) {
                conteo[j]++;
                break;
            }
        }
    }

    // Histograma   
    for (size_t j = 0; j < bases.size(); j++) {
        cout << bases[j] << " : " << conteo[j] << "\n";
    }

}
