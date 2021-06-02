//
// Created by Sebastian Knell on 1/06/21.
//

#include "Menu.h"

void Menu::execute() {
    select_working_file();
    select_option();
}

void Menu::select_option() {
    string input;
    int option;
    do {
        cout << "Seleccione una opción. Ingrese QUIT para terminar" << endl;
        cout << "1. Imprimir todos los registros" << endl;
        cout << "2. Buscar un registro" << endl;
        cout << "3. Búsqueda por rango" << endl;
        cout << "4. Insertar un registro" << endl;
        cout << "5. Remover un registro" << endl;
        cin >> input;
        if (input == "QUIT") return;
        option = stoi(input);
        if (option < 1 || option > 5) cout << "Opción inválida" << endl;
        switch (option) {
            case 1:
                if (workingClass == sequential)
                    sequentialFile.print_all();
                else if(workingClass == isam)
                    isamFile.printAll();
                break;
            case 2:
                search();
                break;
            case 3:
                range_search();
                break;
            case 4:
                insert();
                break;
            case 5:
                remove();
                break;
            default:
                cout << "Ocurrió un error" << endl;
        }
    } while (input != "QUIT");
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
        else cout << "No se encontro ningun registro con el ID: " << id << endl;
    }
    else if(workingClass == isam) {
        auto dataRecord = isamFile.search(id);
        if(dataRecord.has_value()) dataRecord->print();
        else cout << "No se encontro ningun registro con el ID: " << id << endl;
    }
}

void Menu::range_search() {
    int id1;
    int id2;
    cout << "Ingrese un rango de ID's en el que desea buscar" << endl;
    cout << "Inicio: "; cin >> id1;
    cout << "Fin: "; cin >> id2;
    if (workingClass == sequential) {
        auto records = sequentialFile.range_search(id1, id2);
        if (!records.empty()) {
            for (auto record : records) print_record(record);
        }
        else cout << "No se encontró ningún registro en el rango dado" << endl;
    }
    else if(workingClass == isam) {
        auto records = isamFile.rangeSearch(id1, id2);
        if(!records.empty()) {
            for(auto record: records) record.print();
        }
        else cout << "No se encontró ningún registro en el rango dado" << endl;
    }
}

void Menu::insert() {
    int input;
    do {
        cout << "Ingrese un ID para el registro a instertar: ";
        cin >> input;
        if(input <= 0) cout << "ID ingresado no es valido";
    } while(input <= 0);
    vector<string> data;
    data.emplace_back(to_string(input));
    data.emplace_back("Male");
    data.emplace_back("25");
    data.emplace_back("1");
    data.emplace_back("0");
    data.emplace_back("Yes");
    data.emplace_back("Private");
    data.emplace_back("Urban");
    data.emplace_back("100.01");
    data.emplace_back("30.8");
    data.emplace_back("smokes");
    data.emplace_back("0");
    if(workingClass == sequential) {
        FixedRecord fixedRecord;
        fixedRecord.load_data(data);
        if (sequentialFile.insert(fixedRecord))
            cout << "Registro insertado correctamente" << endl;
        else cout << "No se pudo insertar registro" << endl;
    }
    else if (workingClass == isam) {
        Record record;
        record.load_data(data);
        isamFile.insert(record);
        cout << "Registro insertado correctamente" << endl;
    }
}

void Menu::remove() {
    int id;
    cout << "Ingrese el ID que desea remover: ";
    cin >> id;
    if (workingClass == sequential) {
        bool success = sequentialFile.remove(id);
        if (success) cout << "Registro eliminado exitosamente" << endl;
        else cout << "No se encontro ningun registro con el ID: " << id << endl;
    }
    else if(workingClass == isam) {
        bool success = isamFile.remove(id);
        if(success) cout << "Registro eliminado exitosamente" << endl;
        else cout << "No se encontro ningun registro con el ID: " << id <<endl;
    }
}
