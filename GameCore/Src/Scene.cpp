#include "Scene.h"

Scene::~Scene()
{
}

void Scene::OnEnter()
{
    // シーンがアクティブになるときの処理
}

void Scene::OnExit()
{
    // シーンが非アクティブになるときの処理
}

void Scene::Update(const UpdateContext &context)
{
    // シーンの更新処理
}

void Scene::Draw()
{
    // シーンの描画処理
}