#include "CompiledShader.h"
#include "LightManager.h"


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

void CompiledShader::CreateShaders()
{
	CreateShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "VS", "vs_5_1");
	CreateShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "VSTextureFullScreen", "vs_5_0");

	CreateShaderByteCode(L"hlsl\\DefaultShader.hlsl", LIGHT_MANAGER->GetShaderDefined().data(), "PS", "ps_5_1");
	CreateShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "PSTextureFullScreen", "ps_5_0");
}

void CompiledShader::CreateShaderByteCode(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target)
{
	if (compiledShaders[entrypoint] == nullptr)
		compiledShaders[entrypoint] = D3DUtil::CompileShader(filename, defines, entrypoint, target);
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

D3D12_SHADER_BYTECODE CompiledShader::GetShaderByteCode(const std::string & codeName)
{
	if (compiledShaders[codeName] != nullptr)
		return ShaderByteCode(compiledShaders[codeName]);

	return D3D12_SHADER_BYTECODE();
}
