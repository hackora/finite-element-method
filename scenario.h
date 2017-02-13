#ifndef SCENARIO_H
#define SCENARIO_H

class FEMObject;

#include "application/gmlibwrapper.h"

// qt
#include <QObject>



class Scenario : public GMlibWrapper {
  Q_OBJECT
public:
  using GMlibWrapper::GMlibWrapper;

  void    initializeScenario() override;
  void    cleanupScenario() override;
  void    animation();
  FEMObject*  _tRegular;
  FEMObject*  _tRandom;

  void regularFem();
  void randomFem();

};

#endif // SCENARIO_H
