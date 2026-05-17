#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Base.h"
#include "Link.h"
#include "RiskMatrix.h"
#include "Scenery.h"

// Variables to store environment status
static std::vector<Scenery*> scenarios;
static std::list<Base*> bases;
static std::list<Link*> links;

using namespace std;

int menu(string section) {
    int selectedOption = -1;

    static map<string, vector<string>> sections {
        {"Main", 
            {"Definir bases", "Definir escenario", "Correr escenario"},
        },
        {"Bases Define",
            {"Nueva base", "Nueva conexion", "Eliminar base"}
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

    cout << "\n\n" << section <<  " menu: " << endl;
    int i;
    for (i = 0; i < option.size(); ++i) {
        cout << i+1 << " - " << option[i] << endl;
    }
    cout << i+1 << " - Salir" << endl;

    cout << "Seleccione una opcion: ";
    cin >> selectedOption;

    int maxOption = (int)option.size() + 1;
    if (selectedOption >= 1 && selectedOption <= (int)option.size()) {
        return selectedOption - 1; // Normalize back to 0-index for valid choices
    } else if (selectedOption == maxOption) {
        return -1; // Exit selection
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
            string from, to;
            cout << "Bases disponibles" << endl;
            for(auto b : bases) {
                cout << b->getId() << " : " << b->getName() << endl;
            }
            cout << "ID de la base origen: " << endl;
            cin >> from;
            cout << "ID de la base destino: " << endl;
            cin >> to;
            
            auto itFrom = find_if(bases.begin(), bases.end(), [&](Base* b) {
                    return b->getId() == from;
                });
            auto itTo = find_if(bases.begin(), bases.end(), [&](Base* b) {
                    return b->getId() == to;
                });

            if (itFrom != bases.end() && itTo != bases.end()) {
                links.push_back(new Link(*itFrom, *itTo));
                return;
            }

            cout << "Alguna de las bases no existe. No se creo la conexion." << endl;
            
        }
        case 2: {
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
        cout << "Formato 'x y' sin comas ni parentesis" << endl;
        cout << "Coordenadas superior izquierda: ";
        cin >> topLeft.x >> topLeft.y;
        cout << "Coordenadas inferior derecha: ";
        cin >> bottomRight.x >> bottomRight.y;

        for (int x = topLeft.x; x <= bottomRight.x; x++) {
            for (int y = topLeft.y; y <= bottomRight.y; y++) {
                // Clamp indices to [0,9] to avoid out-of-bounds
                int cx = x;
                int cy = y;
                if (cx < 0) cx = 0;
                if (cy < 0) cy = 0;
                if (cx > 9) cx = 9;
                if (cy > 9) cy = 9;
                mat.dangerZone[cx][cy] = risk;
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

            scenarios.push_back(new Scenery(description, mat, bases, links));
            
            return;
        }
        case 1: {
            int num;

            for (int i = 0; i < scenarios.size(); i++) {
                cout << i+1 << ". " << scenarios[i]->getDescription() << endl;
            }

            cout << "Escriba el numero del escenario que desea borrar: ";
            cin >> num;

            // Convert to 0-based index
            int idx = num - 1;

            if (idx >= 0 && idx < (int)scenarios.size()) {
                delete scenarios[idx];
                scenarios.erase(scenarios.begin() + idx);
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