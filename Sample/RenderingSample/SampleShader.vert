#version 330

// 頂点属性
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// uniform
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main()
{
    // 座標変換
    // ワールド座標
    vec4 pos = vec4(inPosition, 1.0);
    pos = pos * uWorldTransform;
    fragWorldPos = pos.xyz;

    // クリップ座標
    gl_Position = pos * uViewProj;

    // 法線変換
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

    // テクスチャ座標
    fragTexCoord = inTexCoord;
