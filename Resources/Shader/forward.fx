#ifndef _FORWARD_FX_
#define _FORWARD_FX_

#include "params.fx"
#include "utils.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUT
{
    float4 pos : SV_Position;           // SV : SystemValue
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewBinormal : BINORMAL;
};

// ���� ���� ����.
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    // Projection���� ���� ������ǥ��� ��.
    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    // �������� ���ֱ� ���ؼ� View��ǥ������� ���
    output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;      // x,y,z,w << w�� 1�� �Ǿ���.
    output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);        // ���⺤�ʹ� ������ w�� 0���� �Է������ translation�� ������� �ʱ� ������ ������ ���� 0���� �Է�
    output.viewTangent = normalize(mul(float4(input.tangent, 0.f), g_matWV).xyz);        // g_matWV�� ���������ν� view space �������� �� ź��Ʈ�� �������ϴ�.
    output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));

    return output;
}

// �ȼ� ���� ����
float4 PS_Main(VS_OUT input) : SV_Target
{
    // �ȼ� ���̴����� ����� �÷�.
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (g_tex_on_0)
        color = g_tex_0.Sample(g_sam_0, input.uv);

    float3 viewNormal = input.viewNormal;
    // Bump mapping
    if (g_tex_on_1)
    {
        // �÷��� ���÷��� ���� ����� 0~1������ ��ȯ�� ������ �޾ƿɴϴ�.
        // [0. 255] �������� [0, 1]�� ��ȯ
        float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
        // [0, 1] �������� [-1, 1]�� ��ȯ
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
    }
    
    //float4 color = float4(1.f, 1.f, 1.f, 1.f);

    LightColor totalColor = (LightColor)0.f;

    for (int i = 0; i < g_lightCount; ++i)
    {
         LightColor color = CalculateLightColor(i, viewNormal, input.viewPos);
         totalColor.diffuse += color.diffuse;
         totalColor.ambient += color.ambient;
         totalColor.specular += color.specular;
    }

    color.xyz = (totalColor.diffuse.xyz * color.xyz)
        + totalColor.ambient.xyz * color.xyz
        + totalColor.specular.xyz;

     return color;
}

#endif