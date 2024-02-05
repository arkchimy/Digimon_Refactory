Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);

SamplerState g_sampler0 : register(s0);

cbuffer PixelShaderConstantBuffer : register(b0) { float xSplit; }

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float2 center = float2(0.5, 0.5);
    float distance =
        sqrt(pow(input.uv.x - center.x, 2) + pow(input.uv.y - center.y, 2));

    float radius = 0.2;
    
    return distance <= radius ? g_texture0.Sample(g_sampler0, input.uv) * 1.5f
                              : g_texture0.Sample(g_sampler0, input.uv);
    /* return xSplit <= input.uv.x ? g_texture0.Sample(g_sampler0, input.uv)
                                 : g_texture1.Sample(g_sampler0,input.uv);*/
}
