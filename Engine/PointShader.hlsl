
//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer gModel : register(b0)
{
    //�ϊ��s��A���_�A�����̏��Ȃǂ��܂Ƃ߂đ���
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //���[���h�ϊ��}�g���N�X
    float4x4 matNormal; //�@�������[���h���W��
    float4 diffuseColor; //�}�e���A���̐F(�g�U���ˌW��)
    float4 lightVec; //����Ȃ��Ƃ���������
    float4 factor; //�}�e���A���̌��̋��x
    float4 ambientColor; //����H
    float4 specularColor; //���̂̂���H
    float4 shininess; //
    int isTextured; //�e�N�X�`�����\���Ă��邩
};

cbuffer gStage : register(b1)
{
    float4 lightPosition; //�_�����̕����x�N�g��
    float4 eyePosition; //���_
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 wpos : POSITION0; //�ʒu
    float4 pos : SV_POSITION; //�ʒu :����n�܂��̓Z�}���e�B�N�X
    float2 uv : TEXCOORD; //UV���W 
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
   //float4 cos_alpha : COLOR; //�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[�� 
    float4 spos = mul(pos, matWVP);
    float4 wpos = mul(pos, matW);
    float4 wnormal = mul(normal, matNormal);
    outData.pos = spos;
    outData.wpos = wpos;
    outData.uv = uv.xy;
    outData.normal = wnormal;
    outData.eyev = eyePosition - wpos;

	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    float4 diffuse, ambient; //�g�U���ˌ�
    float4 ambientSource = { 0.5f, 0.5f, 0.5f, 1.0f }; //����
    float3 dir = normalize(lightPosition.xyz - inData.wpos.xyz); //�s�N�Z���ʒu�̃|���S���̎O�������W
    float color = saturate(dot(normalize(inData.normal.xyz), dir));
    float3 k = { 0.2f, 0.2f, 1.0f };
    float len = length(lightPosition.xyz - inData.wpos.xyz);
    float dTerm = 1.0 / (k.x + k.y * len + k.z * len * len); //����
    
    float4 R = reflect(normalize(inData.normal), normalize(float4(dir, 1.0f)));
    float4 specular = pow(saturate(dot(R, normalize(inData.eyev))), shininess) * specularColor;
 
    
    //�e�N�X�`������
    if (isTextured == false)
    {
        diffuse = diffuseColor * color * dTerm * factor.x;
        ambient = diffuseColor * ambientSource;
    }
    //�e�N�X�`���Ȃ�
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * color * dTerm * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource;
    }
    return diffuse + specular + ambient;
}
