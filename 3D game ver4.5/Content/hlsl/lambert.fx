//メッシュをライト位置からながめて、Z値を取得する処理を行う

float4x4 WVP;		// ワールド座標系 × ビュー(ライト基準) × 正射影座標系
//float4x4 WVPT;	// ワールド座標系 × ビュー(ライト基準) × 正射影座標系 × テクスチャー座標系
float4 Color;		//カラー

sampler tex0 : register(s0); // メッシュのテクスチャー
//sampler tex1 : register(s1); // デプスバッファサーフェイスのテクスチャー

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 Pos     : POSITION;
   float2 Depth   : TEXCOORD0;
};

//----------------------------------------------------------------
// カラーキーを考慮しない場合のメッシュのZ値を取得するシェーダー
//----------------------------------------------------------------
// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS( float4 Pos     : POSITION,
              float4 Normal  : NORMAL,
              float2 Tex     : TEXCOORD0 )
{
   VS_OUTPUT Out;
   
   Out.Pos    = mul( Pos, WVP );
   Out.Depth.xy = Out.Pos.zw;
      
   return Out;
}

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS( VS_OUTPUT In ) : COLOR0
{  
   float4 Out;
   
   Out = In.Depth.x / In.Depth.y;
   
   return Out;
}

// -------------------------------------------------------------
// カラーキーを考慮する場合のメッシュのZ値を取得するシェーダー（注意１）
// -------------------------------------------------------------

//アルファブレンドによりマスク処理するように変更する。
/*
struct VS_OUTPUT1
{
   float4 Pos     : POSITION;
   float2 Tex     : TEXCOORD0;
   float2 Depth   : TEXCOORD1;
   float4 DepthUV : TEXCOORD2;   //デプスバッファのテクセル座標
};
VS_OUTPUT1 VS1( float4 Pos     : POSITION,
                float4 Normal  : NORMAL,
                float2 Tex     : TEXCOORD0 )
{
   VS_OUTPUT1 Out;
   
   Out.Pos      = mul( Pos, WVP );
   Out.Tex      = Tex;
   Out.Depth.xy = Out.Pos.zw;
   
   // デプスバッファの深度情報を取得するためにテクセル座標をセット
   Out.DepthUV  = mul( Pos, WVPT );   
   
   return Out;
}

float4 PS1( VS_OUTPUT1 In ) : COLOR0
{  
   float4 Out;
   
   //レンダリングするピクセル位置に対応するデプスバッファ上のテクセルの位置のカラー情報を取得する
   float4 ZMapColor = tex2Dproj( tex1, In.DepthUV );
   float z = In.Depth.x / In.Depth.y;
   
   //メッシュのテクスチャーのα値が0より大きいときは、メッシュのZ値を出力する
   if( tex2D( tex0, In.Tex ).a > 0.0f )
   {
      //デプスバッファのZ値 > メッシュのZ値 のときはメッシュのZ値を出力する
      if( ZMapColor.r > z )
         Out = z;
      else
         Out = ZMapColor;
   }

   //メッシュのテクスチャーのα値が0のときは、デプスバッファのZ値を出力する
   else
      Out = ZMapColor;
   
   return Out;
}
*/

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS1_OUTPUT
{
   float4 Pos     : POSITION;
   float2 Tex     : TEXCOORD0;  //テクセル
   float2 Depth   : TEXCOORD1;
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS1_OUTPUT VS1( float4 Pos     : POSITION,
                float4 Normal  : NORMAL,
                float2 Tex     : TEXCOORD0 )
{
   VS1_OUTPUT Out;
   
   Out.Pos      = mul( Pos, WVP );
   Out.Tex      = Tex;
   Out.Depth.xy = Out.Pos.zw;
      
   return Out;
}

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS1( VS1_OUTPUT In ) : COLOR0
{
   float4 Out;

   //RGB成分にはZ値を出力する
   Out.rgb = In.Depth.x / In.Depth.y;

   //A成分にはアルファブレンドの線形合成で使用するアルファ値を出力する
   Out.a = tex2D( tex0, In.Tex ).a;
   
   return Out;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{
   //カラーキーを考慮しない
   pass P0
   {
      VertexShader = compile vs_1_1 VS();
      PixelShader  = compile ps_2_0 PS();
   }     

   //カラーキーを考慮する
   pass P1
   {
      VertexShader = compile vs_1_1 VS1();
      PixelShader  = compile ps_2_0 PS1();
   }     
}
