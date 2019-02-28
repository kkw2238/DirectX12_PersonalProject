#pragma once
#include "PipelineObject.h"

class ComputeObjectShader : public ShaderObject
{
public:
	ComputeObjectShader();
	~ComputeObjectShader();

protected:
	XMUINT3 m_ThreadCount;

};

