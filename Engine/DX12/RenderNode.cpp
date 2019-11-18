#include "DX12PCH.h"

using namespace DirectX;

RenderNode::RenderNode():
	mRenderNodeInfo()
{
}

RenderNode::~RenderNode()
{
}

RenderInfo& RenderNode::GetRenderNodeInfo(int cIndex /*= 0*/)
{
	RenderInfo aTemp;
	return aTemp;
}

UINT RenderNode::GetAllRitemsSize() const
{
	return (UINT)mAllRitems.size();
}

void RenderNode::BuildRenderItem(Materials& mMaterials, std::string matrialPrefix)
{

}


void RenderNode::UpdateRenderNodeCBs(FrameResource* mCurrFrameResource, const Light& mMainLight, const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	UpdateRenderNodeCBs(*currObjectCB);
}

void RenderNode::UpdateRenderNodeCBs(UploadBuffer<ObjectConstants>& currendCB)
{
	for (auto& e : mAllRitems)
	{
		if (e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));

			currendCB.CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}

void RenderNode::AddRenderItem(std::unique_ptr<RenderItem> subRitem, RenderLayer layer)
{
	mRitems[(int)layer].push_back(subRitem.get());
	mAllRitems.push_back( std::move(subRitem));

	Renderer::GetRenderer()->RenderItemSize += mAllRitems.capacity();
}

void RenderNode::BuildSubRitems(
	std::string geoName, 
	/*std::string subRitemName, */
	std::string matName, 
	RenderLayer subRtype, 
	UINT& objCBIndex, 
	FXMMATRIX& worldTransform, 
	CXMMATRIX& texTransform,
	Materials &mMaterials,
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> &mGeometries)
{
	auto subRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&subRitem->World, worldTransform);
	XMStoreFloat4x4(&subRitem->TexTransform, texTransform);
	subRitem->ObjCBIndex = objCBIndex;
	subRitem->Mat = mMaterials.Get(matName);
	subRitem->Geo = mGeometries[geoName].get();
	subRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	subRitem->IndexCount = subRitem->Geo->DrawArgs[geoName].IndexCount;
	subRitem->StartIndexLocation = subRitem->Geo->DrawArgs[geoName].StartIndexLocation;
	subRitem->BaseVertexLocation = subRitem->Geo->DrawArgs[geoName].BaseVertexLocation;
	subRitem->Geo->DrawArgs[geoName].Bounds.Transform(subRitem->Bounds, XMLoadFloat4x4(&subRitem->World));
	AddRenderItem(std::move(subRitem), subRtype);
	//mRitems[(int)subRtype].push_back(subRitem.get());
	//mAllRitems.push_back(std::move(subRitem));
}

void RenderNode::Draw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, 
			     ComPtr<ID3D12DescriptorHeap> mCbvHeap, 
				 const DrawParams mDrawParams,
				 Materials& mMaterials, 
				 std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs)
{
	DrawRenderItems(mRitems[(int)RenderLayer::Opaque], cmdList, mCbvHeap, mDrawParams, mMaterials);
	DrawRenderItems(mRitems[(int)RenderLayer::RenderNode], cmdList, mCbvHeap, mDrawParams, mMaterials);
}

void RenderNode::DrawRenderItems(const std::vector<RenderItem*>& ritems, 
							Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, 
							ComPtr<ID3D12DescriptorHeap> mCbvHeap, 
							const DrawParams mDrawParams, 
							Materials& mMaterials)
{
	// For each render item...
	for (size_t i = 0; i < ritems.size(); ++i)
	{
		auto ri = ritems[i];
		SetCommandList(ri, cmdList, mCbvHeap, mDrawParams, mMaterials);
		
		cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
	}
}

void RenderNode::SetCommandList(RenderItem* ri, 
						   Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList, 
	                       ComPtr<ID3D12DescriptorHeap> mCbvHeap, 
						   const DrawParams mDrawParams, 
	                       Materials& mMaterials)
{
	cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
	cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
	cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

	const int texOffset = 3;
	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	tex.Offset(ri->Mat->DiffuseSrvHeapIndex, mDrawParams.mCbvSrvDescriptorSize);

	cmdList->SetGraphicsRootDescriptorTable(0, tex);

	if (ri->Mat->NormalSrvHeapIndex >= 0)
	{
		UINT normalOffset = mDrawParams.mTexNormalOffset + ri->Mat->NormalSrvHeapIndex;
		CD3DX12_GPU_DESCRIPTOR_HANDLE normal(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		normal.Offset(normalOffset, mDrawParams.mCbvSrvDescriptorSize);
		cmdList->SetGraphicsRootDescriptorTable(1, normal);
	}

	if (ri->ObjCBIndex >= 0)
	{
		
		UINT cbvIndex = mDrawParams.mObjCbvOffset + mDrawParams.mCurrFrameResourceIndex * Renderer::GetRenderer()->RenderItemSize + ri->ObjCBIndex;
		auto cbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
		cbvHandle.Offset(cbvIndex, mDrawParams.mCbvSrvDescriptorSize);
		cmdList->SetGraphicsRootDescriptorTable(texOffset, cbvHandle);
	}

	UINT matCbvIndex = mDrawParams.mMatCbvOffset + mDrawParams.mCurrFrameResourceIndex * mMaterials.GetSize() + ri->Mat->MatCBIndex;
	auto matCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	matCbvHandle.Offset(matCbvIndex, mDrawParams.mCbvSrvDescriptorSize);
	cmdList->SetGraphicsRootDescriptorTable(texOffset + 1, matCbvHandle);
}

