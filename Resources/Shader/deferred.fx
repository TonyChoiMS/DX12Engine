#ifndef _DEFERRED_FX_
#define _DEFERRED_FX_

#include "params.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;

    row_major matrix matWorld : W;
    row_major matrix matWV : WV;
    row_major matrix matWVP : WVP;
    uint instanceID : SV_InstanceID;
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

// 정점 단위 연산.
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    // Instancing이 적용되는 것
    if (g_int_0)
    {
        // Projection까지 가서 투영좌표계로 감.
        output.pos = mul(float4(input.pos, 1.f), input.matWVP);
        output.uv = input.uv;

        // 빛연산을 해주기 위해서 View좌표계까지만 계산
        output.viewPos = mul(float4(input.pos, 1.f), input.matWV).xyz;      // x,y,z,w << w는 1이 되야함.
        output.viewNormal = normalize(mul(float4(input.normal, 0.f), input.matWV).xyz);        // 방향벡터는 마지막 w를 0으로 입력해줘야 translation이 적용되지 않기 때문에 마지막 값을 0으로 입력
        output.viewTangent = normalize(mul(float4(input.tangent, 0.f), input.matWV).xyz);        // g_matWV를 곱해줌으로써 view space 기준으로 한 탄젠트가 구해집니다.
        output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));
    }
    else
    {
        // 적용 안됨.
        // Projection까지 가서 투영좌표계로 감.
        output.pos = mul(float4(input.pos, 1.f), g_matWVP);
        output.uv = input.uv;

        // 빛연산을 해주기 위해서 View좌표계까지만 계산
        output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;      // x,y,z,w << w는 1이 되야함.
        output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);        // 방향벡터는 마지막 w를 0으로 입력해줘야 translation이 적용되지 않기 때문에 마지막 값을 0으로 입력
        output.viewTangent = normalize(mul(float4(input.tangent, 0.f), g_matWV).xyz);        // g_matWV를 곱해줌으로써 view space 기준으로 한 탄젠트가 구해집니다.
        output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));
    }

    return output;
}

struct PS_OUT
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
};

// 픽셀 단위 연산
// 디퍼드 쉐이더는 여기서 라이팅 계산을 하지 않음.
PS_OUT PS_Main(VS_OUT input)
{
    PS_OUT output = (PS_OUT)0;
    
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (g_tex_on_0)
        color = g_tex_0.Sample(g_sam_0, input.uv);

    float3 viewNormal = input.viewNormal;
    if (g_tex_on_1)
    {
        // [0,255] 범위에서 [0,1]로 변환
        float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
        // [0,1] 범위에서 [-1, 1]로 변환
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
    }

    output.position = float4(input.viewPos.xyz, 0.f);
    output.normal = float4(viewNormal.xyz, 0.f);
    output.color = color;

     return output;
}

#endif 