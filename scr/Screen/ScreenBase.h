#pragma once


class ScreenBase
{



public:

	ScreenBase() = default;
	virtual ~ScreenBase() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void End() = 0;

};