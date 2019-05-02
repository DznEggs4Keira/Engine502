
// GLOBALS
Texture2D ObjTexture;
SamplerState ObjSamplerState;
TextureCube SkyMap;

cbuffer GradientBuffer
{
	float4 apexColor;
	float4 centerColor;
};

// TYPEDEFS
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 spherePosition : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SkySpherePixelShader(PixelInputType input) : SV_TARGET
{
	float height;
	float4 outputColor;
	float4 textureColor;

	textureColor = SkyMap.Sample(ObjSamplerState, input.spherePosition);

	// Determine the position on the sky dome where this pixel is located.
	height = input.spherePosition.y;

	// The value ranges from -1.0f to +1.0f so change it to only positive values.
	if (height < 0.0f)
	{
		height = 0.0f;
	}

	// Determine the gradient color by interpolating between the apex and center based on the height of the pixel in the sky dome.
	outputColor = lerp(centerColor, apexColor, height);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	outputColor = outputColor * textureColor;

	return outputColor;
}
