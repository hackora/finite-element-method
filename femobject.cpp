#include "femobject.h"
#include <gmCoreModule>

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
    //this->triangulateDelaunay();
}


void FEMObject::randomTriangulation(int n, float r){

    auto rn = 4;
    auto m = n/rn;
    regularTriangulation(rn,m,r);
    auto nm = std::max(M_PI / ((std::sqrt(3)*sin(M_PI/n)*sin(M_PI/n)+2-n)*0.5/n), (1.1));
    //auto nm = 0.2;
    int num = 1 + n *nm;
    int t = num * 0.8;
    GMlib::Random<int> rand;
    rand.set(0, this->size());
    rand.setSeed(3);
    auto epsilon = 1e-5;
    for (int i=0;i<t;i++){
        std::swap((*this)[rand.get()],(*this)[rand.get()]);
    }
    //TODO: Remove all elements after index num that are not on the boundary

    for (int i=num;i<this->size();i++){
        auto parameters = this->getVertex(i)->getParameter();
        auto distanceFromCenter = std::sqrt ((parameters[0]* parameters[0]) -
                                              (parameters[1] * parameters[1]) );
        if( std::abs(distanceFromCenter - r) <= epsilon){
            std::cout<<"I am a boundary vertex : second half"<<'\n';
        }
        else
            this->removeIndex(i);
    }

    this->triangulateDelaunay();

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
    //populate _nodes

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

    _A.setDim(_nodes.size(),_nodes.size());
    //set zeroes in _A
        for(int i=0; i<_A.getDim1();i++){
            for (int j=0;j<_A.getDim2();j++){
                _A[i][j] = 0;
            }
        }

    //computation

    for (int i=0;i<_nodes.size();i++){

        //Non diagonal elements
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

        //Diagonal elements
        GMlib::Array <GMlib::TSTriangle<float>*> tr = _nodes[i].getTriangles();
        float Tk =0.0;
        for (int k=0;k<tr.size();k++){
           d = vectorsArray(tr[k],&_nodes[i]);
           auto d0 = d[0];
           auto d1 = d[1];
           auto d2 = d[2];
           Tk += (d2 * d2) / std::abs(d0 ^ d1) * 0.5;
        }
        _A[i][i] = Tk;
    }

    for(int i=0; i<_A.getDim1();i++){
        for (int j=0;j<_A.getDim2();j++){
           std::cout<< _A[i][j]<< ' ';
        }
    }

    for (int i=0;i<_nodes.size();i++){
        GMlib::Array <GMlib::TSTriangle<float>*> tr = _nodes[i].getTriangles();
        float sum=0;
        for (int k=0;k<tr.size();k++){
            sum += tr[k]->getArea2D()/3;
        }
        _b[i] = sum;
    }

    _A.invert();

}

void FEMObject::updateHeight(float F){

    //Solving AX=b

//    GMlib::DVector<float> X = _A * _b;
//    X *=F;
//    for (int i=0;i<_nodes.size();i++){
//        _nodes[i]._vt->setZ(X[i]);
//    }
    //std::cout<<X<<'\n';
}
