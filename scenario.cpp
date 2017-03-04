#include "scenario.h"
#include "femobject.h"

//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmOpenglModule>
#include <gmSceneModule>
#include <gmParametricsModule>

// qt
#include <QQuickItem>


//

void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8, 0.002, 0.0008);
  scene()->insertLight( light, false );

  // Insert Sun
  scene()->insertSun();

//  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3> init_cam_pos(  0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  0.0f, 0.0f, 1.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Front cam
  auto front_rcpair = createRCPair("Front");
  front_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, -50.0f, 0.0f ), init_cam_dir, init_cam_up );
  front_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( front_rcpair.camera.get() );
  front_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Side cam
  auto side_rcpair = createRCPair("Side");
  side_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( -50.0f, 0.0f, 0.0f ), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ), init_cam_up );
  side_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( side_rcpair.camera.get() );
  side_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Top cam
  auto top_rcpair = createRCPair("Top");
  top_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, 0.0f, 50.0f ), -init_cam_up, init_cam_dir );
  top_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( top_rcpair.camera.get() );
  top_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );


// FEM Visualization

//  auto _t = std::make_unique<FEMObject*>(new FEMObject());
//  (*_t)->regularTriangulation(4,3,2);
//  (*_t)->toggleDefaultVisualizer();
//  (*_t)->replot();
//  scene()->insert((*_t));

  _tRegular = new FEMObject();
  _tRandom = new FEMObject();

//  _tRegular->setMaxForce(2);
//  _tRegular->regularTriangulation(10,3,4);
//  _tRegular->computation();
//  _tRegular->rotate( GMlib::Angle(180), GMlib::Vector<float,3>( 0.0f, 1.0f, 0.0f) );
//   _tRegular->findSpheresPos();
//  for (unsigned int i=0;i<_tRegular->spheres.size() ;i++){
//      auto sphere=  _tRegular->spheres[i];
//      sphere->setMaterial(GMlib::GMmaterial::blackPlastic());
//      sphere->toggleDefaultVisualizer();
//      sphere->replot(10,10,1,1);
//      scene()->insert(sphere);
//  }
//  _tRegular->toggleDefaultVisualizer();
//  _tRegular->setMaterial(GMlib::GMmaterial::turquoise());
//  _tRegular->replot();
//  scene()->insert(_tRegular);


  _tRandom->setMaxForce(2);
  _tRandom->randomTriangulation(40,4);
  _tRandom->computation();
  _tRandom->findSpheresPos();
    for (unsigned int i=0;i<_tRandom->spheres.size() ;i++){
        auto sphere=  _tRandom->spheres[i];
        sphere->setMaterial(GMlib::GMmaterial::emerald());
        sphere->toggleDefaultVisualizer();
        sphere->replot(10,10,1,1);
        scene()->insert(sphere);
    }

  _tRandom->toggleDefaultVisualizer();
  _tRandom->setMaterial(GMlib::GMmaterial::brass());
 // _tRandom->translateGlobal(GMlib::Vector<float,3>{-8.0f,0.0f,0.0f});
  _tRandom->replot();
  scene()->insert(_tRandom);

}

void Scenario::randomFem(){

}

void Scenario::regularFem(){

}

void Scenario::animation(){
    //_tRegular->replot();
    _tRandom->replot();

}

void Scenario::cleanupScenario() {

}
