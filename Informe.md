# Base de Datos 2 - Proyecto 1
### Integrantes
- Anthony Guimarey Saavedra
- Massimo Imparato Conetta
- Sebastian Knell Noriega

## Índice
- Introducción
- Implementación
    - Sequential File
    - Indexed Sequential Access Method (ISAM)
- Resultados
- Prueba de uso

## Introducción

El objetivo de este proyecto fue implementar dos estructuras de organización de archivos en memoria secundaria utilizando las técnicas vistas en
el curso y comparar su rendimiento. Se pueden realizar las siguientes cuatro operaciones: búsqueda puntual, búsqueda por rango, inserción y eliminación. 
Concretamente decidimos implementar un Sequential File y un Indexed Sequential Access Method (ISAM). Se uso un conjunto de datos de prueba obtenido de [Kaggle](https://www.kaggle.com/datasets). 
Este conjunto de datos se utiliza para predecir si es probable que un paciente sufra un accidente cerebrovascular, teniendo campos como el sexo, la edad, diversas enfermedades y el tabaquismo. 
De acuerdo a lo aprendido, se espera observar un mejor desempeño en las inserciones y búsquedas en el ISAM.

## Implementación
Para poder implementar estas técnicas de organización de archivos ...

- Búsqueda puntual

- Búsqueda por rango

- Inserción

- Eliminación


## Resultados


## Prueba de uso
Se adjunta el siguiente video que muestra la funcionalidad de la aplicacion.


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
