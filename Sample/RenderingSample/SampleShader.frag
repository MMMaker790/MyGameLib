#version 330

// vertから
in vec2 inTexCoord;
in vec3 inNormal;
in vec3 inWorldPos;

// uniform
uniform sampler2D utexture;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));

out vec3 fragColor;

void main()
{
    vec4 texColor = texture(uSampler, texCoord);

    // シンプルなディフューズライティング
    float diff = max(dot(normalize(inNormal), lightDir), 0.2);

    vec3 result = texColor.rgb * objectColor * lightColor * diff;
    fragColor = vec4(result, texColor.a);
}