#include "CompiledShader.h"



CompiledShader::CompiledShader()
{
}


CompiledShader::~CompiledShader()
{
}

CompiledShader * CompiledShader::Instance()
{
	static CompiledShader instance;

	return &instance;
}

D3D12_SHADER_BYTECODE CompiledShader::ShaderByteCode(ComPtr<ID3DBlob> shaderCode)
{
	D3D12_SHADER_BYTECODE result;

	if (shaderCode != nullptr) {
		result.pShaderBytecode = shaderCode->GetBufferPointer();
		result.BytecodeLength = shaderCode->GetBufferSize();
	}

	return result;
}

D3D12_SHADER_BYTECODE CompiledShader::GetShaderByteCode(const std::wstring & filename, const D3D_SHADER_MACRO * defines, const std::string & entrypoint, const std::string & target)
{
	if (compiledShaders[entrypoint] == nullptr)
		compiledShaders[entrypoint] = D3DUtil::CompileShader(filename, defines, entrypoint, target);

	return ShaderByteCode(compiledShaders[entrypoint]);
}
