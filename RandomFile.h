#include <iostream>
#include<fstream>
#include<cstdio>
#include <map>
using namespace std;

struct Record
{
    char nombre[30];
    char carrera[20];
    int ciclo;

    string getKey() {
        return string(nombre);
    }

    void setData() {
        cout << "Alumno:";
        cin >> nombre;
        cout << "Carrera: ";
        cin >> carrera;
        cout << "Ciclo: ";
        cin >> ciclo;
    }

    void showData() {
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
    }    
};

class RandomFile {
private:
    string fileName;
    string indexName;
    //map: mantiene ordenado las entradas
    map<string, long> index;

public:
    RandomFile(string _fileName) {
        this->fileName = _fileName;
        this->indexName = "_ind.dat";
        readIndex();
    }
    
    ~RandomFile(){
        writeIndex();
    }

    /*
    * Lee el archivo donde guardamos una copia del árbol (key,posFisica) para no volver a crear el árbol cada vez que se ejecute el programa
    */
    void readIndex()
    {
        cout << "1" << endl;
      ifstream indexfile;
        indexfile.open(this->indexName, ios::in | ios::binary);
        if(indexfile.is_open())
        {
            cout << "2" << endl;
            Record record;
            indexfile.seekg(0, ios::end); //seekg: posiciona el puntero al final del archivo
            int fin = indexfile.tellg(); 
            indexfile.seekg(0, ios::beg); //seekg: posiciona el puntero al inicio del archivo
            cout << "3" << endl;
            while(indexfile.tellg()<fin){ // end of file
                cout << "posicion: " << indexfile.tellg() << endl;
                cout << "fin: " << fin << endl;
                long posFisica = indexfile.tellg(); //tellg: devuelve la posicion actual del puntero
                indexfile.read((char*)&record, sizeof(Record));
                this->index[record.getKey()] = posFisica;
            }
            cout << "4" << endl;
        }
        cout << "salio del while" << endl;
    }

    /*
    * Todo lo que está en el árbol se escribe en el archivo de índices para que cuando se ejecute el programa de nuevo solo lo leamos y carguemos a un árbol
    */
    void writeIndex(){
        ofstream file(this->indexName, ios::out | ios::binary);
        for(auto& entry : index){
            file.write(entry.first.c_str(), sizeof(entry.first));
            file.write((char*)&entry.second, sizeof(entry.second));
        }
        file.close();
    }

    /*
    * Escribe el registro al final del archivo de datos. Se actualiza el indice. 
    */
    void write_record(Record record) {
       ofstream dataFile;
       dataFile.open(this->fileName, ios::app | ios::binary);
       long posFisica = dataFile.tellp();
       dataFile.write((char*)&record, sizeof(Record));
       this->index[record.getKey()] = posFisica;
       dataFile.close();
    }


    /*
    * Busca un registro que coincida con la key
    */
    Record* search(string key) {
        Record* result = nullptr;
        int posFisica = this->index[key];
        ifstream dataFile;
        dataFile.open(this->fileName, ios::in | ios::binary);
        dataFile.seekg(posFisica, ios::beg);
        dataFile.read((char*)result, sizeof(Record));
        dataFile.close();
        return result;
    }

    /*
   * Muestra todos los registros de acuerdo como fueron insertados en el archivo de datos
   */
    void scanAll() {
        ifstream dataFile;
        dataFile.open(this->fileName, ios::in | ios::binary);
        dataFile.seekg(0, ios::end); //seekg: posiciona el puntero al final del archivo
        int fin = dataFile.tellg();
        dataFile.seekg(0, ios::beg); //seekg: posiciona el puntero al inicio del archivo
        Record record;
        while(dataFile.tellg()<fin){ // end of file
            dataFile.read((char*)&record, sizeof(Record)); //leer del archivo
            record.showData();
        }
        dataFile.close();
    }

    /*
   * Muestra todos los registros de acuerdo a como estan ordenados en el indice
   */
    void scanAllByIndex() {
       for(auto it=index.begin(); it!=index.end(); ++it){
            Record* record = search(it->first);
            record->showData();
        }
    }

};


