#include "Mesh.h"
#include "JsonData.h"
#include "LoaderModel.h"
#include "GameMath.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <rapidjson/document.h>
#include <fstream>
#include <utility>

Mesh::Mesh()
	: mVertexArray()
	, mRadius(0.0f)
	, mSpecPower(100.0f)
{
}

Mesh::~Mesh()
{
}

// ムーブコンストラクタ
Mesh::Mesh(Mesh&& other) noexcept
	: mTextures(std::move(other.mTextures))
	, mVertexArray(std::move(other.mVertexArray))
	, mShaderName(std::move(other.mShaderName))
	, mFileName(std::move(other.mFileName))
	, mRadius(other.mRadius)
	, mSpecPower(other.mSpecPower)
{
	// 移動元を空の有効な状態にリセット
	other.mRadius = 0.0f;
	other.mSpecPower = 100.0f;
}

// ムーブ代入演算子
Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		mTextures   = std::move(other.mTextures);
		mVertexArray = std::move(other.mVertexArray);
		mShaderName = std::move(other.mShaderName);
		mFileName   = std::move(other.mFileName);
		mRadius     = other.mRadius;
		mSpecPower  = other.mSpecPower;

		// 移動元を空の有効な状態にリセット
		other.mRadius = 0.0f;
		other.mSpecPower = 100.0f;
	}
	return *this;
}

bool Mesh::Load(const std::string &fileName, Renderer *renderer)
{
	return LoaderModel::LoadMesh(fileName, *this, renderer);
}