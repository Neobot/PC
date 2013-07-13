#ifndef VIEWINTERFACE_H
#define VIEWINTERFACE_H

#include "Acquisition.h"

class ViewInterface
{
public:
	virtual void addPositionAcquisition(int t, const PositionSample& sample, int ct, const CommandSample& csample) = 0;
	virtual void addAvoidingSharpAcquisition(int t, const SharpSample& sample) = 0;
	virtual void setAcquisition(const Acquisition* acquisition) = 0;
	virtual void clear() = 0;
};

#endif // VIEWINTERFACE_H
