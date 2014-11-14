//���[���h x �r���[ x �v���W�F�N�V���� �s��
float4x4 WVP;

//�J���[
float4 Color;

// -------------------------------------------------------------
// �e�N�X�`��
// -------------------------------------------------------------
texture SrcTex;
//�e�N�X�`���̃T���v�����O���@
sampler SrcSamp = sampler_state
{
    Texture = <SrcTex>;		//�Ώۂ̃e�N�X�`��(�O������󂯎��)
    MinFilter = LINEAR;		//�k�����̃T���v�����O(LINEAR�����`�⊮)
    MagFilter = LINEAR;		//�g�厞
    MipFilter = NONE;		//�~�b�v�}�b�v

	//�e�N�X�`���A�h���b�V���O���[�h
    AddressU = Clamp;		//(Clanp��0�`1�ȊO�̍��W�̎��ɒ[�����̃s�N�Z���������L�΂�)
    AddressV = Clamp;
};

//------------���_�V�F�[�_�o�̓Z�}���e�B�N�X------------
//�Z�}���e�B�N�X	�Ӗ�
//POSITION			�X�N���[����Ԃɕϊ��������_�̍��W
//PSIZE				�|�C���g�T�C�Y
//FOG				���_�t�H�O
//COLOR				���_�̐F
//TEXCOORD#			�e�N�X�`�����W

// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos			: POSITION;		//�ʒu���W
	float2 Tex			: TEXCOORD0;	//�e�N�X�`���[���W
};

//------------���_�V�F�[�_���̓Z�}���e�B�N�X------------
//�Z�}���e�B�N�X	�Ӗ�
//POSITION			���[�J����Ԃ̒��_�̈ʒu���W
//BLENDWEIGHT		�u�����f�B���O�̏d��
//BLENDINDICES		�u�����h�̃C���f�b�N�X
//NORMAL			�@���x�N�g��
//PSIZE				�|�C���g�T�C�Y
//DIFFUSE			�f�B�t���[�Y�F
//SPECULAR			�X�y�L�����F
//TEXCOORD#			�e�N�X�`�����W
//TANGENT			�ڐ�
//BINORMAL			�]�@��
//TESSFACTOR		�e�Z���[�V�����W��

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT VS_pass1 (
      float4 Pos    : POSITION,          //���f���̒��_�̈ʒu���W
      float4 Tex    : TEXCOORD0	         //�e�N�X�`�����W
){

	//�o�̓f�[�^
    VS_OUTPUT Out = (VS_OUTPUT)0;
    
    //�s��̊|���Z
    //�ʒu���W(�X�N���[�����W�l������)
    Out.Pos = mul( Pos, WVP );

	//�e�N�X�`��
    Out.Tex = Tex;
    
    return Out;
}

//------�s�N�Z���V�F�[�_����&�o�̓Z�}���e�B�N�X------
//�Z�}���e�B�N�X		�Ӗ�
//COLOR#				�f�B�t���[�Y�F�E�X�y�L�����F
//TEXCOORD#				�e�N�X�`�����W

// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS_pass1( VS_OUTPUT In ) : COLOR
{   
    float4 OutColor;
    //2D�e�N�X�`���[���T���v�����O
	OutColor = tex2D( SrcSamp, In.Tex );
	//�J���[������
	OutColor *= Color;

    return OutColor;
}

float4 PS_pass2( VS_OUTPUT In ) : COLOR
{   
    float4 OutColor;
	OutColor = Color;
	//�J���[������
	OutColor *= Color;
    return OutColor;
}

// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique TShader
{
	//�ʏ퍇��
    pass Normal
    {
		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

        //���_�V�F�[�_
        VertexShader = compile vs_1_1 VS_pass1();
        //�s�N�Z���V�F�[�_
        PixelShader  = compile ps_2_0 PS_pass1();
		Sampler[0] = (SrcSamp);
    }

	//���Z����
    pass Add
    {
		AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = ONE;

        //���_�V�F�[�_
        VertexShader = compile vs_1_1 VS_pass1();
        //�s�N�Z���V�F�[�_
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
