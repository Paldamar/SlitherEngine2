#pragma once

#include "Materials.h"
#include "RenderItem.h"
#include "d3dUtil.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct DrawParams
{
	UINT mCbvSrvDescriptorSize = 0;
	UINT mTexNormalOffset = 0;
	UINT mObjCbvOffset = 0;
	UINT mMatCbvOffset = 0;
	int  mCurrFrameResourceIndex = 0;
};

class GameTimer;
class RenderNode
{
public:
	RenderNode();
	~RenderNode();

public:
	virtual RenderInfo& GetRenderNodeInfo(int cIndex = 0);
	const DirectX::BoundingBox& GetBoundingBox() const { return mInitBoundsBox; }
	MeshGeometry* GetMeshGeometry() const { return  mGeometry.get(); }

	UINT GetAllRitemsSize() const;

	virtual void BuildRenderItem(Materials& mMaterials, std::string matrialPrefix);
		
	virtual void UpdateRenderNodeCBs(
		FrameResource* mCurrFrameResource,
		const Light& mMainLight,
		const GameTimer& gt);

	virtual void UpdateRenderNodeCBs(UploadBuffer<ObjectConstants>& currendCB);

	void AddRenderItem(std::unique_ptr<RenderItem> subRitem, RenderLayer layer);

	void BuildSubRitems(
		std::string geoName,
		//std::string subRitemName,
		std::string matName,
		RenderLayer subRtype,
		UINT& objCBIndex,
		FXMMATRIX& worldTransform,
		CXMMATRIX& texTransform,
		Materials &mMaterials, 
		std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> &mGeometries);

	std::vector<RenderItem*> GetRenderItemLayer(RenderLayer layer)
	{
		return mRitems[(int)layer];
	}

	virtual void Draw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, 
			          ComPtr<ID3D12DescriptorHeap> mCbvHeap, 
					  const DrawParams mDrawParams, 
					  Materials& mMaterials, 
					  std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs);

public: 
	bool mTransformDirty = false;

private:
	DirectX::BoundingBox mInitBoundsBox;
	std::unique_ptr<MeshGeometry> mGeometry;

protected:
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;
	std::vector<RenderItem*> mRitems[(int)RenderLayer::Count];
	RenderInfo mRenderNodeInfo;

	virtual void DrawRenderItems(const std::vector<RenderItem*>& ritems, 
							     Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, 
								 ComPtr<ID3D12DescriptorHeap> mCbvHeap, 
								 const DrawParams mDrawParams,
								 Materials &mMaterials);
	virtual void SetCommandList(RenderItem* ri, 
								Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, 
							    ComPtr<ID3D12DescriptorHeap> mCbvHeap, 
								const DrawParams mDrawParams,
								Materials& mMaterials);
};
