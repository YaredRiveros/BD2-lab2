#include "RandomFile.h"

using namespace std;

void writeFile(string filename){
    RandomFile file(filename);
    Record record;
    for (int i = 0; i < 2; i++)
    {
        record.setData();
        file.write_record(record);
    }
    cout << "olii" << endl;   
}

void readFile(string filename){
    RandomFile file(filename);
    cout<<"--------- show all data -----------\n";
    file.scanAll();
    cout<<"--------- show all sorted data -----------\n";
    file.scanAllByIndex();   
}

int main(){
    writeFile("data.dat");
    cout << "a" << endl;
    readFile("data.dat");
    cout << "b" << endl;
    return 0;
}