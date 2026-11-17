#include "GameMath.h"
#include "Rendering.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <string>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

bool Initialize(SDL_Window*& window, Renderer& renderer);
bool LoadModel(Renderer& renderer, const std::string& modelFileName, ResourceID& outMeshID);
bool Draw(Renderer& renderer, SDL_Window* window, ResourceID meshID);

int main(int argc, char* argv[])
{
    std::cout << "RenderingSample" << std::endl;

    SDL_Window* window = nullptr;
    Renderer renderer;
    if (!Initialize(window, renderer))
    {
        std::cout << "Initialization failed" << std::endl;
        return -1;
    }

    ResourceID meshID;
    if (!LoadModel(renderer, "./RenderingSample/SampleModel.json", meshID))
    {
        std::cout << "Failed to load model" << std::endl;
        return -1;
    }

    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // 描画処理
            Draw(renderer, window, meshID);

            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }
    }

    return 0;
}

bool Initialize(SDL_Window*& window, Renderer& renderer)
{
    // SDL2の初期化
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
    {
        std::cout << "Failed to initialize SDL2: " << SDL_GetError() << std::endl;
        return false;
    }

    // SDL2のウィンドウ作成
    renderer.SetGraphicsAPI(GraphicsAPI::OpenGL);
    if (!renderer.PrepareWindow())
    {
        std::cout << "Failed to prepare window" << std::endl;
        return false;
    }
    window = SDL_CreateWindow(
        "RenderingSample",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (!window)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    // レンダラーの初期化
    if (!renderer.Initialize(window, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        std::cout << "Failed to initialize renderer" << std::endl;
        return false;
    }

    return true;
}

bool LoadModel(Renderer& renderer, const std::string& modelFileName, ResourceID& outMeshID)
{
    // モデルのロード処理
    if (!renderer.GetMesh(modelFileName, outMeshID))
    {
        std::cout << "Failed to load model: " << modelFileName << std::endl;
        return false;
    }
    return true;
}

bool Draw(Renderer& renderer, SDL_Window* window, ResourceID meshID)
{
    Renderer::MeshDrawInfo meshDrawInfo;
    meshDrawInfo.MeshID = meshID;
    meshDrawInfo.TextureIdx = 0; // 使用するテクスチャのインデックスを指定
    Vector3 position(0.0f, 0.0f, -5.0f); // モデルの位置
    Matrix4 translationMatrix = Matrix4::CreateTranslation(position);
    meshDrawInfo.WorldTransform = translationMatrix;
    renderer.DrawMesh(meshDrawInfo);

    Vector3 cameraPosition(0.0f, 0.0f, 0.0f);
    Vector3 cameraTarget(0.0f, 0.0f, -1.0f);
    Vector3 cameraUp(0.0f, 1.0f, 0.0f);
    Matrix4 viewMatrix = Matrix4::CreateLookAt(cameraPosition, cameraTarget, cameraUp);
    float fov = Math::ToRadians(45.0f); // ラジアンに変換
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    Matrix4 projectionMatrix = Matrix4::CreatePerspectiveFOV(
        fov, 
        static_cast<float>(WINDOW_WIDTH), 
        static_cast<float>(WINDOW_HEIGHT), 
        nearPlane, 
        farPlane);
    renderer.DrawScene(viewMatrix, projectionMatrix);

    renderer.Draw(window);

    return true;
}