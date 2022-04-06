#include "TROOT.h"
#include "TSystem.h"

#include <iostream>
#include <sstream>

using namespace std;

int main(){

    Double_t dCriticalEnergy  = 610/(74+1.24);   // in MeV unit
    Double_t dRadiationLength = 180*(184/(74*74)); // g/cm^2
    Double_t dDensity         = 19.25;           // g/cm^3

    Double_t Rm = dDensity*21/dCriticalEnergy;   // g/cm^2

    Double_t dRadius = Rm/dDensity;             // Radius of calorimeter in cm

    cout<<Rm<<endl;
    cout<<dRadius<<endl;
    cout<<dRadius*10.<<" mm"<<endl;
    return 0;
}
