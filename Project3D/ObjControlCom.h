#pragma once
#include <map>
#include "ControlBase.h"

class ObjControlComputer {
public:
    ObjControlComputer();
    void Update();
    //1=engine,2=rudder,3=autopilot
    std::map<int, ControlBase&> controls;
    //double getThrust() const;
    //double getRudderAngle() const;
    //int getAutopilotTargets() const;

private:
    //EngineControl engineControl;
    //RudderControl rudderControl;
    //Autopilot autopilot;
};