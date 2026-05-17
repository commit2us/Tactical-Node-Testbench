#include "Scenery.h"
#include "Network.h"
#include <iostream>

Scenery::Scenery(string description, RiskMatrix riskMatrix,const list<Base*>& bases, const list<Link*>& links):
    description(description), riskMatrix(riskMatrix), bases(bases), links(links) {
    determineRisk();
}

Scenery::~Scenery() {
    delete graph;
}

void Scenery::determineRisk() {
    for(auto &b : bases) {
        b->setRisk(riskMatrix.getRiskAt(b->getLocation().x, b->getLocation().y));
    }
}

string Scenery::getDescription() {
    return description;
}

Scenery* Scenery::createTutorialLevel(const list<Base*>& bases, const list<Link*>& links) {
    RiskMatrix rm; // Todo en 0 por defecto
    return new Scenery("Level 1: Tutorial Mode (Safe Board with No Mines)", rm, bases, links);
}

Scenery* Scenery::createMinefieldLevel(const list<Base*>& bases, const list<Link*>& links) {
    RiskMatrix rm;
    rm.setRiskAt(2, 3, 9.0f);
    rm.setRiskAt(4, 5, 9.0f);
    rm.setRiskAt(7, 2, 9.0f);
    return new Scenery("Level 2: Minefield Match (Evasion of high danger cells)", rm, bases, links);
}

Scenery* Scenery::createBlockadeLevel(const list<Base*>& bases, const list<Link*>& links) {
    RiskMatrix rm;
    // Muro en la columna 5
    for(int row = 0; row < 10; row++) {
        rm.setRiskAt(row, 5, 10.0f);
    }
    return new Scenery("Level 3: Blockade Course (Forcing peripheral rerouting around the edges)", rm, bases, links);
}

void Scenery::displayBoard() {
    cout << "\n=============================================" << endl;
    cout << "  TABLERO: " << description << endl;
    cout << "=============================================" << endl;
    
    cout << "    ";
    for(int j = 0; j < 10; j++) cout << j << "  ";
    cout << "\n    ------------------------------\n";

    for (int i = 0; i < 10; i++) {
        cout << i << " | ";
        for (int j = 0; j < 10; j++) {
            float risk = riskMatrix.getRiskAt((float)i, (float)j);
            
            // Verificamos si el USUARIO puso una base en esta coordenada (i,j)
            bool hasBase = false;
            for (auto b : bases) {
                if (b->getLocation().x == i && b->getLocation().y == j) {
                    hasBase = true;
                    break;
                }
            }

            // Jerarquía visual: Peligro primero. Si no hay peligro pero hay base, imprime B.
            if (risk >= 10.0f) {
                cout << "X  ";   // X = Bloqueo total
            } 
            else if (risk >= 5.0f) {
                cout << "M  ";   // M = Mina
            } 
            else if (hasBase) {
                cout << "B  ";   // B = Base del usuario
            }
            else {
                cout << ".  ";   // . = Terreno vacío
            }
        }
        cout << "|\n";
    }
    cout << "    ------------------------------\n";
}

void Scenery::run(Base* from, Base* to) {
    // 1. Asignamos los riesgos a las bases reales del usuario
    determineRisk();
    
    // 2. Dibujamos cómo quedó el mapa
    displayBoard();

    // 3. Pasamos las bases y links al motor
    vector<Link*> vecLinks;
    vecLinks.assign(links.begin(), links.end());
    vector<Base*> vecBases;
    vecBases.assign(bases.begin(), bases.end());

    graph = new Network(vecBases, vecLinks);
    
    float riesgoTotalDelViaje = 0.0f;
    vector<Base*> camino = graph->findRoute(from, to, riesgoTotalDelViaje);

    cout << "\n========================================" << endl;
    cout << "  RESULTADO DE LA NAVEGACIÓN" << endl;
    cout << "========================================" << endl;

    if (camino.empty()) {
        cout << "¡ALERTA! No existe ningún camino posible entre la base " 
             << from->getId() << " y la base " << to->getId() << "." << endl;
    } 
    else {
        cout << "Ruta óptima encontrada: ";
        for (size_t i = 0; i < camino.size(); i++) {
            cout << camino[i]->getId();
            if (i < camino.size() - 1) cout << " -> ";
        }
        cout << "\n\nTotal de escalas: " << camino.size() - 1 << endl;
        cout << "Riesgo total del viaje: " << riesgoTotalDelViaje << endl; 
    }
    cout << "=============================================\n" << endl;
}