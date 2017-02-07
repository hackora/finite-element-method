#ifndef FEMOBJECT_H
#define FEMOBJECT_H

#include <gmTrianglesystemModule>

class FEMObject : public GMlib::TriangleFacets<float>
{
public:
    FEMObject();
};

#endif // FEMOBJECT_H
