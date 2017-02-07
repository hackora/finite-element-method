#ifndef NODE_H
#define NODE_H

#include <gmTrianglesystemModule>

class Node
{
public:
    Node();
    GMlib::TSVertex<float>*                      _vt;
    GMlib::Array<GMlib::TSTriangle<float>*>      getTriangles();
    bool                                         isThis(GMlib::TSVertex<float>*);
    GMlib::TSEdge<float>*                        neighbor(Node &n);

};

#endif // NODE_H
