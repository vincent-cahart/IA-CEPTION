#include <iostream>

#include "escapi.h"
#include "DataStructures.hpp"
#include "GraphExplorer.hpp"

bool InitCapture(struct SimpleCapParams& capture) {
    int devices = setupESCAPI(); // init lib
    if (devices == 0) {
        std::cerr << "No webcam found" << std::endl;
        return false;
    }
    if (initCapture(0, &capture) == 0) {
        std::cerr << "Can't init Capture" << std::endl;
        return false;
    }
    for (int i(0); i < 5; i++) // do 5 loops for do light calib
    {
        doCapture(0);
        while (!isCaptureDone(0)) {} //Doing Capture   /!\ if webcam disconnect during this, INFINITE LOOP
    }
    return true;
}

//Create a classic graph
void InitGraph(Graph& graph, const struct SimpleCapParams& capture) {

    //Add nodes
    for (int i(0); i < capture.mHeight; i++) {
        for (int j(0); j < capture.mWidth; j++) {
            graph.addNode(unsigned(i * capture.mWidth + j), ConvertRGBBinToMono(capture.mTargetBuf[i * capture.mWidth + j]) );
        }
    }

    //Add edges all connected
    for (unsigned i = 0; i < graph.getNodes().size(); i++) {
        unsigned x = (i % unsigned(capture.mWidth));
        unsigned y = (i / unsigned(capture.mWidth));

        //!\\ NE PAS OULIER QUE C'EST UN GRAPH NON ORIENTE DONC TOUT LES VOISINS AUTOUR SONT PRIT
        if (y > 0) {
            graph.addEdge(i, x + (y - 1) * unsigned(capture.mWidth)); //node du bas
            if (x > 0)
                graph.addEdge(i, (x - 1) + (y - 1) * unsigned(capture.mWidth)); //node du bas droite
            if (x < unsigned(capture.mWidth))
                graph.addEdge(i, (x + 1) + (y - 1) * unsigned(capture.mWidth)); //node du bas gauche
        }
        if (x > 0)
            graph.addEdge(i, (x - 1) + (y)* unsigned(capture.mWidth)); //node du milieu droit
    }
}

std::vector<Graph> SubdivideGraph(Graph& graph) {
    std::vector<Graph> SubdividedGraphList;
    for (unsigned j = 0; j < 4; j++) {
        for (unsigned i = 0; i < 4; i++) {

            Graph OneGraph;
            for (unsigned gj = 0; gj < 25; gj++) {
                for (unsigned gi = 0; gi < 25; gi++) {
                    //i * element + gi;
                    unsigned x = i * 25 + gi;
                    //j * element * tabs * ligne + gj * element * tabs;
                    unsigned y = j * 25 * 4 * 25 + gj * 25 * 4;

                    //std::cout << (x+y) << std::endl;
                    OneGraph.addNode(gj*25 + gi, graph.getNodes()[(x+y)]->getMono() );
                }
            }

            //Add edges like a suite
            std::vector<Node*> allNodes = OneGraph.getNodes();
            for (Node* node : allNodes) {
                unsigned x = (node->getUId() % 25);
                unsigned y = (node->getUId() / 25);

                if ((x + 1) + y * 25 < allNodes[allNodes.size() - 1]->getUId()) {
                    OneGraph.addEdge(x + y * 25, (x + 1) + y * 25);
                }
            }

            SubdividedGraphList.push_back(OneGraph);
        }
    }
    return SubdividedGraphList;
}

int main()
{
    struct SimpleCapParams capture;
    capture.mWidth = 100;
    capture.mHeight = 100;
    capture.mTargetBuf = new int[ capture.mWidth * capture.mHeight];

    if ( !InitCapture(capture) ) { return -1; }

    Graph graph;

    doCapture(0);

    InitGraph(graph, capture);
    //InitGraphBarcode(graph, capture); // init nodes and edges

    deinitCapture(0);

    unsigned barcodeCount = 0;

    std::vector<Graph> SubdividedGraphList = SubdivideGraph(graph);
    for (Graph subdiveGraph : SubdividedGraphList) {

        subdiveGraph.drawImage(25);

        bool haveBarcode = HaveBarcode(subdiveGraph, 25, 25);
        if (haveBarcode) barcodeCount++;

        std::cout << (haveBarcode ? "BARCODE" : "NOT A BARCODE") << std::endl;
    }

    //graph.printGraph();
    graph.drawImage(capture.mWidth);
    std::cout << "Nombre de codebare trouver dans l'image (peut contenir plusieur fois le meme codebar): " << barcodeCount << std::endl;

    return 0;
}



/* OLD

//Create a graph special for barcode
void InitGraphBarcode(Graph& graph, const struct SimpleCapParams& capture) {

    //Add nodes
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < capture.mWidth; j++) {
            graph.addNode(i * capture.mWidth + j, ConvertRGBBinToMono(capture.mTargetBuf[ (i + int(capture.mWidth*0.33)) * capture.mWidth + j]) );
        }
    }

    //Add edges like a suite
    std::vector<Node*> allNodes = graph.getNodes();
    for (Node* node : allNodes) {
        unsigned x = (node->getUId() % capture.mWidth);
        unsigned y = (node->getUId() / capture.mWidth);

        if ((x + 1) + y * capture.mWidth < allNodes[allNodes.size() - 1]->getUId()) {
            graph.addEdge(x + y * capture.mWidth, (x + 1) + y * capture.mWidth);
        }
    }
}

*/
