//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    //変換行列、視点、光源の情報などをまとめて送る
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //ワールド変換マトリクス
    float4x4 matNormal; //法線をワールド座標に
    float4 diffuseColor; //マテリアルの色(拡散反射係数)
    float4 lightVec; //いらないとおもうこれ
    float4 factor; //マテリアルの光の強度
    float4 ambientColor; //いろ？
    float4 specularColor; //物体のいろ？
    float4 shininess; //
    int isTextured; //テクスチャが貼られているか

};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置 :から始まるやつはセマンティクス
    float2 uv : TEXCOORD; //UV座標 
    float4 cos_alpha : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    
    //法線を回転
    normal = mul(normal, matW);
    
    float4 light = lightVec;
    light = normalize(light);
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);

	//まとめて出力
    return outData;

}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{  
    float4 ambientSource = float4(0.5f, 0.5f, 0.5f, 1.0); //環境光
    float4 diffuse, ambient; //拡散反射光
    
    //テクスチャあり
    if (isTextured == false)
    {
        diffuse = diffuseColor * inData.cos_alpha * factor.x;
        ambient = diffuseColor * ambientSource * factor.x;
    }
    //テクスチャなし
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.cos_alpha * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource * factor.x;
    }
    return diffuse + ambient;
}

