//���C�g����݂�Z�l�ƃJ��������݂�Z�l���r���āA�e�ɂȂ镔�����Â����鏈�����s��

float4x4 WVP;		// ���[���h���W�n �~ �r���[(�J�����)���W�n �~ ���ˉe���W�n
float4x4 LWVP;		// ���[���h���W�n �~ �r���[(���C�g�)���W�n �~ ���ˉe���W�n
float4x4 LWVPT;		// ���[���h���W�n �~ �r���[(���C�g�)���W�n �~ ���ˉe���W�n �~ �e�N�X�`���[���W�n
float4 Color;		//�J���[

float    Bias = 0.0f;          // Z�l�̔�r�ɂ��덷��␳����
float    ShadowColor = 0.1f;   // �e�ɂ�郁�b�V���̃J���[�̌��Z�l

float4   LightDir;		//���C�g�̕���
float4   Ambient = 0.0f;	//�A���r�G���g�F

sampler tex0 : register(s0);     // �I�u�W�F�N�g�̃e�N�X�`���[
sampler tex1 : register(s1);     // �f�v�X�o�b�t�@�T�[�t�F�C�X�̃e�N�X�`���[

// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
   float4 Pos       : POSITION;		//���W
   float4 Col       : COLOR0;		//�J���[
   float2 Tex       : TEXCOORD0;
   float2 Depth     : TEXCOORD1;	//���ˉe���W�n�ɕϊ��������_���W
   float4 LightUV   : TEXCOORD2;	//�e�N�X�`���[���W�n�ɕϊ��������_���W
};

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT VS( float4 Pos     : POSITION,
              float4 Normal  : NORMAL,
              float2 Tex     : TEXCOORD0 )
{
   VS_OUTPUT Out;

   Out.Pos    = mul( Pos, WVP );
   Out.Tex    = Tex;

   // �����o�[�g�g�U�Ɩ��ɂ�郁�b�V���̐F�����v�Z����
   float3 L = -LightDir.xyz;
   float3 N = normalize( Normal.xyz );
   Out.Col = max( Ambient, dot(N, L) );

   // ���ˉe���W�n�ɕϊ�����Z�l���v�Z
   Out.Depth.xy   = mul(Pos, LWVP).zw;

   // �e�N�X�`���[���W�n�ɕϊ��������_���W���v�Z
   Out.LightUV = mul(Pos, LWVPT);

   return Out;
}

// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS( VS_OUTPUT In ) : COLOR0
{  
   float4 Out = tex2D( tex0, In.Tex );
   float3 ShadowColor;
      
   //Z�o�b�t�@�T�[�t�F�C�X����[�x�l���擾����
   float d = tex2Dproj( tex1, In.LightUV ).r;

   //�A�Ɖe���Ȃ߂炩�ɂ��邽�߂ɉA�Ɖe�̗����Ō��Z�����A�Â����Ō��Z����悤�ɂ���
   if( d * In.Depth.y < In.Depth.x - Bias )
      ShadowColor = min( ( In.Col.r + In.Col.g + In.Col.b ) * 0.3333f, ShadowColor );
   else
      ShadowColor = In.Col.rgb;
   
   Out.rgb *= ShadowColor;
   
   return Out;
}

// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader
{
   pass P0
   {
      VertexShader = compile vs_1_1 VS();
      PixelShader  = compile ps_2_0 PS();
   }     
}
