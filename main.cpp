#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()

#include "Base.h"
#include "Link.h"
#include "RiskMatrix.h"
#include "Scenery.h"

// Variables to store environment status
static std::vector<Scenery*> scenarios;
static std::list<Base*> bases;
static std::list<Link*> links;
static std::string lastSceneryRun = "Ninguno (Bases con riesgo por defecto)";

using namespace std;

void displayBases() {
    if (bases.empty()) {
        cout << "No hay bases registradas." << endl;
        return;
    }

    for (auto b : bases) {
        cout << b->getId() << " : " << b->getRisk() << endl;
    }
}

void displayStatus() {
    cout << "\n=============================================" << endl;
    cout << "  ESTADO DEL SISTEMA" << endl;
    cout << "  Escenario de los riesgos actuales: " << lastSceneryRun << endl;
    cout << "=============================================" << endl;
    
    cout << "\n--- BASES ---" << endl;
    if (bases.empty()) {
        cout << "No hay bases registradas." << endl;
    } else {
        for (auto b : bases) {
            cout << "ID: " << b->getId() 
                 << " | Nombre: " << b->getName() 
                 << " | Riesgo actual: " << b->getRisk() 
                 << " | Pos: (" << b->getLocation().x << "," << b->getLocation().y << ")" << endl;
        }
    }

    cout << "\n--- CONEXIONES ---" << endl;
    if (links.empty()) {
        cout << "No hay conexiones registradas." << endl;
    } else {
        for (auto l : links) {
            cout << l->nodes[0]->getId() << " -> " << l->nodes[1]->getId() << endl;
        }
    }
    cout << "=============================================\n" << endl;
}

int menu(string section) {
    int selectedOption = -1;

    static map<string, vector<string>> sections {
        {"Main", 
            {"Definir bases", "Definir escenario", "Correr escenario (ejemplos disponibles)", "Mostrar estado"},
        },
        {"Bases Define",
            {"Nueva base", "Nueva conexion", "Eliminar base", "Eliminar conexion"}
        },
        {"Scenery Define",
            {"Nuevo escenario", "Eliminar escenario"}
        },
        {"Scenery Run",
            {"Seleccionar escenario", "Ejemplos"} // <- NUEVA OPCIÓN AQUÍ
        },
        {"Define Matrix",
            {"Definir nueva seccion"}
        }
    };

    vector<string> option = sections[section];

    cout << "\n" << section <<  " menu: " << endl;
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
            Location loc; // Ya no necesitamos la variable numInput

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

            // Nota: Ya no pedimos el riesgo. La clase Base por defecto
            // le asigna 1.0f, y Scenery::determineRisk() lo cambiará 
            // al valor real justo antes de correr el algoritmo.

            bases.push_back(fresh);
            
            cout << "Base creada exitosamente." << endl;
            return;
        }
        case 1: {
            string from, to;
            cout << "Bases disponibles:" << endl;
            for(auto b : bases) {
                cout << b->getId() << " : " << b->getName() << endl;
            }
            cout << "ID de la base origen: ";
            cin >> from;
            cout << "ID de la base destino: ";
            cin >> to;
            
            auto itFrom = find_if(bases.begin(), bases.end(), [&](Base* b) {
                    return b->getId() == from;
                });
            auto itTo = find_if(bases.begin(), bases.end(), [&](Base* b) {
                    return b->getId() == to;
                });

            if (itFrom != bases.end() && itTo != bases.end()) {
                links.push_back(new Link(*itFrom, *itTo));
                cout << "Conexion creada exitosamente." << endl;
                return;
            }

            cout << "Alguna de las bases no existe. No se creo la conexion." << endl;
            return;
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
                Base* baseToDelete = *it;
                
                // SEGURIDAD: Borrar los links asociados a esta base antes de borrar la base
                // Si no hacemos esto, el programa crashearía al intentar mostrar los links después
                auto linkIt = links.begin();
                while (linkIt != links.end()) {
                    if ((*linkIt)->nodes[0] == baseToDelete || (*linkIt)->nodes[1] == baseToDelete) {
                        delete *linkIt;
                        linkIt = links.erase(linkIt);
                    } else {
                        ++linkIt;
                    }
                }

                // Ahora sí borramos la base
                delete baseToDelete;
                bases.erase(it);
                cout << "Base " << id << " y sus conexiones asociadas eliminadas." << endl;

                return;
            } else {
                cout << "Base inexistente. Intente de nuevo." << endl;
                return;
            }
        }
        case 3: {
            if (links.empty()) {
                cout << "No hay conexiones registradas para eliminar." << endl;
                return;
            }

            string from, to;
            cout << "Conexiones disponibles:" << endl;
            for(auto l : links) {
                cout << l->nodes[0]->getId() << " -> " << l->nodes[1]->getId() << endl;
            }

            cout << "\nID de la base origen de la conexion a borrar: ";
            cin >> from;
            cout << "ID de la base destino de la conexion a borrar: ";
            cin >> to;

            auto it = std::find_if(links.begin(), links.end(), [&](Link* l) {
                return l->nodes[0]->getId() == from && l->nodes[1]->getId() == to;
            });

            if (it != links.end()) {
                delete *it;
                links.erase(it);
                cout << "Conexion de " << from << " a " << to << " eliminada exitosamente." << endl;
            } else {
                cout << "La conexion especificada no existe." << endl;
            }
            return;
        }
        default:
            cout << "Menu de bases fallo. Intente de nuevo.\n" << endl;
            return;            
    }
}

RiskMatrix defineRiskMatrix() {
    RiskMatrix mat = RiskMatrix();
    Location topLeft;
    Location bottomRight;
    int risk;

    cout << "\nRiesgo base en 0" << endl;
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

void sceneryRunMenuExecuter(int option) {
    switch (option) {
        case 0: { // Seleccionar escenario
            if (scenarios.empty()) {
                cout << "No hay escenarios registrados. Registra uno o carga un ejemplo primero." << endl;
                return;
            }

            int i;
            cout << "Escenarios disponibles" << endl;
            for (i = 0; i < scenarios.size(); i++) {
                cout << i+1 << " - " << scenarios[i]->getDescription() << endl;
            }
            cout << "Seleccione el escenario: ";
            cin >> i;

            if (i < 1 || i > scenarios.size()) {
                cout << "El escenario no existe" << endl;
                return;
            }

            displayBases();

            string from, to;
            cout << "ID de la base de origen: ";
            cin >> from;
            cout << "ID de la base destino: ";
            cin >> to;

            auto itFrom = find_if(bases.begin(), bases.end(), [&](Base* b) {
                    return b->getId() == from;
                });
            auto itTo = find_if(bases.begin(), bases.end(), [&](Base* b) {
                    return b->getId() == to;
                });

            if (itFrom != bases.end() && itTo != bases.end()) {
                lastSceneryRun = scenarios[i - 1]->getDescription();
                scenarios[i - 1]->run(*itFrom, *itTo);
            }
            return;
        }
        case 1: { // Ejemplos aleatorios
            // Genera un número aleatorio entre 0 y 2
            int randomChoice = rand() % 3;
            Scenery* example = nullptr;

            // ¡Le pasamos las listas globales 'bases' y 'links' construidas por el usuario!
            if (randomChoice == 0) {
                example = Scenery::createTutorialLevel(bases, links);
            } else if (randomChoice == 1) {
                example = Scenery::createMinefieldLevel(bases, links);
            } else {
                example = Scenery::createBlockadeLevel(bases, links);
            }

            cout << "\n=============================================" << endl;
            cout << "  ESCENARIO DE EJEMPLO CARGADO AL AZAR" << endl;
            cout << "  Descripcion: " << example->getDescription() << endl;
            cout << "=============================================" << endl;

            scenarios.push_back(example);
            return;
        }
        default:
            cout << "Menu de ejecucion fallo. Intente de nuevo." << endl;
            return;
    }
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
            while ((internalOption = menu("Scenery Define")) != -1) {
                sceneryDefineMenuExecuter(internalOption);
            }
            return;
        case 2:
            while ((internalOption = menu("Scenery Run")) != -1) {
                sceneryRunMenuExecuter(internalOption);
            }
            return;
        case 3:
            displayStatus();
            return;
        default:
            cout << "Main menu failed. Try again.\n" << endl;
            return;
    }
}

int main() {
    // Inicializar la semilla para los escenarios aleatorios
    srand(time(nullptr));

    int option;

    while ((option = menu("Main")) != -1) {
        mainMenuExecuter(option);
    }

    // "Destructor" for the environment variables
    for (auto s : scenarios) delete s;
    for (auto b : bases) delete b;

    cout << "Saliendo del sistema..." << endl;
    return 0;
}