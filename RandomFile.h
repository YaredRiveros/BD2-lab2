#include <iostream>
#include<fstream>
#include<cstdio>
#include <map>
using namespace std;

struct Record
{
    char nombre[30];
    char carrera[20];
    int ciclo = -1;

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
        cout << "\n----" << endl;
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
    * Lee el archivo _ind.dat, si existe, y carga el contenido en el map
    */
    void readIndex()
    {
      ifstream indexfile;
        indexfile.open(this->indexName, ios::in | ios::out | ios::binary);
        if (!indexfile.is_open()) {
        throw runtime_error("No se pudo abrir el archivo de índice");
        }
        if(indexfile.good())    //Si el archivo existe
        {
            //Compruebo que el archivo no este vacio
            long posFisica;
            indexfile.seekg(0,ios::beg);
            indexfile.read((char*)&posFisica, sizeof(posFisica)); //leo el primer registro para ver si esta vacio

            if(indexfile.tellg() != -1){
                char* key = new char[30];
                indexfile.seekg(0, ios::end); //seekg: posiciona el puntero al final del archivo
                int fin = indexfile.tellg();
                indexfile.seekg(0, ios::beg); //seekg: posiciona el puntero al inicio del archivo

                while(indexfile.tellg()<fin){ // end of file
                    posFisica = indexfile.tellg(); //tellg: devuelve la posicion actual del puntero
                    indexfile.read(key, 30);
                    indexfile.read((char*)&posFisica, sizeof(posFisica));
                    this->index[key] = posFisica;
                }

            }
            
        }
        indexfile.close();
    }

    /*
    * Al finalizar le programa, sobreescribe el contenido del map en el archivo _ind.dat
    */
    void writeIndex(){
        ofstream file(this->indexName, ios::out | ios::binary);
        if (!file.is_open()) {
        throw runtime_error("No se pudo abrir el archivo de índice para escribir");
        }
        for(auto& entry : index){
            //Guardo la key
            file.write(entry.first.c_str(), 30);
            //Guardo la posicion del registro en data.dat
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
       // Nombre es llave única por lo que no se puede repetir
        if(this->index.find(record.getKey()) != this->index.end()){
            try{
                throw "Ya existe un registro con el mismo nombre";
            }catch(const char* msg){
                cerr << msg << endl;
            }
        }else{
            dataFile.write((char*)&record, sizeof(Record));
            this->index[record.getKey()] = posFisica;
        }
        dataFile.close();
    }


    /*
    * Busca un registro que coincida con la key
    */
    Record search(string key) {
        Record result;
        //Si no se encuentra el registro, se retorna un registro vacio
        auto it = this->index.find(key);
        if(it == this->index.end())
            return result;
        //Si se encuentra el registro, se retorna el registro
        long posFisica = this->index[key];
        ifstream dataFile;
        dataFile.open(this->fileName, ios::in | ios::binary);
        dataFile.seekg(posFisica, ios::beg);
        dataFile.read((char*)&result, sizeof(Record));
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
            Record record = search(it->first);
            if(record.ciclo==-1)
                cout << "No se encontró el registro" << endl;
            else
                record.showData();
        }
    }

};


