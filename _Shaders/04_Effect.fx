float4 Color = float4(1,1,1,1);
//-----------------------------------------------------------------------------
// Structures
//-----------------------------------------------------------------------------
matrix World;
matrix View;
matrix Projection;
//shader 는 모든걸 float

struct VertexInput
{
    float4 Position : POSITION0;
    float3 Color : COLOR0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float3 Color : COLOR0;
};

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    /*
    * IA -> VS -1 -1 에서 => WVP
    * 정점을 WVP 변환을 한다.
    */
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Color = input.Color;

    return output;
    /*VertexOutput output;
    output.Position = input.Position;
    output.Color = input.Color;

    return output;*/
}

//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
float4 PS(VertexOutput input) : SV_TARGET0
{
   // return float4(input.Color, 1);
    return Color;
}

//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}