
#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"


bool is_number(const std::string& stn)
{
    std::string::const_iterator it = stn.begin();
    while (it != stn.end() && std::isdigit(*it)) ++it;
    return !stn.empty() && it == stn.end();
}

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction, B1RunAction* runAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  fRunAction(runAction)
{}


B1SteppingAction::~B1SteppingAction()
{}


void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!step->GetTrack()->GetNextVolume()) {return;}
    /*if (!fScoringVolume) {
    const B1DetectorConstruction* detectorConstruction
            = static_cast<const B1DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fScoringVolume = detectorConstruction->GetScoringVolume();
  }*/

    auto touchable = (step->GetPreStepPoint()->GetTouchable());
	auto touchableVol = touchable->GetVolume()->GetName();
	auto DepEnr = step->GetTotalEnergyDeposit();
    std::string VolName = (std::string) touchableVol;

    if (is_number(VolName)) {
        (fRunAction->vDepoEnr_run)[std::stoi(VolName)-1] += DepEnr;
        //take last element (back of vector) from vector of losses,
        //and fill it with deposited energy according to number of detector
        //last element is added at the beginning of every event
        //std::cout << DepEnr << std::endl;
        (fRunAction->fEdepTot) += DepEnr;

        //fEventAction->prevVol = VolName;
    }


}
