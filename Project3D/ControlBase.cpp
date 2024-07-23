#include "ControlBase.h"

ControlBase::ControlBase() {
}

void ControlBase::SetStatus(int sStatus) { status = sStatus; }
void ControlBase::IncStatus(int inc) { status += inc; };
int ControlBase::GetStatus() { return status; };
float ControlBase::GetStatus2() { return status2; };