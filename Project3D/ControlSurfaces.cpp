#include "ControlSurfaces.h"
#include <cmath>

ControlSurfaces::ControlSurfaces(float S, ObjForces& forces)
    : S(S),
      forces(forces) {
}

void ControlSurfaces::SpawnControlWindow() {
}

void ControlSurfaces::Update() {
    //float dynamicPressure = 0.5f * forces->GetP() * std::pow(forces->GetV(), 2);
    //float liftForce = dynamicPressure * S * liftCoefficient; ///ÉýÁ¦ 
    //return liftForce * leverArm - dampingFactor * pitchRate;
}