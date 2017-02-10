#ifndef FEMOBJECT_H
#define FEMOBJECT_H

#include <gmTrianglesystemModule>
#include "node.h"
#include <QDebug>

class FEMObject : public GMlib::TriangleFacets<float>
{
public:
    FEMObject(): GMlib::TriangleFacets<float>(){ _force=0; goingUp=1;}

    ArrayLX<Node>                                      _nodes;
    GMlib::DMatrix<float>                              _A;
    GMlib::DVector<float>                              _b;
    double                                             _maxForce;
    double                                             _force;
    int                                                goingUp;

    void regularTriangulation(int,int,float);
    void randomTriangulation(int,float);
    void updateHeight(float);
    void computation();
    void setMaxForce(double);

    //compute A: 2 cases
    GMlib::Vector<GMlib::Vector<float,2>,3>           vectorsArray(GMlib::TSEdge<float> *edg);
    GMlib::Vector<GMlib::Vector<float,2>,3>           vectorsArray(GMlib::TSTriangle<float> *tr, Node *node);

    void localSimulate(double dt) override;

};

#endif // FEMOBJECT_H
