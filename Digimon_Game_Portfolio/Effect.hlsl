struct VertexInput
{
    float4 Position : POSITION0;
    float3 Color : COLOR0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = input.Position;
    output.Color = input.Color;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return float4(input.Color, 1);
}
