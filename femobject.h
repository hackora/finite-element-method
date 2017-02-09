#ifndef FEMOBJECT_H
#define FEMOBJECT_H

#include <gmTrianglesystemModule>
#include "node.h"

class FEMObject : public GMlib::TriangleFacets<float>
{
public:
    FEMObject(): GMlib::TriangleFacets<float>(){}

    ArrayLX<Node>                                     _nodes;
    GMlib::DMatrix<float>                              _A;
    GMlib::DVector<float>                              _b;

    void regularTriangulation(int,int,float);
    void randomTriangulation(int,float);
    void updateHeight(float);
    void computation();

    //compute A: 2 cases
    GMlib::Vector<GMlib::Vector<float,2>,3>           vectorsArray(GMlib::TSEdge<float> *edg);
    GMlib::Vector<GMlib::Vector<float,2>,3>           vectorsArray(GMlib::TSTriangle<float> *tr, Node *node);

    void localSimulate(double dt) override {
        for (int i=0;i<100;i++){
           updateHeight(i);
        }
    }

};

#endif // FEMOBJECT_H
