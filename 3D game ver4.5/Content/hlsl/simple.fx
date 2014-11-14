//ワールド x ビュー x プロジェクション 行列
float4x4 WVP;

//カラー
float4 Color;

// -------------------------------------------------------------
// テクスチャ
// -------------------------------------------------------------
texture SrcTex;
//テクスチャのサンプリング方法
sampler SrcSamp = sampler_state
{
    Texture = <SrcTex>;		//対象のテクスチャ(外部から受け取る)
    MinFilter = LINEAR;		//縮小時のサンプリング(LINEAR→線形補完)
    MagFilter = LINEAR;		//拡大時
    MipFilter = NONE;		//ミップマップ

	//テクスチャアドレッシングモード
    AddressU = Clamp;		//(Clanp→0～1以外の座標の時に端っこのピクセルを引き伸ばす)
    AddressV = Clamp;
};

//------------頂点シェーダ出力セマンティクス------------
//セマンティクス	意味
//POSITION			スクリーン空間に変換した頂点の座標
//PSIZE				ポイントサイズ
//FOG				頂点フォグ
//COLOR				頂点の色
//TEXCOORD#			テクスチャ座標

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;		//位置座標
	float2 Tex			: TEXCOORD0;	//テクスチャー座標
};

//------------頂点シェーダ入力セマンティクス------------
//セマンティクス	意味
//POSITION			ローカル空間の頂点の位置座標
//BLENDWEIGHT		ブレンディングの重み
//BLENDINDICES		ブレンドのインデックス
//NORMAL			法線ベクトル
//PSIZE				ポイントサイズ
//DIFFUSE			ディフューズ色
//SPECULAR			スペキュラ色
//TEXCOORD#			テクスチャ座標
//TANGENT			接線
//BINORMAL			従法線
//TESSFACTOR		テセレーション係数

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS_pass1 (
      float4 Pos    : POSITION,          //モデルの頂点の位置座標
      float4 Tex    : TEXCOORD0	         //テクスチャ座標
){

	//出力データ
    VS_OUTPUT Out = (VS_OUTPUT)0;
    
    //行列の掛け算
    //位置座標(スクリーン座標値が入る)
    Out.Pos = mul( Pos, WVP );

	//テクスチャ
    Out.Tex = Tex;
    
    return Out;
}

//------ピクセルシェーダ入力&出力セマンティクス------
//セマンティクス		意味
//COLOR#				ディフューズ色・スペキュラ色
//TEXCOORD#				テクスチャ座標

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS_pass1( VS_OUTPUT In ) : COLOR
{   
    float4 OutColor;
    //2Dテクスチャーをサンプリング
	OutColor = tex2D( SrcSamp, In.Tex );
	//カラーを合成
	OutColor *= Color;

    return OutColor;
}

float4 PS_pass2( VS_OUTPUT In ) : COLOR
{   
    float4 OutColor;
	OutColor = Color;
	//カラーを合成
	OutColor *= Color;
    return OutColor;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{
	//通常合成
    pass Normal
    {
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

        //頂点シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        //ピクセルシェーダ
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }

	//加算合成
    pass Add
    {
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = ONE;

        //頂点シェーダ
        VertexShader = compile vs_1_1 VS_pass1();
        //ピクセルシェーダ
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
