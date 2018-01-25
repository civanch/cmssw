#ifndef SimG4CMS_CaloSD_h
#define SimG4CMS_CaloSD_h
///////////////////////////////////////////////////////////////////////////////
// File: CaloSD.h
// Description: Stores hits of calorimetric type in appropriate container
// Use in your sensitive detector builder:
//    CaloSD* caloSD = new CaloSD(SDname, new CaloNumberingScheme());
///////////////////////////////////////////////////////////////////////////////

#include "SimG4CMS/Calo/interface/CaloG4Hit.h"
#include "SimG4CMS/Calo/interface/CaloG4HitCollection.h"
#include "SimG4CMS/Calo/interface/CaloMeanResponse.h"
#include "SimG4Core/Notification/interface/Observer.h"
#include "SimG4Core/Notification/interface/BeginOfRun.h"
#include "SimG4Core/Notification/interface/BeginOfEvent.h"
#include "SimG4Core/Notification/interface/BeginOfTrack.h"
#include "SimG4Core/Notification/interface/EndOfTrack.h"
#include "SimG4Core/Notification/interface/EndOfEvent.h"
#include "SimG4Core/Notification/interface/TrackWithHistory.h"
#include "SimG4Core/SensitiveDetector/interface/SensitiveCaloDetector.h"
#include "SimG4Core/Application/interface/SimTrackManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
 
#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4VGFlashSensitiveDetector.hh"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class G4Step;
class G4HCofThisEvent;
class CaloSlaveSD;
class G4GFlashSpot;

class CaloSD : public SensitiveCaloDetector, 
               public G4VGFlashSensitiveDetector,
               public Observer<const BeginOfRun *>,    
               public Observer<const BeginOfEvent *>,
               public Observer<const BeginOfTrack *>,
               public Observer<const EndOfTrack *>,
               public Observer<const EndOfEvent *> {

public:    
  
  CaloSD(const std::string& aSDname, const DDCompactView & cpv,
         const SensitiveDetectorCatalog & clg,
         edm::ParameterSet const & p, const SimTrackManager*,
         float timeSlice=1., bool ignoreTkID=false);
  ~CaloSD() override;

  G4bool   ProcessHits(G4Step * step, G4TouchableHistory *) override;
  bool     ProcessHits(G4GFlashSpot* aSpot, G4TouchableHistory*) override;

  uint32_t setDetUnitId(const G4Step* step) override =0;
  
  void     Initialize(G4HCofThisEvent * HCE) override;
  void     EndOfEvent(G4HCofThisEvent * eventHC) override;
  void     clear() override;
  void     DrawAll() override;
  void     PrintAll() override;

  void     clearHits() override;
  void     fillHits(edm::PCaloHitContainer&, const std::string&) override;

protected:

  virtual double getEnergyDeposit(const G4Step* step); 
  virtual bool   getFromLibrary(const G4Step* step); 

  G4ThreeVector  setToLocal(const G4ThreeVector&, const G4VTouchable*) const; 
  G4ThreeVector  setToGlobal(const G4ThreeVector&, const G4VTouchable*) const; 

  bool           hitExists(const G4Step*);
  bool           checkHit();
  CaloG4Hit*     createNewHit(const G4Step*);
  void           updateHit(CaloG4Hit*);
  void           resetForNewPrimary(const G4Step*);
  double         getAttenuation(const G4Step* aStep, double birk1, double birk2,
                                double birk3) const; 

  void     update(const BeginOfRun *) override;
  void     update(const BeginOfEvent *) override;
  void     update(const BeginOfTrack * trk) override;
  void     update(const EndOfTrack * trk) override;
  void     update(const ::EndOfEvent *) override;
  virtual void     initRun();
  virtual bool     filterHit(CaloG4Hit*, double);

  virtual int      getTrackID(const G4Track*); 
  virtual uint16_t getDepth(const G4Step*);   
  double           getResponseWt(const G4Track*);
  int              getNumberOfHits();

  inline void setParameterized(bool val) { isParameterized = val; }

  inline void processHit(const G4Step* step) {
    // check if it is in the same unit and timeslice as the previous one
    if (currentID == previousID) {
      updateHit(currentHit);
    } else if (!checkHit()) {
      currentHit = createNewHit(step);
    }
  }

private:

  void             storeHit(CaloG4Hit*);
  bool             saveHit(CaloG4Hit*);
  void             summarize();
  void             cleanHitCollection();

protected:
  
  // Data relative to primary particle (the one which triggers a shower)
  // These data are common to all Hits of a given shower.
  // One shower is made of several hits which differ by the
  // unit ID (crystal/fibre/scintillator) and the Time slice ID.

  G4ThreeVector                   entrancePoint; 
  G4ThreeVector                   entranceLocal; 
  G4ThreeVector                   posGlobal;    
  float                           incidentEnergy;
  float                           edepositEM, edepositHAD;
  int                             primIDSaved;    //*  ID of the last saved primary

  CaloHitID                       currentID, previousID; 

  double                          energyCut, tmaxHit, eminHit, eminHitD;
  int                             checkHits;
  bool                            useMap;

  const SimTrackManager*          m_trackManager;
  CaloG4Hit*                      currentHit;

  bool                            runInit;

  bool                            corrTOFBeam, suppressHeavy;
  double                          correctT;
  double                          kmaxIon, kmaxNeutron, kmaxProton;

  bool                            forceSave;

private:

  std::unique_ptr<CaloSlaveSD>         slave;
  std::unique_ptr<CaloMeanResponse>    meanResponse;

  CaloG4HitCollection*            theHC; 

  bool                            ignoreTrackID;
  bool                            isParameterized; 

  int                             hcID;
  int                             primAncestor;
  int                             cleanIndex;
  int                             totalHits;

  float                           timeSlice;

  std::map<CaloHitID,CaloG4Hit*>  hitMap;
  std::map<int,TrackWithHistory*> tkMap;

  std::vector<CaloG4Hit*>         reusehit;
  std::vector<CaloG4Hit*>         hitvec;
  std::vector<unsigned int>       selIndex;

};

#endif // SimG4CMS_CaloSD_h
