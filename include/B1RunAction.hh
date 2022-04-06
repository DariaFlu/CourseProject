
#ifndef B1RunAction_h
#define B1RunAction_h 1

#include <vector>
#include <map>
#include <math.h>
#include <fstream>

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"




class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class B1RunAction : public G4UserRunAction
{
  public:
    B1RunAction();
    virtual ~B1RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    //void AddEdep (G4double edep);
    //void PutInHisto(G4double edep);
    std::vector<G4double> vDepoEnr_run ;
    G4int nEvents = 0;
    G4double fEdepTot = 0;



  private:
    std::vector<G4double> hist;
    G4double HIST_MIN = 0.;
    G4double HIST_MAX = 100.;
    G4int NOBINS = 500;

    G4int fNofLayers;

    //vector<G4double> dGausdEnergy[100];
    //G4double dGausdEnergy[fNofLayers];


};

#endif
