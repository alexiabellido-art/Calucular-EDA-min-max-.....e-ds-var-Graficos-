#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
using namespace std;

double hallarMediana(double a[], int ini, int fin) {
    int m = (ini + fin) / 2;
    if ((fin - ini) % 2 == 0) return (a[m - 1] + a[m]) / 2.0;
    return a[m];
}

int posicion(double x, double minVal, double maxVal) {
    if (maxVal == minVal) return 0;
    return (int)((x - minVal) / (maxVal - minVal) * 40 + 0.5);
}

int main() {
    const int MAX = 1000;
    double numeros[MAX], original[MAX];
    int n;

    do {
        cout << "Ingrese la cantidad de numeros (1-1000): ";
        cin >> n;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            n = 0;
        }
    } while (n < 1 || n > MAX);

    double suma = 0;
    for (int i = 0; i < n; i++) {
        cout << "Ingrese el numero " << i + 1 << ": ";
        while (!(cin >> numeros[i])) {
            cout << "Error. Ingrese un numero valido: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        original[i] = numeros[i];
        suma += numeros[i];
    }

    
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (numeros[j] > numeros[j + 1]) {
                double auxiliar = numeros[j];
                numeros[j] = numeros[j + 1];
                numeros[j + 1] = auxiliar;
            }
        }
    }

    double minimo = numeros[0];
    double maximo = numeros[n - 1];
    double media = suma / n;

    double varianza = 0;
    for (int i = 0; i < n; i++) {
        varianza += (numeros[i] - media) * (numeros[i] - media);
    }
    varianza /= n;
    double desviacion = sqrt(varianza);

    double mediana = hallarMediana(numeros, 0, n);
    double q1 = (n > 1) ? hallarMediana(numeros, 0, n / 2) : mediana;
    double q3 = (n > 1) ? hallarMediana(numeros, (n + 1) / 2, n) : mediana;
    double iqr = q3 - q1;
    double limInf = q1 - 1.5 * iqr;
    double limSup = q3 + 1.5 * iqr;


    double moda = numeros[0];
    int maxRep = 1;
    for (int i = 0; i < n; i++) {
        int rep = 0;
        for (int j = 0; j < n; j++) {
            if (numeros[j] == numeros[i]) rep++;
        }
        if (rep > maxRep) {
            maxRep = rep;
            moda = numeros[i];
        }
    }

    cout << fixed << setprecision(2);
    cout << "\n=== ANALISIS EXPLORATORIO DE DATOS ===\n";
    cout << "Cantidad de datos: " << n << '\n';
    cout << "Valor minimo: " << minimo << '\n';
    cout << "Valor maximo: " << maximo << '\n';
    cout << "Rango: " << maximo - minimo << '\n';
    cout << "Media: " << media << '\n';
    cout << "Mediana: " << mediana << '\n';
    if (maxRep > 1) cout << "Moda: " << moda << " (" << maxRep << " veces)\n";
    else cout << "Moda: no hay\n";
    cout << "Q1: " << q1 << " | Q3: " << q3 << " | IQR: " << iqr << '\n';
    cout << "Varianza poblacional: " << varianza << '\n';
    cout << "Desviacion estandar poblacional: " << desviacion << '\n';
    if (media != 0) {
        cout << "Coeficiente de variacion: " << (desviacion / fabs(media)) * 100 << "%\n";
    } else {
        cout << "Coeficiente de variacion: no definido (media igual a cero)\n";
    }

    double bigoteMin = maximo, bigoteMax = minimo;
    cout << "Datos atipicos: ";
    bool hayAtipicos = false;
    for (int i = 0; i < n; i++) {
        if (numeros[i] < limInf || numeros[i] > limSup) {
            cout << numeros[i] << " ";
            hayAtipicos = true;
        } else {
            if (numeros[i] < bigoteMin) bigoteMin = numeros[i];
            if (numeros[i] > bigoteMax) bigoteMax = numeros[i];
        }
    }
    if (!hayAtipicos) cout << "ninguno";
    cout << "\n";

    cout << "\n=== DIAGRAMA DE BARRAS ===\n";
    double mayorAbs = fabs(minimo) > fabs(maximo) ? fabs(minimo) : fabs(maximo);
    for (int i = 0; i < n; i++) {
        int largo = (mayorAbs > 0) ? (int)(fabs(original[i]) / mayorAbs * 30 + 0.5) : 0;
        cout << "Dato " << i + 1 << " | ";
        for (int j = 0; j < largo; j++) cout << "█";
        cout << " (" << original[i] << ")\n";
    }

  
    cout << "\n=== BOX PLOT ===\n";
    string linea[41];
    for (int i = 0; i < 41; i++) linea[i] = " ";
    for (int i = posicion(bigoteMin, minimo, maximo); i <= posicion(bigoteMax, minimo, maximo); i++) linea[i] = "-";
    for (int i = posicion(q1, minimo, maximo); i <= posicion(q3, minimo, maximo); i++) linea[i] = "█";
    linea[posicion(mediana, minimo, maximo)] = "M";
    for (int i = 0; i < n; i++) {
        if (numeros[i] < limInf || numeros[i] > limSup) {
            linea[posicion(numeros[i], minimo, maximo)] = "*";
        }
    }
    for (int i = 0; i < 41; i++) cout << linea[i];
    cout << '\n';

    return 0;
}
