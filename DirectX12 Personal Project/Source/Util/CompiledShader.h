#pragma once
/*
 * CompiledShader Ŭ������ �����ϵ� ���̴� �ڵ尡 ����ִ� Ŭ������.
 * hlsl�ߺ� �������� �����ϱ� ���� ������� ����.
 */


#include "D3DUtil.h"
#include <map>

class CompiledShader
{
public:
	CompiledShader();
	~CompiledShader();

	static CompiledShader* Instance();

public:
	void CreateShaders();
	void CreateShaderByteCode(const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target);

	D3D12_SHADER_BYTECODE	ShaderByteCode(ComPtr<ID3DBlob> shaderCode);
	D3D12_SHADER_BYTECODE	GetShaderByteCode(const std::string& codeName);

private:
	std::unordered_map<std::string, ComPtr<ID3DBlob>> compiledShaders;
};

#define COMPILEDSHADER CompiledShader::Instance()