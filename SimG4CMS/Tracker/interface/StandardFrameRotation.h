#ifndef SimG4CMS_StandardFrameRotation_H
#define SimG4CMS_StandardFrameRotation_H

#include "SimG4Core/SensitiveDetector/interface/FrameRotation.h"

#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"

/**
 * To be used for test beam etc. Note: if the sensitive detecor is created without an organization, force this one
 */

class StandardFrameRotation : public FrameRotation 
{
public:
  ~StandardFrameRotation() override = default;
  Local3DPoint transformPoint(const Local3DPoint &, const G4VPhysicalVolume * v=nullptr) const override;
};

#endif
