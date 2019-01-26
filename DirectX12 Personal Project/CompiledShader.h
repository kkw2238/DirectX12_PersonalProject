#pragma once
/*
 * CompiledShader Ŭ������ �����ϵ� ���̴� �ڵ尡 ����ִ� Ŭ������.
 * hlsl�ߺ� �������� �����ϱ� ���� ������� ����.
 */


#include "Source/D3DUtil.h"
#include <map>

class CompiledShader
{
public:
	CompiledShader();
	~CompiledShader();

	static CompiledShader* Instance();

public:
	D3D12_SHADER_BYTECODE	ShaderByteCode(ComPtr<ID3DBlob> shaderCode);
	D3D12_SHADER_BYTECODE	GetShaderByteCode(const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);

private:
	std::unordered_map<std::string, ComPtr<ID3DBlob>> compiledShaders;
};

#define COMPILEDSHADER CompiledShader::Instance()