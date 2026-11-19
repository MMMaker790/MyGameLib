#version 330

// vertから
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

// uniform
uniform sampler2D uTexture;
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightDir = normalize(vec3(0.5, 1.0, 0.5));

out vec4 fragColor;

void main()
{
    vec4 texColor = texture(uTexture, fragTexCoord);

    // シンプルなディフューズライティング
    float diff = max(dot(normalize(fragNormal), lightDir), 0.2);

    vec3 result = texColor.rgb * objectColor * lightColor * diff;
    fragColor = vec4(result, texColor.a);
}