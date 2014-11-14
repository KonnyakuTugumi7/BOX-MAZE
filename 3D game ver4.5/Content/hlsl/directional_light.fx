// ワールド x ビュー x プロジェクション 行列
float4x4 WVP ;

// カラー
float4 Color ;

// 回転行列
float4x4 RM ;

// 平行光源の向き
float4 LightDir ;

//アンビエント色(0.0f～1.0fの値を持ち、1.0fのとき最も強くなる。)
float Ambient;

// -------------------------------------------------------------
// テクスチャ
// -------------------------------------------------------------
texture SrcTex ;

sampler SrcSamp = sampler_state
{
    Texture = <SrcTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
	float4 Col			: COLOR0;
	float2 Tex			: TEXCOORD0;
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS_pass1 (
      float4 Pos    : POSITION,          // モデルの頂点
	  float4 Normal	: NORMAL,			 //モデルの法線
      float4 Tex    : TEXCOORD0	         // テクスチャ座標
){

    VS_OUTPUT Out = (VS_OUTPUT)0;        // 出力データ
    
    // 位置座標d
    Out.Pos = mul( Pos, WVP ) ;
    
    //テクスチャ
    Out.Tex = Tex ;

	//カラー
	float4 nml = mul( Normal, RM ) ;
	float d = -dot( LightDir, nml ) ;
	float C = max( Ambient , d );

	Out.Col = float4( C, C, C, 1 ) ;
   
    return Out;
}

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS_pass1( VS_OUTPUT In ) : COLOR
{   
    float4 Color;

	Color = tex2D( SrcSamp, In.Tex ) ;
	Color *= In.Col ;
;
    return Color;
}

float4 PS_pass2( VS_OUTPUT In ) : COLOR
{   
    float4 OutColor;
	OutColor = Color;
    return OutColor;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{

	// 通常合成
    pass Normal
    {
		AlphaBlendEnable = true ;
		SrcBlend = SRCALPHA ;
		DestBlend = INVSRCALPHA ;

        // シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }

	// 加算合成
    pass Add
    {
		AlphaBlendEnable = true ;
		SrcBlend = ONE ;
		DestBlend = ONE ;

        // シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }
    
    //単色
    pass SolidColor
    {
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

        //頂点シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        //ピクセルシェーダ
        PixelShader  = compile ps_2_0 PS_pass2();
		Sampler[0] = (SrcSamp);
    }

}
