#include "femobject.h"
#include <gmCoreModule>
#include <QDebug>
//FEMObject::FEMObject(){
//}

void FEMObject::regularTriangulation(int n, int m, float r){

    auto centerPoint = GMlib::Point<float,2>(0,0);
    this->insertAlways(centerPoint);
    for(int i=0;i<m;i++){
        for(int j=0;j<n*(i+1);j++){
            GMlib::Angle a = (j * M_2PI)/(n*(i+1));
            GMlib::SqMatrix<float,2> mt(a,GMlib::Vector<float,2>(1,0),GMlib::Vector<float,2>(0,1));
            GMlib::Point<float,2> pt = mt * GMlib::Vector<float,2>((i+1)*r/m,0);
            this->insertAlways(pt);
        }
    }
}


void FEMObject::randomTriangulation(int n, float r){

    auto rn = 4;
    auto m = n/rn;
    regularTriangulation(rn,m,r);
    auto nm = std::max(M_PI / ((std::sqrt(3)*sin(M_PI/n)*sin(M_PI/n)+2-n)*0.5/n), (1.1));
    int num = 1 + n *nm;
    int t = num * 0.8;
    auto epsilon = 1e-5;

    //Initialize and seed rand
    GMlib::Random<int> rand;
    rand.set(0, this->size());
    rand.setSeed(3);

    for (int i=0;i<t;i++){
        std::swap((*this)[rand.get()],(*this)[rand.get()]);
    }

    for (int i=num;i<this->size();i++){
        auto boundaryVertex = this->getVertex(i)->getParameter();
        auto distanceFromCenter = std::sqrt ((boundaryVertex[0]* boundaryVertex[0]) -
                                              (boundaryVertex[1] * boundaryVertex[1]) );
        if( std::abs(distanceFromCenter - r) > epsilon)
            this->removeIndex(i);
    }

}

GMlib::Vector<GMlib::Vector<float,2>,3> FEMObject::vectorsArray(GMlib::TSEdge<float> *edg){

    GMlib::Array<GMlib::TSTriangle<float>*> tr = edg->getTriangle();
    GMlib::Array<GMlib::TSVertex<float>*>   v1 = tr[0]->getVertices();
    GMlib::Array<GMlib::TSVertex<float>*>   v2 = tr[1]->getVertices();
    GMlib::Vector<GMlib::Vector<float,2>,3> d; //output
    GMlib::Point<float,2> p0,p1,p2,p3;
    p0 = edg->getFirstVertex()->getParameter();
    p1 = edg->getLastVertex()->getParameter();
    //p2
    for(int i=0;i<3;i++){
        if(v1[i]!=edg->getFirstVertex() && v1[i]!=edg->getLastVertex())
            p2 = v1[i]->getParameter();
    }
    //p3
    for(int i=0;i<3;i++){
        if(v2[i]!=edg->getFirstVertex() && v2[i]!=edg->getLastVertex())
            p3 = v2[i]->getParameter();
    }

    d[0] = p1 - p0;
    d[1] = p2 - p0;
    d[2] = p3 - p0;

    return d;

}

GMlib::Vector<GMlib::Vector<float,2>,3> FEMObject::vectorsArray(GMlib::TSTriangle<float> *tr, Node *node){

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

void FEMObject::computation(){

    //Triangulate
    this->triangulateDelaunay();

    //Populate _nodes


    for (int i=0;i<this->size();i++){
        auto edges = this->getVertex(i)->getEdges() ;
        if (!(this->getVertex(i)->boundary())) {
            GMlib::TSVertex<float>* vt = this->getVertex(i) ;
             Node node = Node();
             node._vt = vt;
             //_nodes.operator +=(node); //bug
             _nodes.insertAlways(node,true);
        }
    }

    //Set dimensions for _A and _b : very important!

    _A.setDim(_nodes.size(),_nodes.size());
    _b.setDim(_nodes.size());

    //Set zeroes in _A
        for(int i=0; i<_A.getDim1();i++){
            for (int j=0;j<_A.getDim2();j++){
                _A[i][j] = 0;
            }
        }

    //Computation of _A matrix and _b

    for (int i=0;i<_nodes.size();i++){

        //Non diagonal elements of _A
        GMlib::Vector<GMlib::Vector<float,2>,3> d;
        for (int j=0;j<i;j++){
            GMlib::TSEdge<float>* edg = _nodes[i].neighbor(_nodes[j]);
            if (edg != NULL){
                d = vectorsArray(edg);
                auto d0 = d[0];
                auto d1 = d[1];
                auto d2 = d[2];
                auto dd =  1/ (std::abs(d0 * d0));
                auto dh1 = dd * (d1 * d0);
                auto dh2 = dd * (d2 * d0);
                auto area1 = std::abs(d0^d1);
                auto area2 = std::abs(d0^d2);
                auto h1 = dd * (area1 * area1);
                auto h2 = dd * (area2 * area2);

                _A[i][j] = (dh1 * (1 - dh1) / h1 - dd) * area1 * 0.5 +
                           (dh2 * (1 - dh2) / h2 - dd) * area2 * 0.5;

                _A[j][i] = _A[i][j];

            }
        }

        //Diagonal elements of _A and _b
        GMlib::Array <GMlib::TSTriangle<float>*> tr = _nodes[i].getTriangles();
        float Tk =0.0;
        float sum=0;
        for (int k=0;k<tr.size();k++){
           d = vectorsArray(tr[k],&_nodes[i]);
           auto d0 = d[0];
           auto d1 = d[1];
           auto d2 = d[2];
           Tk += (d2 * d2) / std::abs(d0 ^ d1) * 0.5;
           sum += tr[k]->getArea2D()/3;
        }
        _A[i][i] = Tk;
        _b[i] = sum;
    }

//    for(int i=0; i<_A.getDim1();i++){
//        for (int j=0;j<_A.getDim2();j++){
//           std::cout<< _A[i][j]<<"            ";
//        }
//        std::cout<<'\n';
//    }
//    std::cout<<"b follows"<<'\n';
//    for(int i=0; i<_A.getDim1();i++){
//        std::cout<<_b[i]<< ' ';
//    }

    _A.invert();

}

void FEMObject::updateHeight(float F){

    //Solving AX=b

    GMlib::DVector<float> X = _A * F * _b;
    for (int i=0;i<_nodes.size();i++){
        _nodes[i]._vt->setZ(X[i]);
    }
}
