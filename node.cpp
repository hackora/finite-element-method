#include "node.h"

Node::Node()
{

}

GMlib::Array <GMlib::TSTriangle<float>*> Node::getTriangles(){
    return _vt->getTriangles();;
}


