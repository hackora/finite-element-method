#ifndef FEMOBJECT_H
#define FEMOBJECT_H

#include <gmTrianglesystemModule>
#include "node.h"

class FEMObject : public GMlib::TriangleFacets<float>
{
public:
    FEMObject();

    ArrayLX<Node>           _nodes;
    GMlib::DMatrix<float>   _A;
    GMlib::DVector<float>   _b;

    void regularTriangulation(int,int,float);
    void randomTriangulation(int,float);
    void updateHeight(float);
    //combination method I'm not sure about the signature yet
};

#endif // FEMOBJECT_H
