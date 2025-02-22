#include <iostream>
#include <vector>
#include <thread>
#include <chrono>  // Para medir tiempo

using namespace std;

// SUMA SECUENCIAL

long  suma_secuencial(const vector<int>& datos) {
    long suma = 0;
    for (size_t i = 0; i < datos.size(); i++) {
        suma += datos[i];
    }
    return suma;
}

// SUMA PARALELA

void suma_parcial(const vector<int>& datos, int inicio, int fin, long long& resultado) {
    resultado = 0;
    for (int i = inicio; i < fin; i++) {
        resultado += datos[i];
    }
}

 long suma_paralela(const vector<int>& datos, int num_hilos) {
    int n = datos.size();
    vector<thread> hilos;
    vector<long long> resultados(num_hilos, 0);
    int bloque = n / num_hilos;

    //  Particion y Comunicacion
    for (int i = 0; i < num_hilos; i++) {
        int inicio = i * bloque;
        int fin = (i == num_hilos - 1) ? n : inicio + bloque; 
        hilos.push_back(thread(suma_parcial, ref(datos), inicio, fin, ref(resultados[i])));
    }


    for (auto& hilo : hilos) {
        hilo.join();
    }

    /📌 Aglomeracion
     long suma_total = 0;
    for (int i = 0; i < num_hilos; i++) {
        suma_total += resultados[i];
    }

    return suma_total;
}


int main() {
    const int N = 50000000;
    vector<int> datos(N, 1);
    int num_hilos = thread::hardware_concurrency();


    auto inicio_seq = chrono::high_resolution_clock::now();
     long suma_seq = suma_secuencial(datos);
    auto fin_seq = chrono::high_resolution_clock::now();
    chrono::duration<double> tiempo_seq = fin_seq - inicio_seq;


    auto inicio_par = chrono::high_resolution_clock::now();
     long suma_par = suma_paralela(datos, num_hilos);
    auto fin_par = chrono::high_resolution_clock::now();
    chrono::duration<double> tiempo_par = fin_par - inicio_par;
    double speedup = tiempo_seq.count() / tiempo_par.count();

    cout << "Suma secuencial: " << suma_seq << " en " << tiempo_seq.count() << " segundos.\n";
    cout << "Suma paralela:   " << suma_par << " en " << tiempo_par.count() << " segundos.\n";
    cout << "Speedup: " << speedup << "x\n";

    return 0;
}
