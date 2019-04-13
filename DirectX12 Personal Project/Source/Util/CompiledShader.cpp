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
	CreateShaderByteCode(L"hlsl\\Deferred.hlsl", nullptr, "VSTextureFullScreen", "vs_5_0");
	CreateShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "VSTextureDebug", "vs_5_1");

	CreateShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "PS", "ps_5_1");
	CreateShaderByteCode(L"hlsl\\Deferred.hlsl", LIGHT_MANAGER->GetShaderDefined().data(), "PSTextureFullScreen", "ps_5_0");
	CreateShaderByteCode(L"hlsl\\DefaultShader.hlsl", nullptr, "PSTextureDebug", "ps_5_1");

	CreateShaderByteCode(L"hlsl\\Mix.hlsl", nullptr, "CalculateLumFirstPass", "cs_5_0");
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

	D3D12_SHADER_BYTECODE nullvalue;
	::ZeroMemory(&nullvalue, sizeof(D3D12_SHADER_BYTECODE));

	return nullvalue;
}
