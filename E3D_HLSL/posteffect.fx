// Copyright (c) 2009 ochakko lab http://www5d.biglobe.ne.jp/~ochikko  All rights reserved.

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// このファイルは自由に改変してかまいませんが改変した場合はサポート外です。自分でデバッグしてください。
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

float WIDTH0;
float HEIGHT0;
float WIDTH1;
float HEIGHT1;

//Gauss
float  weight[8];
float2 offsetX;
float2 offsetY;
float colmult;


// The per-color weighting to be used for luminance calculations in RGB order.
static const float3 LUMINANCE_VECTOR  = float3(0.2125f, 0.7154f, 0.0721f);


// Tone mapping variables
float  fMiddleGray;       // The middle gray key value
float  fBrightPassThreshold;  // Threshold for BrightPass filter
float  fBrightPassOffset; // Offset for BrightPass filter


//YCbCr
float3 RGB2Y = float3( 0.29900f, 0.58700f, 0.11400f );
float3 Cb2RGB = float3( 0.0f, -0.34414f, 1.77200f );
float3 Cr2RGB = float3( 1.40200f, -0.71414f, 0.0f );

float2 CbCr;

//////////////////////////
// texture
//////////////////////////
texture Texture0;
sampler Samp0Linear = sampler_state
{
    Texture = <Texture0>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};
sampler Samp0Point = sampler_state
{
	Texture = <Texture0>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

texture Texture1;
sampler Samp1Linear = sampler_state
{
    Texture = <Texture1>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};
sampler Samp1Point = sampler_state
{
	Texture = <Texture1>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

texture Texture2;
sampler Samp2Linear = sampler_state
{
    Texture = <Texture2>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};
sampler Samp2Point = sampler_state
{
	Texture = <Texture2>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};

/////////////////////////////////
//// user命令から設定できる定数
/////////////////////////////////
float2 avSampleOffsets[16];	// サンプリングの位置
float4 avSampleWeights[16];	// サンプリングの重み

float4 userFL4_0;
float4 userFL4_1;
float4 userFL4_2;
float4 userFL4_3;
float4 userFL4_4;
float4 userFL4_5;
float4 userFL4_6;
float4 userFL4_7;
float4 userFL4_8;
float4 userFL4_9;
	
texture usertex0;
texture usertex1;
texture usertex2;
texture usertex3;
texture usertex4;

	//samplerは自分で定義してください。
    //定義例
sampler SampUser0Point = sampler_state
{
	Texture = <usertex0>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
    AddressU = Clamp;
    AddressV = Clamp;
};


///////////////////////////////////////////////////////

struct VSTEX1_OUTPUT
{
	float4 Pos			: POSITION;
	float2 Tex0			: TEXCOORD0;
};

struct VSTEX4_OUTPUT
{
    float4 Pos			: POSITION;
	float2 Tex0			: TEXCOORD0;
	float2 Tex1			: TEXCOORD1;
	float2 Tex2			: TEXCOORD2;
	float2 Tex3			: TEXCOORD3;
};

struct VSTEX8_OUTPUT
{
    float4 Pos            : POSITION;
    float2 Tex0            : TEXCOORD0;
    float2 Tex1            : TEXCOORD1;
    float2 Tex2            : TEXCOORD2;
    float2 Tex3            : TEXCOORD3;
    float2 Tex4            : TEXCOORD4;
    float2 Tex5            : TEXCOORD5;
    float2 Tex6            : TEXCOORD6;
    float2 Tex7            : TEXCOORD7;
};

struct VSTLV_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse : COLOR0;
    float2  Tex0    : TEXCOORD0;
};


struct PS_OUTPUT
{
	float4 Color	: COLOR0;
};

///////////////////////

// ------------------------------------------------------------
// 頂点シェーダプログラム
// ------------------------------------------------------------

VSTEX1_OUTPUT VS1Tex (
      float4 Pos    : POSITION           // モデルの頂点
     ,float2 Tex    : TEXCOORD0	         // テクスチャ座標
){
    VSTEX1_OUTPUT Out;
    
    Out.Pos = Pos;    
    Out.Tex0 = Tex;
    
    return Out;
}

VSTEX4_OUTPUT VS16Box (
      float4 Pos    : POSITION           // モデルの頂点
     ,float2 Tex    : TEXCOORD0	         // テクスチャ座標
){
    VSTEX4_OUTPUT Out;
    
    Out.Pos = Pos;
    
    Out.Tex0 = Tex + float2(-1.0f/WIDTH0, -1.0f/HEIGHT0);
    Out.Tex1 = Tex + float2(+1.0f/WIDTH0, -1.0f/HEIGHT0);
    Out.Tex2 = Tex + float2(-1.0f/WIDTH0, +1.0f/HEIGHT0);
    Out.Tex3 = Tex + float2(+1.0f/WIDTH0, +1.0f/HEIGHT0);
    
    return Out;
}

VSTEX4_OUTPUT VS9Corn (
      float4 Pos    : POSITION           // モデルの頂点
     ,float2 Tex   : TEXCOORD0	         // テクスチャ座標
){
    VSTEX4_OUTPUT Out;
    
    Out.Pos = Pos;
    
    Out.Tex0 = Tex + float2( 0.0f/WIDTH0,  0.0f/HEIGHT0);
    Out.Tex1 = Tex + float2(+1.0f/WIDTH0,  0.0f/HEIGHT0);
    Out.Tex2 = Tex + float2( 0.0f/WIDTH0, +1.0f/HEIGHT0);
    Out.Tex3 = Tex + float2(+1.0f/WIDTH0, +1.0f/HEIGHT0);
    
    return Out;
}

VSTEX8_OUTPUT VSGaussX (
      float4 Pos    : POSITION,          // モデルの頂点
      float2 Tex    : TEXCOORD0             // テクスチャ座標
){
    VSTEX8_OUTPUT Out;
    
    Out.Pos = Pos;
    
    Out.Tex0 = Tex + float2( - 1.0f/WIDTH0, 0.0f );
    Out.Tex1 = Tex + float2( - 3.0f/WIDTH0, 0.0f );
    Out.Tex2 = Tex + float2( - 5.0f/WIDTH0, 0.0f );
    Out.Tex3 = Tex + float2( - 7.0f/WIDTH0, 0.0f );
    Out.Tex4 = Tex + float2( - 9.0f/WIDTH0, 0.0f );
    Out.Tex5 = Tex + float2( -11.0f/WIDTH0, 0.0f );
    Out.Tex6 = Tex + float2( -13.0f/WIDTH0, 0.0f );
    Out.Tex7 = Tex + float2( -15.0f/WIDTH0, 0.0f );
    
    return Out;
}

VSTEX8_OUTPUT VSGaussY (
      float4 Pos    : POSITION,          // モデルの頂点
      float2 Tex    : TEXCOORD0             // テクスチャ座標
){
    VSTEX8_OUTPUT Out;
    
    Out.Pos = Pos;
    
    Out.Tex0 = Tex + float2( 0.0f, - 1.0f/HEIGHT0 );
    Out.Tex1 = Tex + float2( 0.0f, - 3.0f/HEIGHT0 );
    Out.Tex2 = Tex + float2( 0.0f, - 5.0f/HEIGHT0 );
    Out.Tex3 = Tex + float2( 0.0f, - 7.0f/HEIGHT0 );
    Out.Tex4 = Tex + float2( 0.0f, - 9.0f/HEIGHT0 );
    Out.Tex5 = Tex + float2( 0.0f, -11.0f/HEIGHT0 );
    Out.Tex6 = Tex + float2( 0.0f, -13.0f/HEIGHT0 );
    Out.Tex7 = Tex + float2( 0.0f, -15.0f/HEIGHT0 );
    
    return Out;
}

VSTLV_OUTPUT VSTLV (
      float4 Pos    : POSITION           // モデルの頂点
     ,float4 Diffuse : COLOR0
     ,float2 Tex    : TEXCOORD0	         // テクスチャ座標
){
    VSTLV_OUTPUT Out;
    
    Out.Pos = Pos;
    Out.Diffuse = Diffuse;
    Out.Tex0 = Tex;
    
    return Out;
}


// ------------------------------------------------------------
// ピクセルシェーダプログラム
// ------------------------------------------------------------

PS_OUTPUT PSav4( VSTEX4_OUTPUT i )
{
	PS_OUTPUT Output;
	
	float4 col0, col1, col2, col3;
	col0 = tex2D( Samp0Linear, i.Tex0 );
	col1 = tex2D( Samp0Linear, i.Tex1 );
	col2 = tex2D( Samp0Linear, i.Tex2 );
	col3 = tex2D( Samp0Linear, i.Tex3 );
	
	Output.Color = ( col0 + col1 + col2 + col3 ) * 0.25f;	
//	Output.Color.w = 1.0f;

	return Output;
}

PS_OUTPUT PSGaussX(VSTEX8_OUTPUT In)
{   
    PS_OUTPUT Out;
    
    Out.Color  = weight[0] * (tex2D( Samp0Linear, In.Tex0 )
                        + tex2D( Samp0Linear, In.Tex7 + offsetX ));
    Out.Color += weight[1] * (tex2D( Samp0Linear, In.Tex1 )
                        + tex2D( Samp0Linear, In.Tex6 + offsetX ));
    Out.Color += weight[2] * (tex2D( Samp0Linear, In.Tex2 )
                        + tex2D( Samp0Linear, In.Tex5 + offsetX ));
    Out.Color += weight[3] * (tex2D( Samp0Linear, In.Tex3 )
                        + tex2D( Samp0Linear, In.Tex4 + offsetX ));
    Out.Color += weight[4] * (tex2D( Samp0Linear, In.Tex4 )
                        + tex2D( Samp0Linear, In.Tex3 + offsetX ));
    Out.Color += weight[5] * (tex2D( Samp0Linear, In.Tex5 )
                        + tex2D( Samp0Linear, In.Tex2 + offsetX ));
    Out.Color += weight[6] * (tex2D( Samp0Linear, In.Tex6 )
                        + tex2D( Samp0Linear, In.Tex1 + offsetX ));
    Out.Color += weight[7] * (tex2D( Samp0Linear, In.Tex7 )
                        + tex2D( Samp0Linear, In.Tex0 + offsetX ));
    
    Out.Color.xyz *= colmult;
    
    return Out;
}

PS_OUTPUT PSGaussY(VSTEX8_OUTPUT In)
{   
    PS_OUTPUT Out;
    
    Out.Color  = weight[0] * (tex2D( Samp0Linear, In.Tex0 )
                        + tex2D( Samp0Linear, In.Tex7 + offsetY ));
    Out.Color += weight[1] * (tex2D( Samp0Linear, In.Tex1 )
                        + tex2D( Samp0Linear, In.Tex6 + offsetY ));
    Out.Color += weight[2] * (tex2D( Samp0Linear, In.Tex2 )
                        + tex2D( Samp0Linear, In.Tex5 + offsetY ));
    Out.Color += weight[3] * (tex2D( Samp0Linear, In.Tex3 )
                        + tex2D( Samp0Linear, In.Tex4 + offsetY ));
    Out.Color += weight[4] * (tex2D( Samp0Linear, In.Tex4 )
                        + tex2D( Samp0Linear, In.Tex3 + offsetY ));
    Out.Color += weight[5] * (tex2D( Samp0Linear, In.Tex5 )
                        + tex2D( Samp0Linear, In.Tex2 + offsetY ));
    Out.Color += weight[6] * (tex2D( Samp0Linear, In.Tex6 )
                        + tex2D( Samp0Linear, In.Tex1 + offsetY ));
    Out.Color += weight[7] * (tex2D( Samp0Linear, In.Tex7 )
                        + tex2D( Samp0Linear, In.Tex0 + offsetY ));

    Out.Color.xyz *= colmult;
    
    return Out;
}

PS_OUTPUT PSGauss5x5( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
	
    float4 sample = 0.0f;

	for( int i=0; i < 13; i++ ){
		sample += avSampleWeights[i] * tex2D( Samp0Point, In.Tex0 + avSampleOffsets[i] );
//		sample += avSampleWeights[i] * tex2D( Samp0Linear, In.Tex0 + avSampleOffsets[i] );
	}
	Out.Color = sample;

	return Out;
}

PS_OUTPUT PSTLVPoint( VSTLV_OUTPUT In )
{
	PS_OUTPUT Out;
	
    float4 sample;
	sample = tex2D( Samp0Point, In.Tex0 );

	Out.Color = sample * In.Diffuse;

	return Out;
}

PS_OUTPUT PSTLVLinear( VSTLV_OUTPUT In )
{
	PS_OUTPUT Out;
	
    float4 sample;
	sample = tex2D( Samp0Linear, In.Tex0 );

	Out.Color = sample * In.Diffuse;

	return Out;
}

PS_OUTPUT PSDownScale4x4( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
    float4 sample = 0.0f;

	for( int i=0; i < 16; i++ )
	{
		sample += tex2D( Samp0Point, In.Tex0 + avSampleOffsets[i] );
	}
    
	Out.Color = sample / 16;
	return Out;
}

PS_OUTPUT PSDownScale2x2( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
    float4 sample = 0.0f;

	for( int i=0; i < 4; i++ )
	{
		sample += tex2D( Samp0Point, In.Tex0 + avSampleOffsets[i] );
	}
    
    Out.Color = sample / 4;
	return Out;
}

PS_OUTPUT PSToneMapFirst( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
    float3 vSample = 0.0f;
    float  fLogLumSum = 0.0f;

    for(int iSample = 0; iSample < 9; iSample++)
    {
        // Compute the sum of log(luminance) throughout the sample points
        vSample = tex2D(Samp0Point, In.Tex0 + avSampleOffsets[iSample]);
        fLogLumSum += log(dot(vSample, LUMINANCE_VECTOR)+0.0001f);
    }
    
    // Divide the sum to complete the average
    fLogLumSum /= 9;

	Out.Color = float4(fLogLumSum, fLogLumSum, fLogLumSum, 1.0f);
    return Out;
}
PS_OUTPUT PSToneMapSecond( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
    float fResampleSum = 0.0f; 
    
    for(int iSample = 0; iSample < 16; iSample++)
    {
        // Compute the sum of luminance throughout the sample points
        fResampleSum += tex2D(Samp0Point, In.Tex0 + avSampleOffsets[iSample]);
    }
    
    // Divide the sum to complete the average
    fResampleSum /= 16;
	Out.Color = float4(fResampleSum, fResampleSum, fResampleSum, 1.0f);
    return Out;
}
PS_OUTPUT PSToneMapFinal( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
    float fResampleSum = 0.0f;
    
    for(int iSample = 0; iSample < 16; iSample++)
    {
        // Compute the sum of luminance throughout the sample points
        fResampleSum += tex2D(Samp0Point, In.Tex0 + avSampleOffsets[iSample]);
    }
    
    // Divide the sum to complete the average, and perform an exp() to complete
    // the average luminance calculation
    fResampleSum = exp(fResampleSum/16);
    Out.Color = float4(fResampleSum, fResampleSum, fResampleSum, 1.0f);
    return Out;
}


PS_OUTPUT PSBrightPass( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
	float4 vSample = tex2D( Samp0Point, In.Tex0 );
	float  fAdaptedLum = tex2D( Samp1Point, float2(0.5f, 0.5f) );
	
	// Determine what the pixel's value will be after tone-mapping occurs
	vSample.rgb *= fMiddleGray/(fAdaptedLum + 0.001f);
	
	// Subtract out dark pixels
	vSample.rgb -= fBrightPassThreshold;
	
	// Clamp to 0
	vSample = max(vSample, 0.0f);
	
	// Map the resulting value into the 0 to 1 range. Higher values for
	// BRIGHT_PASS_OFFSET will isolate lights from illuminated scene 
	// objects.
	vSample.rgb /= (fBrightPassOffset + vSample);
    Out.Color = vSample;
	return Out;
}

PS_OUTPUT PSHDRFinalPass( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;
    float4 vSample = tex2D(Samp0Point, In.Tex0);
    float4 vBloom = tex2D(Samp1Linear, In.Tex0);
	float fAdaptedLum = tex2D(Samp2Point, float2(0.5f, 0.5f));
	
    // Map the high range of color values into a range appropriate for
    // display, taking into account the user's adaptation level, and selected
    // values for for middle gray and white cutoff.
	vSample.rgb *= fMiddleGray/(fAdaptedLum + 0.001f);
	vSample.rgb /= (1.0f+vSample);
    
    // Add the star and bloom post processing effects
    vSample += vBloom;
    Out.Color = vSample;
    return Out;
}

PS_OUTPUT PSMono( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;

	float4 vSample;
	vSample = tex2D( Samp0Point, In.Tex0 );

	Out.Color.xyz = dot( vSample.xyz, RGB2Y );
	Out.Color.w = vSample.w;

    return Out;
}

PS_OUTPUT PSCbCr( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;

	float4 vSample = tex2D( Samp0Point, In.Tex0 );

	float3 YCbCr;
	YCbCr.x = dot( vSample.xyz, RGB2Y );
	
	YCbCr.y = CbCr.x;//定数(指定値)を代入
	YCbCr.z = CbCr.y;//定数(指定値)を代入
	
	float3 newrgb = YCbCr.x
			+ mul( Cb2RGB, YCbCr.y )
			+ mul( Cr2RGB, YCbCr.z );
			
	Out.Color.xyz = newrgb;
	Out.Color.w = vSample.w;		

    return Out;
}

PS_OUTPUT PSDummy( VSTEX1_OUTPUT In )
{
	PS_OUTPUT Out;

	float4 vSample = tex2D( SampUser0Point, In.Tex0 );
	Out.Color = vSample * userFL4_0;

	return Out;
}

// ------------------------------------------------------------
// テクニック
// ------------------------------------------------------------
technique PostEffectBlur0
{
    pass P0// １６ボックスフィルタサンプリング
    {
        VertexShader = compile vs_2_0 VS16Box();
        PixelShader  = compile ps_2_0 PSav4();        
    }
    pass P1// ９コーンフィルタサンプリング
    {
        VertexShader = compile vs_2_0 VS9Corn();
        PixelShader  = compile ps_2_0 PSav4();        
    }
    pass P2// Gauss5x5フィルタサンプリング
    {
        VertexShader = compile vs_2_0 VS1Tex();
        PixelShader  = compile ps_2_0 PSGauss5x5();        
    }
}

technique PostEffectBlurGauss
{
    pass P0
    {
        VertexShader = compile vs_2_0 VSGaussX();
        PixelShader  = compile ps_2_0 PSGaussX();
    }
    pass P1
    {
        VertexShader = compile vs_2_0 VSGaussY();
        PixelShader  = compile ps_2_0 PSGaussY();
    }
}

technique PostEffectTLV
{
	//E3DBlendRTTextureでsrcとdestとでそれぞれ呼び出される。
	//blend自体は固定機能で（RenderStateで）行われる。
    pass P0
    {
        VertexShader = compile vs_2_0 VSTLV();
        PixelShader  = compile ps_2_0 PSTLVPoint();
    }
    pass P1
    {
        VertexShader = compile vs_2_0 VSTLV();
        PixelShader  = compile ps_2_0 PSTLVLinear();
    }
}

technique PostEffectDownScale
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSDownScale4x4();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSDownScale2x2();
	}

}

technique PostEffectToneMap
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSToneMapFirst();
	}

	pass P1
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSToneMapSecond();
	}

	pass P2
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSToneMapFinal();
	}

}

technique PostEffectBrightPass
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSBrightPass();
	}
}

technique PostEffectHDRFinal
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSHDRFinalPass();
	}
}

technique PostEffectMono
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSMono();
	}
}

technique PostEffectCbCr
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSCbCr();
	}
}

//////////////////////////////////////////////
// user定義テクニック
//////////////////////////////////////////////

technique PostEffectUser0
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSDummy();
	}
}
technique PostEffectUser1
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSDummy();
	}
}
technique PostEffectUser2
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSDummy();
	}
}
technique PostEffectUser3
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSDummy();
	}
}
technique PostEffectUser4
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS1Tex();
		PixelShader = compile ps_2_0 PSDummy();
	}
}

