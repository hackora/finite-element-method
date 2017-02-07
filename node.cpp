#include "node.h"

Node::Node()
{

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
