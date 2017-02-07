#include "femobject.h"

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
            GMlib::Point<float,2> pt = mt * GMlib::Point<float,2>((i+1)/m*r/(j+1),0);
            this->insertAlways(pt);
        }
    }
}
