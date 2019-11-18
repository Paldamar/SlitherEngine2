#pragma once


#include "d3dUtil.h"

enum class RenderLayer : int
{
	Opaque = 0,
	Mirrors,
	Reflected,
	Transparent,
	Sky,
	RenderNode,
	Character,
	Shadow,
	Count
};

struct RenderInfo
{	
	DirectX::BoundingBox mBoundingBox;
	
	RenderInfo()
	{ }
};

struct RenderItem
{
	RenderItem() = default;
	RenderItem(const RenderItem& rhs) = delete;

	DirectX::XMFLOAT4X4 World = MathHelper::Identity4x4();
	DirectX::XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	int ObjCBIndex = -1;

	int NumFramesDirty = gNumFrameResources;
	
	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;
	DirectX::BoundingBox Bounds;

	// Primitive topology.
	D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// DrawIndexedInstanced parameters.
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	int BaseVertexLocation = 0;
};
