//ライトからみたZ値とカメラからみたZ値を比較して、影になる部分を暗くする処理を行う

float4x4 WVP;		// ワールド座標系 × ビュー(カメラ基準)座標系 × 正射影座標系
float4x4 LWVP;		// ワールド座標系 × ビュー(ライト基準)座標系 × 正射影座標系
float4x4 LWVPT;		// ワールド座標系 × ビュー(ライト基準)座標系 × 正射影座標系 × テクスチャー座標系
float4 Color;		//カラー

float    Bias = 0.0f;          // Z値の比較による誤差を補正する
float    ShadowColor = 0.1f;   // 影によるメッシュのカラーの減算値

float4   LightDir;		//ライトの方向
float4   Ambient = 0.0f;	//アンビエント色

sampler tex0 : register(s0);     // オブジェクトのテクスチャー
sampler tex1 : register(s1);     // デプスバッファサーフェイスのテクスチャー

// -------------------------------------------------------------
// 頂点シェーダからピクセルシェーダに渡すデータ
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 Pos       : POSITION;		//座標
   float4 Col       : COLOR0;		//カラー
   float2 Tex       : TEXCOORD0;
   float2 Depth     : TEXCOORD1;	//正射影座標系に変換した頂点座標
   float4 LightUV   : TEXCOORD2;	//テクスチャー座標系に変換した頂点座標
};

// -------------------------------------------------------------
// 頂点シェーダプログラム
// -------------------------------------------------------------
VS_OUTPUT VS( float4 Pos     : POSITION,
              float4 Normal  : NORMAL,
              float2 Tex     : TEXCOORD0 )
{
   VS_OUTPUT Out;

   Out.Pos    = mul( Pos, WVP );
   Out.Tex    = Tex;

   // ランバート拡散照明によるメッシュの色情報を計算する
   float3 L = -LightDir.xyz;
   float3 N = normalize( Normal.xyz );
   Out.Col = max( Ambient, dot(N, L) );

   // 正射影座標系に変換したZ値を計算
   Out.Depth.xy   = mul(Pos, LWVP).zw;

   // テクスチャー座標系に変換した頂点座標を計算
   Out.LightUV = mul(Pos, LWVPT);

   return Out;
}

// -------------------------------------------------------------
// ピクセルシェーダプログラム
// -------------------------------------------------------------
float4 PS( VS_OUTPUT In ) : COLOR0
{  
   float4 Out = tex2D( tex0, In.Tex );
   float3 ShadowColor;
      
   //Zバッファサーフェイスから深度値を取得する
   float d = tex2Dproj( tex1, In.LightUV ).r;

   //陰と影をなめらかにするために陰と影の両方で減算せず、暗い方で減算するようにする
   if( d * In.Depth.y < In.Depth.x - Bias )
      ShadowColor = min( ( In.Col.r + In.Col.g + In.Col.b ) * 0.3333f, ShadowColor );
   else
      ShadowColor = In.Col.rgb;
   
   Out.rgb *= ShadowColor;
   
   return Out;
}

// -------------------------------------------------------------
// テクニック
// -------------------------------------------------------------
technique TShader
{
   pass P0
   {
      VertexShader = compile vs_1_1 VS();
      PixelShader  = compile ps_2_0 PS();
   }     
}
