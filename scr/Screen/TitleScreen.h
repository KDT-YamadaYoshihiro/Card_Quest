#pragma once
#include "ScreenBase.h"

class TitleScreen : public ScreenBase
{



public:

	TitleScreen() = default;
	virtual ~TitleScreen() = default;

	void Init() override;
	void Update() override;
	void Render() override;
	void End() override;


};