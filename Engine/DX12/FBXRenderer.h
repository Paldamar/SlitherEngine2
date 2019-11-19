#pragma once

#include "Renderer.h"
#include "RenderNode.h"
#include <map>

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

//const int gNumFrameResources = 3;

class Textures;
class Materials;
class SkinnedActor;
class AIPawn;

class FBXRenderer : public Renderer
{
public:
	FBXRenderer();
	FBXRenderer(const FBXRenderer& rhs) = delete;
	FBXRenderer& operator=(const FBXRenderer& rhs) = delete;
	~FBXRenderer();

	virtual bool Initialize(MsgProc_Callback callback)override;

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	void UpdateRenderNodeCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);
	void UpdateMaterialCB(const GameTimer& gt);

	virtual void Draw(const GameTimer& gt)override;

	virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
	virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
	virtual void OnMouseMove(WPARAM btnState, int x, int y)override;
	void OnKeyboardInput(const GameTimer& gt);

	void LoadTextures();
	void BuildMaterials();
	void BuildShapeGeometry();
	void BuildRenderNodes();
	void BuildRenderNodeFbxGeometry(std::string folderPath, std::string fbxModelName);
	void BuildRootSignature();
	void BuildDescriptorHeaps();
	void BuildShadersAndInputLayout();
	void BuildFrameResources();

	void BuildTextureBufferViews();
	void BuildConstantBufferViews(int mCbvOffset, UINT ItemSize, UINT ConstantsSize, eUploadBufferIndex e);
	void BuildConstantBufferViews();
	   	  
	void BuildPSOs();



	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

private:
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	
	std::unique_ptr<RenderItem> skyRitem;
	std::unique_ptr<RenderNode> skyRenderNode;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;
	std::unordered_map<std::string, ComPtr<ID3D12PipelineState>> mPSOs;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	//std::vector<D3D12_INPUT_ELEMENT_DESC> mUIInputLayout;
	
	// Pass
	PassConstants mMainPassCB;

    std::map<std::string, std::shared_ptr<RenderNode>> mAllRenderNodes;

	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;


	// Render items divided by PSO.
	std::vector<RenderItem*> mRitems[(int)RenderLayer::Count];

	DrawParams mDrawParams;

	//int mCurrFrameResourceIndex = 0;

	UINT mTexDiffuseOffset = 0;
	//UINT mTexDiffuseOffset = 0;
	//UINT mTexNormalOffset = 0;
	UINT mTexSkyCubeOffset = 0;
	//UINT mObjCbvOffset = 0;
	UINT mPassCbvOffset = 0;
	//UINT mMatCbvOffset = 0;
	
	//UINT mCbvSrvDescriptorSize = 0;

	bool mIsWireframe = false;
	bool mFbxWireframe = false;
	bool mCameraDetach = false; // True - Camera Move with player

	Light mMainLight;

	POINT mLastMousePos;
	
	Textures mTexDiffuse;
	Textures mTexNormal;
	Textures mTexSkyCube;
	Materials mMaterials;
};