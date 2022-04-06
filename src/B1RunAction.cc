#include <vector>
#include <map>

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"


B1RunAction::B1RunAction() : G4UserRunAction(),
  fNofLayers(30), //Number of layers
  hist(500,0), //histogram
  fEdepTot(0.)
{

  vDepoEnr_run.resize(fNofLayers, 0.);
}


B1RunAction::~B1RunAction()
{}


void B1RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  fEdepTot = 0.;


  for(size_t i = 0; i<500; i++){ hist[i]=0; }
    vDepoEnr_run.resize(fNofLayers, 0.);
}


void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;


  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const B1PrimaryGeneratorAction* generatorAction
        = static_cast<const B1PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // Print
  //
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }


     std::vector<G4double> vCumulLosses (vDepoEnr_run);
     vCumulLosses.resize(fNofLayers, 0);

     G4cout << "Writing spectrum in layers.dat file..." << G4endl;
     std::ofstream filed1("layers.dat", std::ios::trunc);
     //double bin_width = (HIST_MAX - HIST_MIN) / NOBINS;
     for(int i = 0; i<fNofLayers; i++){
         //double energy0 = i*bin_width + HIST_MIN;
         filed1 << i << " " << vDepoEnr_run[i]/MeV/nofEvents << std::endl;
         if (i == 0) {
             vCumulLosses[i] = vDepoEnr_run[i];
         }
         else {
             vCumulLosses[i] = vCumulLosses[i-1] + vDepoEnr_run[i];
         }

     }
     filed1.close();



     G4cout << "Writing spectrum in CumulLosses.dat file..." << G4endl;
     std::ofstream filed2("CumulLosses.dat", std::ios::trunc);
     //double bin_width = (HIST_MAX - HIST_MIN) / NOBINS;
     for(int i = 0; i<fNofLayers; i++){
         //double energy0 = i*bin_width + HIST_MIN;
         filed2 << i << " " << vCumulLosses[i]/MeV/nofEvents << std::endl;
     }
     filed2.close();
     G4cout << G4endl << "Total energy deposit per one primary particle: "
     << fEdepTot/MeV/nofEvents << "MeV" << G4endl;
     G4cout << "Done!" << G4endl;

 }

/*void B1RunAction::AddEdep(G4double edep){
  fEdep  += edep;
  fEdep2 += edep*edep;
}

void B1RunAction::PutInHisto(G4double edep){
    double bin_width = (HIST_MAX - HIST_MIN) / NOBINS;
    int index0 = int(floor((edep-HIST_MIN)/bin_width/keV)); //energy spread simulation is used
    if(index0 > 0 && index0 < NOBINS){ hist[index0]++; }
    ftotEnerg += edep;
}
*/
//void B1RunAction::PutInArray(int index, G4double dEnergy){

    //vector<G4double> dGausdEnergy[100]
    //dGausdEnergy[index].push_beck(dEnergy);
//}
//
// void B1RunAction::PutInVector(G4double edep){
//     dGausdEnergy.push_back(edep);
// }
