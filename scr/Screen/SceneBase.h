#pragma once


class SceneBase
{

public:

	SceneBase() = default;
	virtual ~SceneBase() = default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void End() = 0;

};