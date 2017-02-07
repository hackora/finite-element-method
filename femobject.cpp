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
