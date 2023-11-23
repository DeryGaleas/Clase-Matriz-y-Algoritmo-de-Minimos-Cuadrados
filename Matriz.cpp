#include <iostream>
#include <fstream> 
using namespace std;

const double EPSILON = 1e-10;



class Matriz{
    public:
        int filas;
        int columnas;
        double ** arregloMatriz;
        double ** transpuesta; 


    public:
        //Constructores
        Matriz(int x, int y, bool matriz_nula=true);
        Matriz(const char *nombreArchivo);

        //Destructor
        ~Matriz();

        //Prototipo de Sobrecarga de Operadores
        Matriz operator+(const Matriz & matrizB);
        Matriz operator-(const Matriz & matrizB);
        Matriz operator*(const Matriz & matrizB);
        //Metodos
        void imprimir();
        Matriz obtener_transpuesta();
        void reduccionGaussJordan();
        void intercambiarFilas(int fila1, int fila2);
        void escalarFila(int fila, double escalar);
        void sumarFilas(int filaDestino, int filaOrigen, double factor);
        void agregarColumna(const Matriz &nuevaColumna);
};

//Metodo de Minimos Cuadrados por Transpuesta
Matriz minimos_cuadrados(Matriz &matrizA, Matriz &matrizB);


int main(){
    Matriz matrizA("matrizA.txt");
    Matriz Vector("vector.txt");
    minimos_cuadrados(matrizA, Vector);    

    return 0;
}


//Constructores
Matriz::Matriz(int x, int y, bool matriz_nula){
    this->filas = x;
    this->columnas = y;

    this->arregloMatriz = new double*[filas];
    for (int i=0; i<filas; i++){
        this->arregloMatriz[i] = new double[columnas];

        for (int j=0; j<columnas; j++){
            if (matriz_nula){
                this->arregloMatriz[i][j]=0;
            }else{
                 cout<< "Ingrese elemento ("<<i+1<<", "<<j+1<<") : ";
                cin>> this->arregloMatriz[i][j];
            } 
        }  
    }
}

Matriz::Matriz(const char *nombreArchivo){

    ifstream archivo(nombreArchivo);
    if (!archivo){
        cerr<< "No se pudo encontrar el archivo "<<nombreArchivo <<endl;
        exit(1);
    }

    archivo >> filas >> columnas;

    this->arregloMatriz = new double *[filas];
    for (int i = 0; i < filas; i++) {
        this->arregloMatriz[i] = new double[columnas];

        for (int j = 0; j < columnas; j++) {

            archivo >> this->arregloMatriz[i][j];
        }
    }

    archivo.close();
}


//Destructor
Matriz::~Matriz(){
    for (int i=0; i<filas; i++){
        delete[] this->arregloMatriz[i];
    }
    delete[] arregloMatriz;
};


//Sobrecarga de OperadoresS
Matriz Matriz::operator+(const Matriz & matrizB){
    if(this->filas != matrizB.filas || this->columnas != matrizB.columnas){
        cerr<<"No se pueden sumar matrices con diferentes dimensiones." <<endl;
        exit(1);
    }
     Matriz resultado(this->filas, this->columnas);

     for (int i=0; i< this->filas; i++){
        for(int j=0; j< this->columnas; j++){
            resultado.arregloMatriz[i][j] = this->arregloMatriz[i][j]+ matrizB.arregloMatriz[i][j];
        }
     }
     return resultado;
}


Matriz Matriz::operator-(const Matriz & matrizB){
    if(this->filas != matrizB.filas || this->columnas != matrizB.columnas){
        cerr<<"No se pueden sumar matrices con diferentes dimensiones." <<endl;
        exit(1);
    }
     Matriz resultado(this->filas, this->columnas);

     for (int i=0; i< this->filas; i++){
        for(int j=0; j< this->columnas; j++){
            resultado.arregloMatriz[i][j] = this->arregloMatriz[i][j]- matrizB.arregloMatriz[i][j];
        }
     }
     return resultado;
}


Matriz Matriz:: operator*(const Matriz & matrizB){

    if (this->columnas != matrizB.filas){
        cerr <<"No pueden multiplicarse matrices con estas dimensiones" <<endl;
        exit(1);
    }

    Matriz resultado(this->filas, matrizB.columnas);

    for (int i = 0; i<this->filas; i++){

        for (int j =0; j<matrizB.columnas; j++){

            for(int k =0; k<this->columnas; k++){

                resultado.arregloMatriz[i][j] += this->arregloMatriz[i][k] * matrizB.arregloMatriz[k][j];

            }
        }
    }

    return resultado;
}





//Metodos
void Matriz::imprimir(){
    cout<<endl;
    cout<<"Matriz: ";
    for (int i=0; i<filas; i++){
        cout<<endl;
        for (int j=0; j<columnas; j++){
            
            cout<< this->arregloMatriz[i][j]<<"    ";
        }
    }
};

Matriz Matriz::obtener_transpuesta(){
    Matriz transpuesta(columnas, filas);

    for (int i=0; i<filas; i++){
        for(int j=0; j<columnas; j++){
            transpuesta.arregloMatriz[j][i] = arregloMatriz[i][j];
        }
    }
    return transpuesta;
}

void Matriz::reduccionGaussJordan() {
    int filaPivote = 0; 

    for (int col = 0; col < columnas - 1; col++) {
        // Encontrar el pivote no nulo en la columna actual
        while (filaPivote < filas && abs(arregloMatriz[filaPivote][col]) < EPSILON) {
            filaPivote++;
        } 

        if (filaPivote == filas) {
            // No hay pivote no nulo en esta columna, pasar a la siguiente
            continue;
        }

        // Hacer que el pivote sea igual a 1
        escalarFila(filaPivote, 1 / arregloMatriz[filaPivote][col]);

        // Hacer ceros en las demás filas
        for (int fila = 0; fila < filas; fila++) {
            if (fila != filaPivote) {
                sumarFilas(fila, filaPivote, -arregloMatriz[fila][col]);
            }
        }
        // Mover al siguiente pivote
        filaPivote++;
    }
}

void Matriz::intercambiarFilas(int fila1, int fila2) {
    swap(arregloMatriz[fila1], arregloMatriz[fila2]);
}

void Matriz::escalarFila(int fila, double escalar) {
    for (int col = 0; col < columnas; col++) {
        arregloMatriz[fila][col] *= escalar;
    }
}

void Matriz::sumarFilas(int filaDestino, int filaOrigen, double factor) {
    for (int col = 0; col < columnas; col++) {
        arregloMatriz[filaDestino][col] += factor * arregloMatriz[filaOrigen][col];
    }
}


void Matriz::agregarColumna(const Matriz &nuevaColumna) {
    // Verificar que las dimensiones sean compatibles
    if (filas != nuevaColumna.filas) {
        cerr << "Error: Las dimensiones de las matrices no son compatibles." << endl;
        return;
    }

    // Crear una nueva matriz con una columna adicional
    double **nuevaMatriz = new double *[filas];
    for (int i = 0; i < filas; i++) {
        nuevaMatriz[i] = new double[columnas + 1];
        for (int j = 0; j < columnas; j++) {
            nuevaMatriz[i][j] = arregloMatriz[i][j];
        }
        nuevaMatriz[i][columnas] = nuevaColumna.arregloMatriz[i][0];
    }

    // Liberar la memoria de la matriz original
    for (int i = 0; i < filas; i++) {
        delete[] arregloMatriz[i];
    }
    delete[] arregloMatriz;

    // Actualizar la matriz con la columna adicional
    arregloMatriz = nuevaMatriz;
    columnas++;
}


// Metodos externos a la clase Matriz

Matriz minimos_cuadrados(Matriz &matrizA, Matriz &matrizB){
    
    if(matrizA.filas != matrizB.filas || matrizB.columnas!=1){
        cerr<<"Las dimensiones de la Matriz no son compatibles con el Vector L.I.";
        exit(1);
    }

    cout<<endl;
    cout<<"Matriz A";
    matrizA.imprimir();

    cout<<endl;
    cout<<"Vector";
    matrizB.imprimir();

    Matriz A_transpuesta = matrizA.obtener_transpuesta();

    // Desarrollo de la igualdad lado izquierdo
    Matriz operacion_zurda = A_transpuesta*matrizA;

    // Desarrollo de la igualdad lado derecho
    Matriz operacion_diestra = A_transpuesta*matrizB;

    //Crear Matriz Aumentada
    operacion_zurda.agregarColumna(operacion_diestra);
    Matriz operacion_final = operacion_zurda;

    //Resolver sitema de Ecuaciones lineal
    cout<<endl;
    cout<<"Resultado de A*x = b";
    operacion_final.reduccionGaussJordan();
    operacion_final.imprimir();

    return A_transpuesta;

}

