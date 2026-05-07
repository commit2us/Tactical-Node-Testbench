#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cctype>

#include "Scenery.h"

using namespace std;

int menu(string section) {
    int selectedOption = -1;

    vector<string> option = map<string, vector<string>> {
        {"Main", 
            {"Definir bases", "Definir escenario", "Correr escenario", "Salir"},
        },
        {}
    }[section];

    cout << "\n\n\n" << section <<  " menu: " << endl;
    for (int i = 0; i < option.size(); i++) {
        cout << i << " - " << option[i] << endl;
    }

    cout << "Seleccione una opcion: " << endl;
    cin >> selectedOption;

    if (selectedOption > 0 and selectedOption <= option.size() + 1) {
        return selectedOption;
    } else if (selectedOption == option.size()) {
        return -1;
    } else {
        cout << "Opcion invalida. Intente de nuevo" << endl;
        return menu(section);
    }
}

int main() {
    int option = 1;
    while ((option = menu("Main")) > 0) {
        cout << option;
    }
}