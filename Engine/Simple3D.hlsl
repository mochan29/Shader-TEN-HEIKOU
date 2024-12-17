//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
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

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu :����n�܂��̓Z�}���e�B�N�X
    float2 uv : TEXCOORD; //UV���W 
    float4 cos_alpha : COLOR; //�F�i���邳�j
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
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    
    //�@������]
    normal = mul(normal, matW);
    
    float4 light = lightVec;
    light = normalize(light);
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);

	//�܂Ƃ߂ďo��
    return outData;

}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{  
    float4 ambientSource = float4(0.5f, 0.5f, 0.5f, 1.0); //����
    float4 diffuse, ambient; //�g�U���ˌ�
    
    //�e�N�X�`������
    if (isTextured == false)
    {
        diffuse = diffuseColor * inData.cos_alpha * factor.x;
        ambient = diffuseColor * ambientSource * factor.x;
    }
    //�e�N�X�`���Ȃ�
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.cos_alpha * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource * factor.x;
    }
    return diffuse + ambient;
}

