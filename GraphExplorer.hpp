#pragma once
#include "DataStructures.hpp"

unsigned ParcourirGraph(std::map<Node*, bool>& visited, Node* node, unsigned valeur) {
    visited[node] = true;
    for (std::pair<Node*, int> pairNode : node->getNeighbors()) {
        if (!visited[pairNode.first]) {

            if (pairNode.second > 150 || pairNode.second < -150) valeur++;

            return ParcourirGraph(visited, pairNode.first, valeur);
        }
    }
    return valeur;
}

bool HaveBarcode(Graph& graph, const unsigned& mW, const unsigned& mH) {
    Graph BarcodeGraph;

    std::map<Node*, bool> visitedNodes;
    unsigned change = ParcourirGraph(visitedNodes, graph.getNodes()[0], 0);
    std::cout << "change: " << change << "size: " << graph.getNodes().size() << std::endl;

    //on regarde si le nombre de changement brutaux est équivalent a 15% des nodes du graph alors on considere que c'est un code barre
    return (change > graph.getNodes().size()*0.15) ? true : false;
}





/*
//Create graph with draw Line
for (unsigned i = 0; i < 4; i = i + 1) {
    for (unsigned x = 0; x < mW; x = x + 1) {
        BarcodeGraph.addNode(x + i * mW, graph.getNodes()[x + ((mH / 3) + i * 4) * mW]->getMono());
    }
}

for (Node* node : BarcodeGraph.getNodes()) {
    unsigned x = (node->getUId() % mW);
    unsigned y = (node->getUId() / mW);

    if ((x + 1) + y * mW < BarcodeGraph.getNodes()[BarcodeGraph.getNodes().size()-1]->getUId())
        BarcodeGraph.addEdge(x + y * mW, (x + 1) + y * mW);
}
BarcodeGraph.printGraph();
BarcodeGraph.drawImage(mW);
*/

//Process graph to find result
