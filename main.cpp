#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Base.h"
#include "RiskMatrix.h"
#include "Scenery.h"

// Variables to store environment status
static std::vector<Scenery*> scenarios;
static std::list<Base*> bases;

using namespace std;

int menu(string section) {
    int selectedOption = -1;

    static map<string, vector<string>> sections {
        {"Main", 
            {"Definir bases", "Definir escenario", "Correr escenario"},
        },
        {"Bases Define",
            {"Nueva base", "Eliminar base"}
        },
        {"Scenery Define",
            {"Nuevo escenario", "Eliminar escenario"}
        },
        {"Scenery Run",
            {"Seleccionar escenario"}
        },
        {"Define Matrix",
            {"Definir nueva seccion"}
        }
    };

    vector<string> option = sections[section];

    cout << "\n\n\n" << section <<  " menu: " << endl;
    int i;
    for (i = 0; i < option.size(); ++i) {
        cout << i+1 << " - " << option[i] << endl;
    }
    cout << i+1 << " - Salir" << endl;

    cout << "Seleccione una opcion: ";
    cin >> selectedOption;

    if (selectedOption > 0 and selectedOption <= option.size() + 1) {
        return --selectedOption; // Normalize back to 0-index
    } else if (selectedOption > option.size()) {
        return -1;
    } else {
        cout << "Opcion invalida. Intente de nuevo" << endl;
        return menu(section);
    }
}

void basesDefineMenuExecuter(int option) {
    switch (option) {
        case 0: {
            Base* fresh = new Base();

            string strInput;
            float numInput;
            Location loc;

            cout << "ID de la base: ";
            cin >> strInput;
            fresh->setId(strInput);
            cout << "Nombre de la base: ";
            cin >> strInput;
            fresh->setName(strInput);
            cout << "Coordenada X de la base: ";
            cin >> loc.x;
            cout << "Coordenada Y de la base: ";
            cin >> loc.y;
            fresh->setLocation(loc.x, loc.y);
            cout << "Riesgo de la base: ";
            cin >> numInput;
            fresh->setRisk(numInput);

            bases.push_back(fresh);

            // Missing safe checking the inputs
            return;
        }
        case 1: {
            string id;
            for(auto b : bases) {
                cout << b->getId() << " : " << b->getName() << endl;
            }

            cout << "\nID de la base a borrar: ";
            cin >> id;

            auto it = std::find_if(bases.begin(), bases.end(), [&](Base* b) {
                return b->getId() == id;
            });
            
            if (it != bases.end()) {
                delete *it;
                bases.erase(it);
                cout << "Base " << id << " eliminada." << endl;

                return;
            } else {
                cout << "Base inexistente. Intente de nuevo" << endl;
                basesDefineMenuExecuter(option);
                return;
            }
            break;
        }
        default:
            cout << "Menu principal fallo. Intente de nuevo.\n\n\n" << endl;
            return;            
    }
}

RiskMatrix defineRiskMatrix() {
    RiskMatrix mat = RiskMatrix();
    Location topLeft;
    Location bottomRight;
    int risk;

    cout << "\n\nRiesgo base en 0" << endl;
    cout << "Defina cuadrados usando (x,y) de la esquina superior izquierda e inferior derecha" << endl;

    int option;
    while ((option = menu("Define Matrix")) != -1) {
        cout << "Riesgo de la secccion: ";
        cin >> risk;
        cout << "Formato 'x y' sin comas ni parentesis";
        cout << "Coordenadas superior izquierda: ";
        cin >> topLeft.x >> topLeft.y;
        cout << "Coordenadas inferior derecha: ";
        cin >> bottomRight.x >> bottomRight.y;

        for (int x = topLeft.x; x <= bottomRight.x; x++) {
            for(int y = topLeft.y; y <= bottomRight.y; y++) {
                // Modify for propper encapsulation
                mat.dangerZone[x][y] = risk;
            } 
        }

        cout << "Seccion completada." << endl;
    }

    return mat;
}

void sceneryDefineMenuExecuter(int option) {
    if (bases.empty()) {
        cout << "No hay bases registradas para operar escenarios. Registralas." << endl;;
        return;
    }

    switch (option) {
        case 0: {
            string description;
            cout << "Descripcion del escenario: ";
            cin >> description;

            RiskMatrix mat = defineRiskMatrix();

            scenarios.push_back(new Scenery(description, mat, bases));
            
            return;
        }
        case 1: {
            int num;

            for (int i =0; i < scenarios.size(); i++) {
                cout << i+1 << ". " << scenarios[i]->getDescription() << endl;
            }

            cout << "Escriba el numero del escenario que desea borrar: ";
            cin >> num;

            if (num < scenarios.size() and num >= 0) {
                for(int i = num; i < scenarios.size(); i++) {
                    scenarios[i] = scenarios[i+1];
                }

                scenarios.shrink_to_fit();

                return;
            } else {
                cout << "Escenario inexistente. Intente de nuevo" << endl;
                sceneryDefineMenuExecuter(option);
                return;
            }
        }
        default:
            cout << "Menu principal fallo. Intente de nuevo." << endl;
            return;
    }
}

void sceneryRunMenuExecuter() {
    int i;
    cout << "Escenarios disponibles" << endl;
    for (i = 0; i < scenarios.size(); i++) {
        cout << i+1 << " - " << scenarios[i]->getDescription() << endl;
        ++i;
    }
    cout << "Seleccione el escenario: ";
    cin >> i;

    // scenarios[i]->run();
}

void mainMenuExecuter(int option) {
    int internalOption;
    switch (option) {
        case 0:
            while ((internalOption = menu("Bases Define")) != -1) {
                basesDefineMenuExecuter(internalOption);
            }
            return;
        case 1:
            sceneryDefineMenuExecuter(menu("Scenery Define"));
            return;
        case 2:
            sceneryRunMenuExecuter();
            return;
        default:
            cout << "Main menu failed. Try again.\n\n\n" << endl;
            return;
    }
}

int main() {
    int option;

    while ((option = menu("Main")) != -1) {
        mainMenuExecuter(option);
    }

    // "Destructor" for the environment variables
    for (auto s : scenarios) delete s;
    for (auto b : bases) delete b;

    cout << "Saliendo del sistema..." << endl;
}