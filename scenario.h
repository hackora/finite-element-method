#ifndef SCENARIO_H
#define SCENARIO_H

class Simulator;
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

private:
  std::shared_ptr<Simulator> _simulator;
};

#endif // SCENARIO_H
