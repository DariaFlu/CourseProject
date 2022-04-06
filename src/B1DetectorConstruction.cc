#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"

#include <string>
#include <vector>

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
fScoringVolume(0),//ionization chamber
fFrontChVolume(0), //front chamber
vLayers(0) //Layers for proccesing  of ionization losses
//NofLayers(-1)
{ }


B1DetectorConstruction::~B1DetectorConstruction() { }


G4VPhysicalVolume* B1DetectorConstruction::Construct()


{

    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    // Envelope parameters
    /*G4double env_sizeX = 100*mm, env_sizeY = 100*mm, env_sizeZ = 2000*mm;
    G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");*/

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    //Create the box - mother volume "world"
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

    G4double world_x = 50.*mm;
    G4double world_y = 50.*mm;
    G4double world_z = 300.*mm;

    G4Box* solidWorld =
        new G4Box("world_box",
                    world_x, world_y, 0.5*world_z);

    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(solidWorld,
                            world_mat,
                            "World");

    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(0,
                          G4ThreeVector(),
                          logicWorld,
                          "World",
                          0,
                          false,
                          0,
                          checkOverlaps);

//-----------------------------------------------------------------------

//--------------THE PARTICLE SOURCE--------------------------------------
    //the particles source
    G4Material* source_mat = nist->FindOrBuildMaterial("G4_Galactic");

    G4Box* source =
        new G4Box("source",
                  5.*mm,
                  5.*mm,
                  5.*mm
                );
    G4LogicalVolume* source_log =
            new G4LogicalVolume(source, source_mat,"source_log"); //The logical volume for particles source

    new G4PVPlacement(0 , // no rotation
        G4ThreeVector(0, 0, -5.*mm),
                source_log,
                "sourcePlace",
                logicWorld, //LOGICAL VOLUME
                false,
                0);

//-----------------------------------------------------------------------

//----------------Geometry of calorimeter---------------------------------

    G4double dCriticalEnergy  = 610/(74+1.24);   // in MeV unit
    G4double dRadiationLength = 180*184/(74*74); // g/cm^2
    G4double dDensity         = 19.25;           // g/cm^3

    G4double Rm = dRadiationLength*21/dCriticalEnergy;   // g/cm^2

    G4double dRadius = Rm/dDensity;             // Radius of calorimeter in cm
    //G4cout << G4endl << "Critical radius: " << dRadius << "cm" << G4endl;

//----------------------------------------------------------------------

    G4double fNofLayers = 30;

    G4double fThickfTungsten = 3.5*mm;
    G4double fThickfOrg      = 1.0*mm;

    G4double dist = 0 + fThickfTungsten*0.5;
    //NEED TO CHANGE DIST FROM WORLD CENTER TO SOURCE

    G4double detX = 10.*mm;
    G4double detY = 10.*mm;
    G4double detZ = fThickfTungsten; //GEOMETRY OF CALORIMETER

    std::vector<G4VPhysicalVolume*> pv;
    std::vector<G4VPhysicalVolume*> pvW;


//tung_mat!!!!!!
//org_mat!!!!!!

// G4Tubs(const G4String& pname, // name
// G4double pRmin, // inner radius
// G4double pRmax, // outer radius
// G4double pDz, // Z half length
// 0.*deg;, // starting Phi
// 360.*deg;); // segment angle


    G4Material* tung_mat = nist->FindOrBuildMaterial("G4_W");

    G4Material* org_mat  = nist->FindOrBuildMaterial("G4_POLYSTYRENE");

    /* G4Box* solidTungsten = new G4Box("Det",
                           5.*mm, 5.*mm, 0.5*fThickfTungsten);

    G4Box* solidOrg      = new G4Box("Det",
                           10.*mm, 10.*mm, 0.5*fThickfOrg);*/

    G4Tubs* solidTungsten = new G4Tubs("Det1",
                                        0.*mm,
                                        dRadius*cm,
                                        0.5*fThickfTungsten,
                                        0.*deg,
                                        360.*deg);

    G4Tubs* solidOrg = new G4Tubs("Det2",
                                        0.*mm,
                                        dRadius*cm,
                                        0.5*fThickfOrg,
                                        0.*deg,
                                        360.*deg);

    std::vector<G4LogicalVolume*> vLayers1;

    for (int i = 1; i <= fNofLayers; i++){

        std::string iw = "w" + std::to_string(i);

        G4LogicalVolume* logicW =
            new G4LogicalVolume(solidTungsten, tung_mat, iw);

        vLayers1.push_back(logicW);

        G4VPhysicalVolume* physW =
        new G4PVPlacement(0,
                          G4ThreeVector(0., 0., dist),
                          vLayers1[i-1],
                          iw,
                          logicWorld,
                          false,
                          0,
                          1);
        //pv.push_back(physDet);
        dist += fThickfTungsten/2+fThickfOrg/2.;

        G4LogicalVolume* logicDet =
            new G4LogicalVolume(solidOrg, org_mat, std::to_string(i));

        vLayers.push_back(logicDet);

        G4VPhysicalVolume* physDet =
        new G4PVPlacement(0,
                          G4ThreeVector(0., 0., dist),
                          vLayers[i-1],
                          std::to_string(i),
                          logicWorld,
                          false,
                          0,
                          1);
        pv.push_back(physDet);

        dist += fThickfTungsten/2+fThickfOrg/2.;


    }


    //fScoringVolume = cal_log;

    return physWorld;
}
