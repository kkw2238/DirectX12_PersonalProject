#include "D3DUtil.h"

ComPtr<ID3DBlob> D3DUtil::LoadFileBinary(const std::wstring & fileName)
{
	std::ifstream ifs(fileName, std::ios::binary);

	ifs.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size = (int)ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);

	ComPtr<ID3DBlob> id3dFileDataStorage;
	D3DCreateBlob(size, id3dFileDataStorage.GetAddressOf());

	ifs.read((char*)id3dFileDataStorage->GetBufferPointer(), size);
	ifs.close();

	return id3dFileDataStorage;
}

ComPtr<ID3D12Resource> D3DUtil::CreateDefaultBuffer(ID3D12Device * device, ID3D12GraphicsCommandList * commandList, const void * defaultData, UINT64 byteSize, ComPtr<ID3D12Resource>& uploadBuffer)
{
	ComPtr<ID3D12Resource> iD3DDefaultBuffer;

	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(iD3DDefaultBuffer.GetAddressOf())
	);

	device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf())
	);

	D3D12_SUBRESOURCE_DATA subResourceData;
	subResourceData.pData = defaultData;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	ChangeResourceState(iD3DDefaultBuffer.Get(), commandList, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources<1>(commandList, iD3DDefaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
	ChangeResourceState(iD3DDefaultBuffer.Get(), commandList, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);

	return iD3DDefaultBuffer;
}

ComPtr<ID3DBlob> D3DUtil::CompileShader(const std::wstring & fileName, const D3D_SHADER_MACRO * defines, const std::string & entrypoint, const std::string & target)
{
	UINT compileFlags = 0;

#if defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = S_OK;

	ComPtr<ID3DBlob> id3dByteCode = nullptr;
	ComPtr<ID3DBlob> id3dErrorMsg;

	hr = D3DCompileFromFile(fileName.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entrypoint.c_str(), target.c_str(), compileFlags, 0, &id3dByteCode, &id3dErrorMsg);

	if (id3dErrorMsg != nullptr)
		OutputDebugStringA((char*)id3dErrorMsg->GetBufferPointer());

	return id3dByteCode;
}

void D3DUtil::ChangeResourceState(ID3D12Resource* resource, ID3D12GraphicsCommandList * commandList, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource, beforeState, afterState));
}

bool D3DUtil::IsKeyDown(char key)
{
	SHORT keyDown = GetAsyncKeyState(key);
	if (keyDown & 0x0001) return true;
	else if (keyDown & 0x8000) return true;
	return false;
}

std::wstring D3DUtil::StringToWString(std::string & str)
{
	std::wstring wString;
	wString.assign(str.begin(), str.end());
	return wString;
}

ExeptionUtility::ExeptionUtility(const HRESULT hr, const int line, const std::string& fileName, const std::string& functionName) :
	errorCode(hr), errorLine(line),	errorFileName(fileName), errorFunctionName(functionName)
{}

std::wstring ExeptionUtility::D3DErrorToString()
{
	_com_error errorCode(errorCode);
	std::wstring interpretError = errorCode.ErrorMessage();
	size_t charLocation = errorFileName.rfind('\\') + 1;

	std::string errorMsg;
	errorMsg =
		"Error!!! \n FileName : " + errorFileName.substr(charLocation) + '\n' +
		"FuncName : " + errorFunctionName + '\n' +
		"Line : " + std::to_string(errorLine);

	return D3DUtil::StringToWString(errorMsg) + L"Error : " + interpretError;
}

std::wstring ExeptionUtility::WinErrorToString()
{
	TCHAR* errorMsg;
	DWORD createWindowErrorCode = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		nullptr, (int)errorCode + 0x3e80,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(TCHAR*)&errorMsg,
		0,
		nullptr
	);

	return std::wstring(errorMsg);
}
