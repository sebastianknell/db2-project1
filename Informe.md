# Base de Datos 2 - Proyecto 1
### Integrantes
- Anthony Guimarey Saavedra
- Massimo Imparato Conetta
- Sebastian Knell Noriega

## Índice
  - [Introducción](#introducción)
  - [Implementación](#implementación)
    - [Sequential File](#sequential-file)
    - [ISAM](#isam)
  - [Resultados](#resultados)
  - [Prueba de uso](#prueba-de-uso)
  - [Anexos](#anexos)

## Introducción
El objetivo de este proyecto fue implementar dos estructuras de organización de archivos en memoria secundaria utilizando las técnicas vistas en
el curso y comparar su rendimiento. Se pueden realizar las siguientes cuatro operaciones: búsqueda puntual, búsqueda por rango, inserción y eliminación. 
Concretamente decidimos implementar un Sequential File y un Indexed Sequential Access Method (ISAM). Se uso un conjunto de datos de prueba obtenido de [Kaggle](https://www.kaggle.com/datasets). 
Este conjunto de datos se utiliza para predecir si es probable que un paciente sufra un accidente cerebrovascular, teniendo campos como el sexo, la edad, diversas enfermedades y el tabaquismo. 
De acuerdo a lo aprendido, se espera observar un mejor desempeño en las inserciones y búsquedas en el ISAM.

## Implementación
## Sequential File
Para implementar el Sequential File se utilizó archivos de longitud fija. Logramos implementar los 4 métodos principales solicitados: búsqueda, búsqueda por rango, inserción y eliminación. A continuación se muestra la definición de la clase.
```cpp
class SequentialFile {
    string data_file;
    string aux_file = "aux.dat";
    int max_aux_size;
public:
    explicit SequentialFile(string data_file = "seq-file-data.dat", int max_aux_size = 10):
        data_file(std::move(data_file)), max_aux_size(max_aux_size) {};
    void load_data(const string&);
    void print_all();
    optional<FixedRecord> search(int);
    vector<FixedRecord> range_search(int, int);
    bool insert(FixedRecord);
    bool remove(int);
    void merge_data();
};
```
El método `merge_data()` lo utilizamos para combinar los registros que hay en el archivo de data (seq-file-data.dat) y el auxiliar (aux.dat) una vez que este pase cierta cantidad de registros `max_aux_size`. Este consiste en encontrar la primera referencia a un registro en el archivo auxiliar y escribir el resto de forma ordenada en el archivo de data, para lo cual se usa un vector de apoyo.

En un inicio utilizamos registros de longitud variable, sin embargo tuvimos dificultades para implementar la búsqueda binaria en este contexto. Los registros fijos nos dan la facilidad de movernos por el data file como si fuera un arreglo. Para leer y escribir estos registros no necesitamos de la clase [Buffer](utils/Buffer.h), sino que lo hacemos directamente con el objeto. Así el proceso de lectura y escritura serían los siguientes: `stream.read((char*)&record, sizeof(record))` para leer y `stream.write((char*)&record, sizeof(record))` para escribir.
## ISAM
Para ISAM utilizamos registros de longitud variable. También logramos implementar los 4 métodos principales para estos: Búsqueda, Búsqueda por rango, Inserción y Eliminación.
Comenzemos con la definición de la clase [ISAM](ISAM.h).
### Definición de la clase ISAM
```cpp
struct IndexRecord {
    int id{};
    long long pos{};
};

class ISAM {
    string dataFile;
    string indexFile = "index.dat";

    static bool readIndex(IndexRecord&, ifstream&);
    static bool readIndex(IndexRecord&, fstream&);
    static bool writeIndex(IndexRecord&, ofstream&);
    static bool writeIndex(IndexRecord &indexRecord, fstream &stream);
    static bool readRecord(Record &, ifstream&);
    static bool writeRecord(Record &, ofstream&, unsigned long&);

public:
    explicit ISAM(string dataFile = "ISAM-data.dat"): dataFile(move(dataFile))  {};
    void loadData(const string&);
    void printAll();
    optional<Record> search(int);
    vector<Record> rangeSearch(int, int);
    void insert(Record);
    bool remove(int);
    static long long getFileSize(ifstream&);
    static long long getFileSize(fstream&);
    static long long find(int id, fstream &stream);
    void printIndex();
};
```

Los métodos; `readIndex()`, `writeIndex()`, `readRecord()`, `writeRecord()`, `getFileSize()` y `printIndex()` son métodos de utilidad para ser usados por otras funciones ó para comprobar que todo esté funcionando correctamente. Se puede observar el struct de `IndexRecord` donde tenemos los atributos: `id` y `pos`. Los cuales `id` hace referencia al ID del registro en data y `pos` hace referencia a la posición del registro en el archivo ISAM-data.dat. Las funiones `readIndex()` y `writeIndex()` nos sirven para escribir y leer un struct IndexRecord al archivo index.dat. Similarmente, `readRecord()` y `writeRecord()` nos sirven para escribir y leer un registro en el archivo ISAM-data.dat.

Por otro lado, nuestras funciones principales son; `loadData()`, `printAll()`, `search()`, `rangeSearch()`, `insert()` y `remove()`.

La función `loadData()` lee todos los datos del archivo csv y a partir de ellos construye registros e índices y los escribe en los archivos. A través de la librería rapidcsv, leemos los datos de [Stroke-data.csv](data/stroke-data.csv) y en base a ellos, para cada línea, armamos un registro con la función `load_data` de [Record](Record.cpp) y con el index de este y la posicion en el data file donde lo vamos a escribir instanciamos un objecto `IndexRecord`. Finalmente escribimos el registro con `writeRecord()` y el indexRecord.
### loadData()
```cpp
void ISAM::loadData(const string& fromFilename) {
    Record temp;
    ofstream data(dataFile, ios::out | ios::binary);
    ofstream index(indexFile, ios::out | ios::binary);
    if(!data) return;
    rapidcsv::Document document(fromFilename);
    auto len = document.GetRowCount();
    unsigned long offset = 0;
    IndexRecord indexRecord;

    for(int i = 0 ; i < len ; i++)
    {
        vector<string> row = document.GetRow<string>(i);
        temp.load_data(row);
        indexRecord.pos = data.tellp();
        writeRecord(temp, data, offset);
        indexRecord.id = temp.get_key();
        index.write((char*)&indexRecord, sizeof(indexRecord));
    }
}
```
La función `writeRecord()` hace uso de la clase de utilidad [Buffer](utils/Buffer.cpp) para construir el registro y para escribir el registro:
```cpp
bool ISAM::writeRecord(Record &record, ofstream &stream, unsigned long &offset) {
    Buffer buffer;
    record.pack(buffer);
    bool success = buffer.write(stream);
    return success;
}
```
```cpp
bool Buffer::pack(const char *str, int size) {
    short len = 0;
    if (size >= 0) len = size;
    else len = strlen(str);
    if (len > strlen(str)) return false;
    int start = next_byte;
    next_byte += len + 1;
    if (next_byte > max_bytes) return false;
    memcpy(&buffer[start], str, len);
    buffer[start + len] = delimeter;
    buffer_size = next_byte;
    return true;
}
```
### printAll()
La función `printAll()` itera y lee todo el archivo `index.dat` a través de la función `readIndex()`, el cual recibe una referencia a un objeto indexRecord y un stream, para así leer esa posición en el stream y asignarla al objeto. Luego lee la `pos` de indexRecord que apunta a la posición del respectivo registro en el archivo `ISAM-data.dat`, leerlo e imprimirlo.
```cpp
void ISAM::printAll() {
    ifstream data(dataFile, ios::binary);
    ifstream index(indexFile, ios::binary);
    IndexRecord indexRecord;
    Record record;
    while (readIndex(indexRecord, index)) {
        data.seekg(indexRecord.pos);
        bool success = readRecord(record, data);
        if (!success) break;
        record.print();
    }
}
```
## search()
La función `optional<Record> search(id)` abre ambos archivos en modo de lectura. Instancia objetos de tipo Record y IndexRecord para leer. Luego hace una búsqueda binaria donde `l `= 0 y `r `= tamaño del archivo `index.dat` / el tamaño de un indexRecord. Luego se va moviendo en el archivo con `seekg()` y lee los indexRecord para comparar el `indexRecord.id` contra el id a buscar. Si lo encuentra, posiciona el `data` file en la posición del `indexRecord.pos`, lee el registro y lo devuelve. De modo contrario, utilizando `optional` devuelve un null optional.
```cpp
optional<Record> ISAM::search(int id){
    ifstream data(dataFile, ios::binary);
    ifstream index(indexFile, ios::binary);
    IndexRecord indexRecord;
    Record temp;
    long indexSize = sizeof(indexRecord);
    long long l = 0;
    long long r = (getFileSize(index) / indexSize) -1 ;
    while(r > l) {
        long long mid = (r+l) / 2;
        index.seekg(mid*indexSize);
        readIndex(indexRecord, index);
        if(indexRecord.id < id) l = mid+1;
        else if (indexRecord.id > id) r = mid-1;
        else {
            data.seekg(indexRecord.pos);
            if (readRecord(temp, data)) {
                data.close();
                index.close();
                return temp;
            }
            else return nullopt;
        }
    }
    readIndex(indexRecord, index);
    if(indexRecord.id == id) {
        data.seekg(indexRecord.pos);
        if(readRecord(temp, data)) {
            printIndexRecord(indexRecord);
            data.close();
            index.close();
            return temp;
        }
    } else return nullopt;
    return nullopt;
}
```
## rangeSearch()
La función `rangeSearch()` hace uso de la función de utilidad `find()`:
```cpp
long long ISAM::find(int id, fstream& stream){
    long indexSize = sizeof(IndexRecord);
    long long l = 0;
    long long r = (getFileSize(stream) / indexSize) - 1;
    long long pos;
    IndexRecord indexRecord;

    while(r > l) {
        long long m = (r+l) / 2;
        pos = m * indexSize;
        stream.seekg(m*indexSize);
        readIndex(indexRecord, stream);
        if(indexRecord.id < id) l = m + 1;
        else if(indexRecord.id > id) r = m-1;
        else {
            return pos;
        }
    }
    while(indexRecord.id < id) {
        pos += indexSize;
        stream.seekg(pos);
        readIndex(indexRecord, stream);
    }
    while (indexRecord.id > id) {
        pos -= indexSize;
        stream.seekg(pos);
        readIndex(indexRecord, stream);
    }
    return pos;
}
```
La cual es una búsqueda binaria con básicamente la misma lógica que en `search()`, con la única diferencia que: si no encuentra el id en el index, devolverá la posición del index anterior al buscado (mientras que el search o encuentra el id o no lo hace). Lo cual es muy útil para `rangeSearch(id1, id2)` ya que a partir de esa posición podemos iterar y leer los índices hasta que el `indexRecord.pos` sea igual o mayor que el id2 e ir agregando al vector de resultado cada registro correspondiente.
```cpp
vector<Record> ISAM::rangeSearch(int id1, int id2){
    ifstream data(dataFile, ios::binary);
    fstream index(indexFile, ios::in | ios::binary);
    IndexRecord indexRecord;
    Record temp;
    vector<Record> result;
    auto pos = find(id1, index);
    index.seekg(pos);
    readIndex(indexRecord, index);
    while(indexRecord.id < id2) {
        data.seekg(indexRecord.pos);
        readRecord(temp, data);
        result.push_back(temp);
        readIndex(indexRecord, index);
    }
    data.close();
    index.close();
    return result;
}
```
## insert()
La función `insert(Record)` ya es un poco más complicada en una estructura ISAM. Esto se debe a que nuestro archivo `index.dat` tiene que siempre estar ordenado, mientras que nuestro archivo `ISAM-data.dat` lo más probable es que esté desordenado (ya que no es necesario por que los índices apuntaran a las posiciones). Como consecuencia, para "insertar algun index en medio del `index.dat`" lo que debemos hacer es guardar todos los índices que tenemos en el archivo en un vector, insertar en el vector en el lugar correcto el nuevo index, y luego reescribir todo el archivo. Mientras que, en el `ISAM-data.dat` el nuevo record será simplemente escrito al final del archivo.

Entonces el procedimiento es este. Encontramos la posición a la que se debe insertar el registro a través de `find()`, leemos los índices hasta esa posición, agregándolos a un vector `vector<IndexRecord> rewrite`, agregamos el nuevo índice al mismo vector, y luego leemos y agregamos los índices restantes. Finalmente escribimos el record al final del `data file` y, iterando el vector rewrite, reescribimos todo el `index file`.
```cpp
void ISAM::insert(Record record) {
    ofstream data(dataFile, ios::ate | ios::app | ios::binary);
    fstream index(indexFile, ios::in | ios::out | ios::binary);
    if(!data || !index) return;
    IndexRecord indexRecord;
    vector<IndexRecord> rewrite;
    unsigned long offset = 0;
    int id = record.get_key();

    auto pos = find(id, index);
    index.seekg(0);
    while(index.tellg() <= pos) {
        readIndex(indexRecord, index);
        rewrite.push_back(indexRecord);
    }

    index.seekg(pos + sizeof(IndexRecord)); // located at insertion pos

    // iterate indexes
    indexRecord.pos = data.tellp();
    indexRecord.id = id;
    rewrite.push_back(indexRecord);
    while(readIndex(indexRecord, index)) {
        rewrite.push_back(indexRecord); // add to vector
    }

    writeRecord(record, data, offset);

    data.close();
    index.close();
    ofstream indexOut(indexFile, ios::out | ios::trunc | ios::binary);
    for(auto & i : rewrite) {
        writeIndex(i, indexOut);
    }
}
```
## remove()
Del mismo modo, la función `remove(id)`, al igual que `insert()`, tiene un procedimiento complicado en esta estructura. Como en el insert, no podemos simplemente alterar o borrar algo en medio del archivo, por lo que debemos guardar lo que tenemos sin el registro e índice que queremos eliminar, y luego reescribir los archivos.

Para esto; primero nos aseguramos de que el id que queremos remover exista. Luego, utilizando la función `find()` encontramos la posición donde está el índice a eliminar (el índice es lo importante ya que nos apuntará a el registro directamente). Con la posición, podemos leer el archivo `index.dat` y guardar todos los índices y registros a los cuales apuntan en vectores hasta esa posición, saltearnos el índice y registro a eliminar (hacemos esto moviendo el puntero en el archivo leyendo un índice más sin utilizarlo), y guardar los siguientes a él.

Ya que leímos los registros a través del `index file`, hemos insertado los registros en el vector **ordenadamente**, lo cual nos sirve bastante, ya que ahora podemos recrear las posiciones de los `indexRecord.pos` (Esto era muy complicado de otra forma ya que: como el data file estaba desordenado, no teníamos forma de saber las nuevas posiciones de los registros después de eliminar uno en medio). Entonces, iteramos sobre el vector de data a reescribir `vector<Record> dataRewrite` y construimos los nuevos índices a partir del campo `get_key()` de los records y la posición a la que se están escribiendo en el `data file` mediante `dataOut.tellp()`. De esta manera, mientras que escribimos los records, creamos sus respectivos índices y reescribimos completamente los archivos.
```cpp
bool ISAM::remove(int id) {
    ifstream dataIn(dataFile, ios::binary);
    fstream index(indexFile, ios::in | ios::out | ios::binary);
    if(!dataIn || !index) return false;
    IndexRecord indexRecord;
    Record dataRecord;
    vector<IndexRecord> indexRewrite;
    vector<Record> dataRewrite;
    //vector<long> newPositions;
    unsigned long offset = 0;

    if(!search(id).has_value()) return false;
    auto pos = find(id, index);
    index.seekg(0);
    // save data before removed record
    while(index.tellg() < pos) {
        readIndex(indexRecord, index);
        indexRewrite.push_back(indexRecord);
        dataIn.seekg(indexRecord.pos);
        readRecord(dataRecord, dataIn);
        dataRewrite.push_back(dataRecord);
    }

    readIndex(indexRecord, index);

    // save data after removed record
    while(readIndex(indexRecord, index)) {
        dataIn.seekg(indexRecord.pos);
        readRecord(dataRecord, dataIn);
        dataRewrite.push_back(dataRecord);
        indexRewrite.push_back(indexRecord);
    }
    dataIn.close();
    index.close();
    ofstream dataOut(dataFile, ios::trunc | ios::binary);
    ofstream indexOut(indexFile, ios::trunc | ios::binary);
    for(auto & d : dataRewrite) {
        indexRecord.id = d.get_key();
        indexRecord.pos = dataOut.tellp();
        writeRecord(d, dataOut, offset);
        indexOut.write((char*)&indexRecord, sizeof(indexRecord));
    }
    dataOut.close();
    indexOut.close();
    return true;
}
```

## Resultados


## Prueba de uso
Se adjunta el siguiente video que muestra la funcionalidad de la aplicación.

[DB2 - Proyecto 1 - Sequential File & ISAM Structures](https://www.youtube.com/watch?v=bifs8Qo_C_Y)

## Anexos
- Descripcion de los campos en los registros del conjunto de datos

Atributo | Definicion
------------ | -------------
id | Identificador unico
gender | Género del paciente
age | Edad del paciente
hypertension | Si el paciente tiene hipertensión
heart_disease | Si el paciente tiene alguna enfermedad cardica
ever_married | Si el paciente es casado
work_type | A que se dedica el paciente
residence_type | Tipo de residencia
avg_glucose_level | Nivel de glucosa en la sangre
bmi | Indice de masa corporal
smoking_status | Estado de tabaquismo
stroke | Si el paciente tuvo un accidente cerebrovascular
