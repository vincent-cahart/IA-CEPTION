#ifndef DATASTRUCTURES_HPP
#define DATASTRUCTURES_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

unsigned ConvertRGBBinToMono(const unsigned& PixByte) {
    unsigned R, G, B;
    //DECODING RGB24
    // R=bits 16-24
    R = ((PixByte >> 16) & 255);
    // G=bits 8-16
    G = ((PixByte >> 8) & 255);
    // B=bits 0-8
    B = (PixByte & 255);

    //coeff based on what is most perceived as brightness by humans as color in one channel
    // http://poynton.ca/notes/colour_and_gamma/ColorFAQ.html#RTFToC9
    return unsigned((0.2125 * R) + (0.7154 * G) + (0.0721 * B));
}

//Edge count and nextNode

class Node {
private:
    unsigned uId;
    unsigned Mono;
    std::map<Node*, int> neighbors;
public:
    Node(const unsigned & uid, const unsigned & mono) : uId(uid), Mono(mono) {
 }

    void addNeighbor(Node* neighbor) {
        neighbors[neighbor] = int(this->getMono()) - int(neighbor->getMono());
    }

    std::map<Node*, int> getNeighbors() { return neighbors; }
    unsigned getMono() const  { return Mono; }
    unsigned getUId() const { return uId; }
};

class Graph {
private:
    std::vector<Node*> Nodes;
public:
    Graph() {}

    std::vector<Node*> getNodes() { return Nodes; }

    void addNode(const unsigned& nodeID, const unsigned& mono);
    void addEdge(const unsigned& firstNodeID, const unsigned& secondNodeID);

    void printGraph();
    void drawImage(const unsigned& mW);
};

void Graph::addNode(const unsigned & nodeID, const unsigned& mono) {
    Node* node = new Node(nodeID, mono);
    Nodes.push_back(node);
}

void Graph::addEdge(const unsigned& firstNodeID, const unsigned& secondNodeID) {
    Nodes[firstNodeID]->addNeighbor(Nodes[secondNodeID]);
    Nodes[secondNodeID]->addNeighbor(Nodes[firstNodeID]);
}

void Graph::printGraph() {
    for (unsigned i = 0; i < Nodes.size(); i++) {
        std::map<Node*, int> neighbors = Nodes[i]->getNeighbors();
        std::cout << "Node: " << Nodes[i]->getUId() << " connected with: \n";
        for (std::pair<Node*, int> neighbor : neighbors) {
            std::cout << "\tnode: " << neighbor.first->getUId() << " weight= " << neighbor.second << std::endl;
        }
        std::cout << "\n";
    }
}

void Graph::drawImage(const unsigned &mW) {
    //Test a Ascii art
    char light[] = " .,-o+O0@";
    //char light[] = "...-o+@@@";
    for (Node* node : Nodes) {
        std::cout << std::setw(2) << light[7 - ((node->getMono() >> 5) & 7)];
        if (( (node->getUId()+1) % mW) == 0) std::cout << std::endl;
    }
}

//Salut a tous c'est antoine on se retrouve pour une nouvelle vidéo sur slither.io
#endif // DATASTRUCTURES_HPP
