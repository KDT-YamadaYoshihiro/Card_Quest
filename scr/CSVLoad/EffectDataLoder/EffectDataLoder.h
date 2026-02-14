#pragma once
#include "CSVLoad/CSVLoader.h"
#include "System/Singleton/Singleton.h"

class EffectDataLoder : public CSVLoader, public Singleton<EffectDataLoder>
{
private:
	friend class Singleton<EffectDataLoder>;

public:




};

