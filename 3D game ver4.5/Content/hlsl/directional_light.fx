// ���[���h x �r���[ x �v���W�F�N�V���� �s��
float4x4 WVP ;

// �J���[
float4 Color ;

// ��]�s��
float4x4 RM ;

// ���s�����̌���
float4 LightDir ;

//�A���r�G���g�F(0.0f�`1.0f�̒l�������A1.0f�̂Ƃ��ł������Ȃ�B)
float Ambient;

// -------------------------------------------------------------
// �e�N�X�`��
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
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;
	float4 Col			: COLOR0;
	float2 Tex			: TEXCOORD0;
};

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT VS_pass1 (
      float4 Pos    : POSITION,          // ���f���̒��_
	  float4 Normal	: NORMAL,			 //���f���̖@��
      float4 Tex    : TEXCOORD0	         // �e�N�X�`�����W
){

    VS_OUTPUT Out = (VS_OUTPUT)0;        // �o�̓f�[�^
    
    // �ʒu���Wd
    Out.Pos = mul( Pos, WVP ) ;
    
    //�e�N�X�`��
    Out.Tex = Tex ;

	//�J���[
	float4 nml = mul( Normal, RM ) ;
	float d = -dot( LightDir, nml ) ;
	float C = max( Ambient , d );

	Out.Col = float4( C, C, C, 1 ) ;
   
    return Out;
}

// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
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
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader
{

	// �ʏ퍇��
    pass Normal
    {
		AlphaBlendEnable = true ;
		SrcBlend = SRCALPHA ;
		DestBlend = INVSRCALPHA ;

        // �V�F�[�_
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }

	// ���Z����
    pass Add
    {
		AlphaBlendEnable = true ;
		SrcBlend = ONE ;
		DestBlend = ONE ;

        // �V�F�[�_
        VertexShader = compile vs_1_1 VS_pass1();
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }
    
    //�P�F
    pass SolidColor
    {
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

        //���_�V�F�[�_
        VertexShader = compile vs_1_1 VS_pass1();
        //�s�N�Z���V�F�[�_
        PixelShader  = compile ps_2_0 PS_pass2();
		Sampler[0] = (SrcSamp);
    }

}
