// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "VertexArray.h"
#include <GL/glew.h>
#include <iostream>
#include <cassert>

namespace
{
	void CheckGLError(const char* file, int line)
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::cerr << "OpenGL Error 0x" << std::hex << err << " at " << file << ":" << line << std::endl;
			assert(false);
		}
	}
}

VertexArray::VertexArray(const void* verts, unsigned int numVerts, Layout layout,
	const unsigned int* indices, unsigned int numIndices)
	: mNumVerts(numVerts)
	, mNumIndices(numIndices)
	, mVertexBuffer(0)
	, mIndexBuffer(0)
	, mLayout(layout)
{
	// 頂点配列オブジェクトの作成
	glGenVertexArrays(1, &mVertexArray);
	CheckGLError(__FILE__, __LINE__);
	glBindVertexArray(mVertexArray);
	CheckGLError(__FILE__, __LINE__);

	unsigned vertexSize = GetVertexSize(layout);

	// 頂点バッファの作成
	glGenBuffers(1, &mVertexBuffer);
	CheckGLError(__FILE__, __LINE__);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	CheckGLError(__FILE__, __LINE__);
	glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);
	CheckGLError(__FILE__, __LINE__);

	// インデックスバッファの作成
	glGenBuffers(1, &mIndexBuffer);
	CheckGLError(__FILE__, __LINE__);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	CheckGLError(__FILE__, __LINE__);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	CheckGLError(__FILE__, __LINE__);

	// 頂点レイアウト毎の設定
	if (layout == PosNormTex)
	{
		// 位置は3つのfloat
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// 法線は3つのfloat
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// テクスチャ座標は2つのfloat
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));
	}
	else if (layout == PosNormSkinTex)
	{
		// 位置は3つのfloat
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);

		// 法線は3つのfloat
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 3));

		// スキニングジョイントインデックスは 4つのunsigned byte(int維持)
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6));

		// スキニングジョイントウェイトは 4つのunsigned byte(floatに変換)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 4));

		// テクスチャ座標は2つのfloat
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, vertexSize,
			reinterpret_cast<void*>(sizeof(float) * 6 + sizeof(char) * 8));
	}
}

// ムーブコンストラクタ
VertexArray::VertexArray(VertexArray&& other) noexcept
	: mNumVerts(other.mNumVerts)
	, mNumIndices(other.mNumIndices)
	, mLayout(other.mLayout)
	, mVertexBuffer(other.mVertexBuffer)
	, mIndexBuffer(other.mIndexBuffer)
	, mVertexArray(other.mVertexArray)
{
	// ソースの ID を 0 にリセット（デストラクタで削除されないように）
	other.mNumVerts = 0;
	other.mNumIndices = 0;
	other.mVertexBuffer = 0;
	other.mIndexBuffer = 0;
	other.mVertexArray = 0;
}

// ムーブ代入演算子
VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		// 既存のリソースを解放
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteVertexArrays(1, &mVertexArray);

		// 値を移動
		mNumVerts = other.mNumVerts;
		mNumIndices = other.mNumIndices;
		mLayout = other.mLayout;
		mVertexBuffer = other.mVertexBuffer;
		mIndexBuffer = other.mIndexBuffer;
		mVertexArray = other.mVertexArray;

		// ソースをリセット
		other.mNumVerts = 0;
		other.mNumIndices = 0;
		other.mVertexBuffer = 0;
		other.mIndexBuffer = 0;
		other.mVertexArray = 0;
	}
	return *this;
}

VertexArray::~VertexArray()
{
	// バッファの削除
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void* VertexArray::GetVertexBuffer() const
{
	// GLからバッファを取得
	GLint old;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	void* ret = nullptr;
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, mNumVerts * GetVertexSize(mLayout), &ret);
	return ret;
}

void* VertexArray::GetIndexBuffer() const
{
	// GLからインデックスバッファを取得
	GLint old;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &old);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	void* ret = nullptr;
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mNumIndices * sizeof(unsigned int), &ret);
	return ret;
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}

unsigned int VertexArray::GetVertexSize(VertexArray::Layout layout)
{
	unsigned vertexSize = 8 * sizeof(float);
	if (layout == PosNormSkinTex)
	{
		vertexSize = 8 * sizeof(float) + 8 * sizeof(char);
	}
	return vertexSize;
}
