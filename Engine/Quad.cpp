#include "Quad.h"
#include "Camera.h"
#include "Define.h"

Quad::Quad()
{
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	indexNum_ = 0;
	vertexNum_ = 0;
	pTexture_ = nullptr;
}

Quad::~Quad()
{
	Release();
}

HRESULT Quad::Initialize()
{
	//���_���/���_�o�b�t�@�쐬
	InitVertexData();
	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(nullptr, L"���s: ���_�o�b�t�@", L"Sprite.cpp", MB_OK);
		return E_FAIL;
	}

	//�C���f�b�N�X���/�C���f�b�N�X�o�b�t�@�쐬
	InitIndexData();
	if (FAILED(CreateIndexBuffer()))
	{
		MessageBox(nullptr, L"���s: �C���f�b�N�X�o�b�t�@", L"Quad.cpp", MB_OK);
		return E_FAIL;
	}

	//�R���X�^���g�o�b�t�@�쐬
	if (FAILED(CreateConstantBuffer()))
	{
		MessageBox(nullptr, L"���s: �R���X�^���g�o�b�t�@", L"Quad.cpp", MB_OK);
		return E_FAIL;
	}

	//�e�N�X�`���̃��[�h
	if (FAILED(LoadTexture()))
	{
		MessageBox(nullptr, L"���s: �e�N�X�`���̃��[�h", L"Quad.cpp", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

void Quad::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);
	transform.Calculation();
	PassDataToCB(transform);
	SetBufferToPipeline();
	Direct3D::pContext->DrawIndexed(index_.size(), 0, 0);
}

void Quad::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_DELETE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Quad::InitVertexData()
{
	//���_�f�[�^
	vertices_=
	{ //���_�AUV�A�@��
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) }, //����
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) }, //�E��
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) }, //�E��
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f) }, //����
	};
	vertexNum_ = vertices_.size();
}

HRESULT Quad::CreateVertexBuffer()
{
	HRESULT hr;
	//���_�f�[�^�p�o�b�t�@�ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexNum_; //�o�b�t�@�̃T�C�Y(byte)
	bd_vertex.Usage = D3D11_USAGE_DEFAULT; //�ǂݍ��݁E�������݂̕��@
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER; //�o�C���h�Ɋւ�����
	bd_vertex.CPUAccessFlags = 0; //CPU�A�N�Z�X�Ɋւ�����
	bd_vertex.MiscFlags = 0; //�����ȃt���O
	bd_vertex.StructureByteStride = 0; //�\���̂̃T�C�Y(byte)
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: ���_�o�b�t�@", L"Quad.cpp", MB_OK);
		return hr;
	}
	return S_OK;
}

void Quad::InitIndexData()
{
	//�C���f�b�N�X���
	index_ = { 0,2,3,0,1,2, };
	indexNum_ = index_.size();
}

HRESULT Quad::CreateIndexBuffer()
{
	HRESULT hr;
	//�C���f�b�N�X�o�b�t�@����
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �C���f�b�N�X�o�b�t�@", L"Quad.cpp", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Quad::CreateConstantBuffer()
{
	HRESULT hr;
	//�R���X�^���g�o�b�t�@�ݒ�
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	//�R���X�^���g�o�b�t�@�쐬
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: �R���X�^���g�o�b�t�@", L"Quad.cpp", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Quad::LoadTexture()
{
	//�e�N�X�`������
	pTexture_ = new Texture;
	if (FAILED(pTexture_->Load("Assets/Images/neko.png")))
	{
		MessageBox(NULL, L"�e�N�X�`���̍쐬�Ɏ��s���܂���", L"Quad.cpp", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void Quad::PassDataToCB(Transform& transform)
{
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
	cb.winSize =Direct3D::WindowSize;
	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); //GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); //�f�[�^��l�𑗂�
	Direct3D::pContext->Unmap(pConstantBuffer_, 0); //�ĊJ
}

void Quad::SetBufferToPipeline()
{
	//���_�o�b�t�@�Z�b�g
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�[�Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@�Z�b�g
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_); //���_�V�F�[�_�[�p	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_); //�s�N�Z���V�F�[�_�[�p

	//�T���v���[�ƃV�F�[�_�[���\�[�X�r���[���V�F�[�_�ɃZ�b�g
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);
	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
}


