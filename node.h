#ifndef NODE_H
#define NODE_H

#include <gmTrianglesystemModule>

class Node
{
public:
    Node();
    GMlib::TSVertex<float>                      *_vt;
    GMlib::Array<GMlib::TSTriangle<float>*>      getTriangles();

};

#endif // NODE_H
