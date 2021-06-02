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
### Sequential File
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
### ISAM


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
