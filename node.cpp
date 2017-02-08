#include "node.h"

Node::Node(){
    std::cout<< "constructor of nodes";
    auto centerPoint = GMlib::Point<float,2>(0,0);
    auto vt = GMlib::TSVertex<float>(centerPoint);
    _vt = &vt;
}

GMlib::Array <GMlib::TSTriangle<float>*> Node::getTriangles(){

    return _vt->getTriangles();;
}

bool Node::isThis(GMlib::TSVertex<float> * vt){

    if (vt == _vt)
        return true;
    return false;
}

GMlib::TSEdge<float>* Node::neighbor(Node &n){

    GMlib::Array<GMlib::TSEdge<float>*> edg = _vt->getEdges();
    for(int i=0;i<edg.size();i++){
        if (isThis(edg[i]->getOtherVertex(*_vt)))
            return edg[i];
    }
    return NULL;
}
