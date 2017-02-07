#include "femobject.h"
#include <gmCoreModule>

FEMObject::FEMObject()
{

}

void FEMObject::regularTriangulation(int n, int m, float r){

    auto centerPoint = GMlib::Point<float,2>(0,0);
    this->insertAlways(centerPoint);
    for(int i=0;i<m;i++){
        for(int j=0;j<n*(i+1);j++){
            GMlib::Angle a = (j * M_2PI)/(n*(i+1));
            GMlib::SqMatrix<float,2> mt(a,GMlib::Vector<float,2>(1,0),GMlib::Vector<float,2>(0,1));
            GMlib::Point<float,2> pt = mt * GMlib::Point<float,2>((i+1)/m*r,0);
            this->insertAlways(pt);
        }
    }
}


void FEMObject::randomTriangulation(int n, float r){

    GMlib::Random<int> rand;
    auto rn = 4;
    auto m = n/rn;
    regularTriangulation(rn,m,r);
    auto nm = std::max(M_PI / (std::sqrt(3*sin(M_PI/n)*sin(M_PI/n)+2-n)*0.5/n), (1.1));
    auto num = 1 + n *nm;
    int t = num * 0.8;
    for (int i=0;i<t;i++){
        std::swap(this[rand.get()],this[rand.get()]);
    }
    //TODO: Remove all elements after index num that are not on the boundary


}

GMlib::Vector<GMlib::Vector<float,2>,3>           vectorsArray(GMlib::TSEdge<float> *edg){

    GMlib::Array<GMlib::TSTriangle<float>*> tr = edg->getTriangle();
    GMlib::Array<GMlib::TSVertex<float>*>   v1 = tr[0]->getVertices();
    GMlib::Array<GMlib::TSVertex<float>*>   v2 = tr[1]->getVertices();
    GMlib::Vector<GMlib::Vector<float,2>,3> d; //output
    GMlib::Point<float,2> p0,p1,p2,p3;
    p0 = edg->getFirstVertex()->getParameter();
    p1 = edg->getFirstVertex()->getParameter();
    //p2
    for(int i=0;i<3;i++){
        if(v1[i]!=edg->getFirstVertex() && v1[i]!=edg->getLastVertex())
            p2 = v1[i]->getParameter();
    }
    //p3
    for(int i=0;i<3;i++){
        if(v2[i]!=edg->getFirstVertex() && v2[i]!=edg->getLastVertex())
            p2 = v2[i]->getParameter();
    }

    d[0] = p1 - p0;
    d[1] = p2 - p0;
    d[2] = p3 - p0;

    return d;

}

GMlib::Vector<GMlib::Vector<float,2>,3>           vectorsArray(GMlib::TSTriangle<float> *tr, Node *node){

    GMlib::Point<float,2> p0,p1,p2;
    GMlib::Vector<GMlib::Vector<float,2>,3> d; //output
    GMlib::Array<GMlib::TSVertex<float>*>   v = tr->getVertices();
    if (node->isThis(v[1]))
        std::swap(v[0],v[1]);
    if (node->isThis(v[2]))
        std::swap(v[0],v[2]);

    p0 = v[0]->getParameter();
    p1 = v[1]->getParameter();
    p2 = v[2]->getParameter();

    d[0] = p1 - p0;
    d[1] = p2 - p0;
    d[2] = p2 - p1;

    return d;

}
