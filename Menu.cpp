//
// Created by Sebastian Knell on 1/06/21.
//

#include "Menu.h"

void Menu::execute() {
    select_working_file();
    select_option();
}

void Menu::select_option() {
    cout << "Seleccione una opción. Ingrese QUIT para terminar" << endl;
    cout << "1. Buscar un registro" << endl;
    cout << "2. Búsqueda por rango" << endl;
    cout << "3. Insertar un registro" << endl;
    cout << "4. Remover un registro" << endl;
    string input;
    int option;
    do {
        cin >> input;
        if (input == "QUIT") return;
        option = stoi(input);
        if (option < 1 || option > 4) cout << "Opción inválida" << endl;
        switch (option) {
            case 1:
                search();
                break;
            case 2:
                range_search();
                break;
            case 3:
                insert();
                break;
            case 4:
                remove();
                break;
            default:
                cout << "Ocurrió un error" << endl;
        }
    } while (option < 1 || option > 4);
}

void Menu::select_working_file() {
    cout << "Seleccione un método de organización de archivos" << endl;
    cout << "1. Sequential File" << endl;
    cout << "2. ISAM (Indexed Sequential Access Method)" << endl;
    int option;
    do {
        cin >> option;
        if (option != 1 && option != 2) cout << "Opción inválida" << endl;
    } while (option != 1 && option != 2);
    workingClass = option == 1 ? sequential : isam;
    if (workingClass == sequential) sequentialFile.load_data("stroke-data.csv");
}

void Menu::search() {
    int id;
    cout << "Ingrese el ID que desea buscar: ";
    cin >> id;
    if (workingClass == sequential) {
        auto fixedRecord = sequentialFile.search(id);
        if (fixedRecord.has_value()) print_record(fixedRecord.value());
        else cout << "No se encontro ningun registro con el ID" << id << endl;
    }
}

void Menu::range_search() {
    int id1;
    int id2;
    cout << "Ingrese un rango de ID's en el que desea buscar";
    cout << "Inicio: "; cin >> id1;
    cout << "Fin: "; cin >> id2;
    if (workingClass == sequential) {
        auto records = sequentialFile.range_search(id1, id2);
        if (!records.empty()) {
            for (auto record : records) print_record(record);
        }
        else cout << "No se encontró ningún registro en el rango dado" << endl;
    }
}

void Menu::insert() {

}

void Menu::remove() {

}
