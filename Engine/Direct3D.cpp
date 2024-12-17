#include "Direct3D.h"
#include <d3dcompiler.h>
#include "Define.h"

namespace Direct3D
{
	ID3D11Device* pDevice; //�f�o�C�X(Direct3D���̂���)
	ID3D11DeviceContext* pContext; //�f�o�C�X�R���e�L�X�g(�`�揈��)
	IDXGISwapChain* pSwapChain; //�X���b�v�`�F�C��(�o�b�t�@���X���b�v)
	ID3D11RenderTargetView* pRenderTargetView; //�����_�[�^�[�Q�b�g�r���[(�`�����ɃA�N�Z�X���邽�߂̂���)
	ID3D11VertexShader* pVertexShader = nullptr; //���_�V�F�[�_�[
	ID3D11PixelShader* pPixelShader = nullptr; //�s�N�Z���V�F�[�_�[
	ID3D11InputLayout* pVertexLayout = nullptr; //���_�C���v�b�g���C�A�E�g
	ID3D11RasterizerState* pRasterizerState = nullptr; //���X�^���C�U�[

	ID3D11Texture2D* pDepthStencil;//�[�x�X�e���V��
	ID3D11DepthStencilView* pDepthStencilView; //�[�x�X�e���V���r���[

	XMINT2 WindowSize;

	struct SHADER_BUNDLE
	{
		ID3D11VertexShader* pVertexShader_ = nullptr; //���_�V�F�[�_�[
		ID3D11PixelShader* pPixelShader_ = nullptr; //�s�N�Z���V�F�[�_�[
		ID3D11InputLayout* pVertexLayout_ = nullptr; //���_�C���v�b�g���C�A�E�g
		ID3D11RasterizerState* pRasterizerState_ = nullptr; //���X�^���C�U�[
	};

	SHADER_BUNDLE shaderBundle[SHADER_MAX];
	XMFLOAT4 lightPos = {0,0.5,0,0};
}

HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	HRESULT hr;

	///////////////////////////���낢�돀�����邽�߂̐ݒ�///////////////////////////////
	//���낢��Ȑݒ荀�ڂ��܂Ƃ߂��\����
	DXGI_SWAP_CHAIN_DESC scDesc;

	//�Ƃ肠�����S��0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//�`���̃t�H�[�}�b�g
	scDesc.BufferDesc.Width = winW; //��ʕ�
	scDesc.BufferDesc.Height = winH; //��ʍ���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //���F�g���邩

	//FPS(1/60�b��1��)
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//���̑�
	scDesc.Windowed = TRUE; //�E�B���h�E���[�h���t���X�N���[����
	scDesc.OutputWindow = hWnd; //�E�B���h�E�n���h��
	scDesc.BufferCount = 1; //�o�b�N�o�b�t�@�̖���
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
	scDesc.SampleDesc.Count = 1; //MSAA(�A���`�G�C���A�X)�̐ݒ�
	scDesc.SampleDesc.Quality = 0; //����

	////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, //�ǂ̃r�f�I�A�_�v�^���g�p���邩?����Ȃ��nullptr��
		D3D_DRIVER_TYPE_HARDWARE,//�h���C�o�̃^�C�v��n���A�ӂ���HARDWARE
		nullptr,//��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
		0, //���炩�̃t���O���w�肷��(�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG)
		nullptr, //�f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Anullptr�ɂ��Ƃ���OK
		0, //��̈����Ń��x�������w�肵����
		D3D11_SDK_VERSION, //SDK�̃o�[�W�����A�K�����̒l
		&scDesc, //��ł��낢��ݒ肵���\����
		&pSwapChain, //������������SwapChain�̃A�h���X���Ԃ��Ă���
		&pDevice, //������������Device�A�h���X���Ԃ��Ă���
		&level, //������������Device�AContext�̃��x�����Ԃ��Ă���
		&pContext); //������������Context�̃A�h���X���Ԃ��Ă���
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �X���b�v�`�F�[���쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
	//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾(�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g)
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//�����_�[�^�[�Q�b�g�r���[���쐬
	//�����G���[�o����if(pBackBuffer!=0)�������Ă���
	if (pBackBuffer != 0)
	{
		hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
		if (FAILED(hr))
		{
			MessageBox(nullptr, L"���s: �����_�[�^�[�Q�b�g�r���[�쐬", L"Direct3D.cpp", MB_OK);
			return hr;
		}
	}

	//�ꎞ�I�Ƀo�b�N�o�b�t�@���擾���������Ȃ̂ŉ��
	SAFE_RELEASE(pBackBuffer);

	///////////////////////////�r���[�|�[�g(�`��͈�)�ݒ�///////////////////////////////
	//�����_�����O���ʂ�\������͈�
	D3D11_VIEWPORT vp;
	vp.Width = (float)winW;	//��
	vp.Height = (float)winH; //����
	vp.MinDepth = 0.0f;	//��O
	vp.MaxDepth = 1.0f; //��
	vp.TopLeftX = 0; //��
	vp.TopLeftY = 0; //��

	//�[�x�X�e���V���r���[�̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = winW;
	descDepth.Height = winH;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���f����2D�e�N�X�`���쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}
	hr = pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �[�x�X�e���V���r���[�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�(�p�C�v���C��)
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //�f�[�^�̓��͎�ނ��w��
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView); //�`����ݒ�
	pContext->RSSetViewports(1, &vp);

	//�V�F�[�_�[����
	hr = InitShader();
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �V�F�[�_�[����", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	WindowSize = { winW,winH };

	return S_OK;
}

HRESULT Direct3D::InitShader2D()
{
	//--------2D�p�J�n-----------------------------------------------
	HRESULT hr;
	//���_�V�F�[�_�̍쐬(�R���p�C��)
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Engine//Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);

	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_2D].pVertexShader_);
	//�G���[��������v���O�����I��
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���_�V�F�[�_�̍쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g�쐬->���_��񂾂���n���Ă���
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, //�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 }, //UV���W
	};
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateInputLayout(layout, 2, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_TYPE::SHADER_2D].pVertexLayout_);
	SAFE_RELEASE(pCompileVS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���_�C���v�b�g���C�A�E�g�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//�s�N�Z���V�F�[�_�쐬(�R���p�C��)
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Engine//Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_2D].pPixelShader_);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �s�N�Z���V�F�[�_�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK; //���p�`�̗����͕`�悵�Ȃ�(�J�����O)
	rdc.FillMode = D3D11_FILL_SOLID; //���p�`�̓�����h��Ԃ�()
	//rdc.FillMode = D3D11_FILL_WIREFRAME; //���C���[�t���[���\��
	rdc.FrontCounterClockwise = FALSE; //�����v����\�ɂ���A��FALSE=���v��肪�\
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_TYPE::SHADER_2D].pRasterizerState_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���X�^���C�U�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}
	//--------2D�p�I��-----------------------------------------------
	return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
	//--------3D�p�J�n-----------------------------------------------
	HRESULT hr;

	//���_�V�F�[�_�̍쐬(�R���p�C��)
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Engine//Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_3D].pVertexShader_);
	//�G���[��������v���O�����I��
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���_�V�F�[�_�̍쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g�쐬->���_��񂾂���n���Ă���
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, //�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 }, //UV���W
		{ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },//�@��
	};
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateInputLayout(layout, 3, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_TYPE::SHADER_3D].pVertexLayout_);
	SAFE_RELEASE(pCompileVS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���_�C���v�b�g���C�A�E�g�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//�s�N�Z���V�F�[�_�쐬(�R���p�C��)
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Engine//Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_3D].pPixelShader_);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �s�N�Z���V�F�[�_�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK; //���p�`�̗����͕`�悵�Ȃ�(�J�����O)
	rdc.FillMode = D3D11_FILL_SOLID; //���p�`�̓�����h��Ԃ�()
	//rdc.FillMode = D3D11_FILL_WIREFRAME; //���C���[�t���[���\��
	rdc.FrontCounterClockwise = FALSE; //�����v����\�ɂ���A��FALSE=���v��肪�\
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_TYPE::SHADER_3D].pRasterizerState_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���X�^���C�U�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}
	//--------3D�p�I��-----------------------------------------------
	return S_OK;
}

HRESULT Direct3D::InitPointShader()
{
	//--------�_�����p�J�n-----------------------------------------------
	HRESULT hr;

	//���_�V�F�[�_�̍쐬(�R���p�C��)
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Engine//PointShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_POINT].pVertexShader_);
	//�G���[��������v���O�����I��
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���_�V�F�[�_�̍쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g�쐬->���_��񂾂���n���Ă���
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, //�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 }, //UV���W
		{ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },//�@��
	};
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateInputLayout(layout, 3, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_TYPE::SHADER_POINT].pVertexLayout_);
	SAFE_RELEASE(pCompileVS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���_�C���v�b�g���C�A�E�g�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//�s�N�Z���V�F�[�_�쐬(�R���p�C��)
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Engine//PointShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_POINT].pPixelShader_);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �s�N�Z���V�F�[�_�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK; //���p�`�̗����͕`�悵�Ȃ�(�J�����O)
	rdc.FillMode = D3D11_FILL_SOLID; //���p�`�̓�����h��Ԃ�()
	//rdc.FillMode = D3D11_FILL_WIREFRAME; //���C���[�t���[���\��
	rdc.FrontCounterClockwise = FALSE; //�����v����\�ɂ���A��FALSE=���v��肪�\
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_TYPE::SHADER_POINT].pRasterizerState_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���X�^���C�U�쐬", L"Direct3D.cpp", MB_OK);
		return hr;
	}
	//--------�_�����p�I��-----------------------------------------------
	return S_OK;
}

HRESULT Direct3D::InitShader()
{
	if (FAILED(InitShader2D()))
	{
		return E_FAIL;
	}
	if (FAILED(InitShader3D()))
	{
		return E_FAIL;
	}
	if (FAILED(InitPointShader()))
	{
		return E_FAIL;
	}
	return S_OK;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
	//���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g
	pContext->VSSetShader(shaderBundle[type].pVertexShader_, NULL, 0); //���_�V�F�[�_�[
	pContext->PSSetShader(shaderBundle[type].pPixelShader_, NULL, 0); //�s�N�Z���V�F�[�_�[
	pContext->IASetInputLayout(shaderBundle[type].pVertexLayout_); //���_�C���v�b�g���C�A�E�g
	pContext->RSSetState(shaderBundle[type].pRasterizerState_); //���X�^���C�U�[
}

void Direct3D::BeginDraw()
{
	//�w�i�̐F
	float clearColor[4] = { 0.94f,0.97f,1.0f, 1.0f }; //R,G,B,A

	//��ʂ��N���A �f�o�C�X�R���e�L�X�g(�G��`���l)�̏��d��
	pContext->ClearRenderTargetView(pRenderTargetView, clearColor);

	//�[�x�o�b�t�@�N���A
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndDraw()
{
	//�X���b�v(�o�b�N�o�b�t�@��\�ɕ\������)
	pSwapChain->Present(0, 0);
}

void Direct3D::Release()
{
	//�V�F�[�_�[�̉��
	SAFE_RELEASE(pRasterizerState);
	SAFE_RELEASE(pVertexLayout);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(pVertexShader);

	//�`��ɕK�v�ȃI�u�W�F�N�g�̉��
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pContext);
	SAFE_RELEASE(pDevice);

	//Z�o�b�t�@�֘A�̉��
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pDepthStencilView);
}

XMFLOAT4 Direct3D::GetLightPos()
{
	return lightPos;
}

void Direct3D::SetLightPos(XMFLOAT4 pos)
{
	lightPos = pos;
}
