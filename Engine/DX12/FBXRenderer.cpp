#include "DX12PCH.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
//	PSTR cmdLine, int showCmd)
//{
//	// Enable run-time memory check for debug builds.
//#if defined(DEBUG) | defined(_DEBUG)
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//#endif
//
//	try
//	{
//		FBXRenderer theApp(hInstance);
//		if (!theApp.Initialize())
//			return 0;
//
//		return theApp.Run();
//	}
//	catch (DxException& e)
//	{
//		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
//		return 0;
//	}
//}

FBXRenderer::FBXRenderer()
{
	
	if (thisRenderer == nullptr)
		thisRenderer = this;

}

FBXRenderer::~FBXRenderer()
{
	if (md3dDevice != nullptr)
		FlushCommandQueue();
}

//TODO Week 7: Update, Updates Camera, Updates RenderNode CB's, Updates Main Pass CB, Updates Material CB's
void FBXRenderer::Update(const GameTimer& gt)
{
	OnKeyboardInput(gt);

	// Cycle through the circular frame resource array.
	mDrawParams.mCurrFrameResourceIndex = (mDrawParams.mCurrFrameResourceIndex + 1) % gNumFrameResources;
	mCurrFrameResource = mFrameResources[mDrawParams.mCurrFrameResourceIndex].get();

	// Has the GPU finished processing the commands of the current frame resource?
	// If not, wait until the GPU has completed commands up to this fence point.
	if (mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	
	mCamera.UpdateViewMatrix();

	UpdateRenderNodeCBs(gt);
	UpdateMainPassCB(gt);
	UpdateMaterialCB(gt);
}
//TODO Week 7: Updates RenderNode CB's
void FBXRenderer::UpdateRenderNodeCBs(const GameTimer& gt)
{
	for (auto& e : mAllRenderNodes)
	{
		e.second->UpdateRenderNodeCBs(mCurrFrameResource, mMainLight, gt);
	}

	skyRenderNode->UpdateRenderNodeCBs(mCurrFrameResource, mMainLight, gt);
}

//TODO Week 7: Updates Main Pass CB
void FBXRenderer::UpdateMainPassCB(const GameTimer& gt)
{
	XMMATRIX view = mCamera.GetView();
	XMMATRIX proj = mCamera.GetProj();

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	mMainPassCB.EyePosW = mCamera.GetEyePosition3f();
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();
	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.FogColor = { 0.8f, 0.8f, 0.8f, 0.5f };
	mMainPassCB.FogRange = 200.0f;
	mMainPassCB.FogStart = 50.0f;

	mMainPassCB.Lights[0].Direction = mMainLight.Direction;
	mMainPassCB.Lights[0].Strength = mMainLight.Strength;
	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	mMainPassCB.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

//TODO Week 7: Updates Material CB's
void FBXRenderer::UpdateMaterialCB(const GameTimer& gt)
{
	auto currMaterialCB = mCurrFrameResource->MaterialCB.get();
	mMaterials.UpdateMaterialCB(currMaterialCB);
}

//TODO Week 7: Draw
void FBXRenderer::Draw(const GameTimer& gt)
{
	auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(cmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	if (mIsWireframe)
	{
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque_wireframe"].Get()));
	}
	else
	{
		ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mPSOs["opaque"].Get()));
	}

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
	mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

	int passCbvIndex = mPassCbvOffset + mDrawParams.mCurrFrameResourceIndex;
	auto passCbvHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	passCbvHandle.Offset(passCbvIndex, mDrawParams.mCbvSrvDescriptorSize);
	mCommandList->SetGraphicsRootDescriptorTable(5, passCbvHandle);

	for (auto& e : mAllRenderNodes)
	{
		e.second->Draw(mCommandList, mCbvHeap, mDrawParams, mMaterials, mPSOs);
	}

	// SkyTex
	CD3DX12_GPU_DESCRIPTOR_HANDLE skyTexDescriptor(mCbvHeap->GetGPUDescriptorHandleForHeapStart());
	skyTexDescriptor.Offset(mTexSkyCubeOffset, mDrawParams.mCbvSrvDescriptorSize);
	mCommandList->SetGraphicsRootDescriptorTable(2, skyTexDescriptor);

	// Sky 
	mCommandList->SetPipelineState(mPSOs["sky"].Get());
	skyRenderNode->Draw(mCommandList, mCbvHeap, mDrawParams, mMaterials, mPSOs);

	// Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Advance the fence value to mark commands up to this fence point.
	mCurrFrameResource->Fence = ++mCurrentFence;

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	mCommandQueue->Signal(mFence.Get(), mCurrentFence);
}

void FBXRenderer::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void FBXRenderer::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void FBXRenderer::OnMouseMove(WPARAM btnState, int x, int y)
{
	float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
	float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

	if ((btnState & MK_LBUTTON) != 0)
	{
		mCamera.AddPitch(dy);

		//// Rotate Camera only
		mCamera.AddYaw(dx);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		mCamera.AddPitch(dy);
		mCamera.AddYaw(dx);

	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;

}

void FBXRenderer::OnKeyboardInput(const GameTimer& gt)
{
	const float dt = gt.DeltaTime();
	bool isForward = true;
	bool isBackward = true;
	// Input
	if (GetAsyncKeyState('7') & 0x8000)
		mIsWireframe = true;
	else if (GetAsyncKeyState('8') & 0x8000)
		mFbxWireframe = true;
	else if (GetAsyncKeyState('9') & 0x8000)
		mCameraDetach = true;
	else if (GetAsyncKeyState('0') & 0x8000)
		mCameraDetach = false;
	else
	{
		mIsWireframe = false;
		mFbxWireframe = false;
	}

	if (GetAsyncKeyState('W') & 0x8000)
	{
			mCamera.Walk(20.0f * dt);
	}
	else if (GetAsyncKeyState('S') & 0x8000) 
	{
			mCamera.Walk(-20.0f * dt);
	}

}

bool FBXRenderer::Initialize(MsgProc_Callback callback)
{
	if (!Renderer::Initialize(callback))
		return false;

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

	mDrawParams.mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//Step 1 TODO Week 7: Go into this function and study what it does
	LoadTextures();
	//Step 2 TODO Week 7: Go into this function and study what it does
	BuildMaterials();
	//Step 3 TODO Week 7: Go into this function and study what it does
	BuildShapeGeometry();

	//Step 4 TODO Week 7: Go into this function and study what it does
	BuildRenderNodes();

	//Step 5 TODO Week 7: Go into this function and study what it does
	BuildRootSignature();
	//Step 6 TODO Week 7: Go into this function and study what it does
	BuildDescriptorHeaps();
	//Step 7 TODO Week 7: Go into this function and study what it does
	BuildShadersAndInputLayout();
	//Step 8 TODO Week 7: Go into this function and study what it does
	BuildFrameResources();

	//Step 9 TODO Week 7: Go into this function and study what it does
	BuildTextureBufferViews();
	//Step 10 TODO Week 7: Go into this function and study what it does
	BuildConstantBufferViews();

	//Step 11 TODO Week 7: Go into this function and study what it does
	BuildPSOs();

	// Execute the initialization commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	//Step 12 TODO Week 7: Go into this function and study what it does
	// Wait until initialization is complete.
	OnResize();

	return true;
}

//Step 1
void FBXRenderer::LoadTextures()
{
	mTexDiffuse.Begin(md3dDevice.Get(), mCommandList.Get(), mCbvHeap.Get());

	std::vector<std::string> texNames;
	std::vector<std::wstring> texPaths;

	texNames.push_back("bricksTex");
	texPaths.push_back(L"../Resource/Textures/bricks.dds");

	texNames.push_back("bricks3Tex");
	texPaths.push_back(L"../Resource/Textures/bricks3.dds");
	
	texNames.push_back("stoneTex");
	texPaths.push_back(L"../Resource/Textures/stone.dds");

	texNames.push_back("tundraTex");
	texPaths.push_back(L"../Resource/Textures/tundra.jpg");

	texNames.push_back("iceTex");
	texPaths.push_back(L"../Resource/Textures/ice.dds");

	texNames.push_back("redTex");
	texPaths.push_back(L"../Resource/Textures/red.png");

	mTexDiffuse.SetTexture(texNames, texPaths);

	mTexDiffuse.End();

	mTexNormal.Begin(md3dDevice.Get(), mCommandList.Get(), mCbvHeap.Get());

	for (int i = 0; i < texPaths.size(); ++i)
	{
		std::wstring TextureNormalFileName;
		TextureNormalFileName = texPaths[i].substr(0, texPaths[i].size() - 4);
		TextureNormalFileName.append(L"_normal.jpg");

		struct stat buffer;
		std::string fileCheck;
		fileCheck.assign(TextureNormalFileName.begin(), TextureNormalFileName.end());
		if (stat(fileCheck.c_str(), &buffer) == 0)
		{
			mTexNormal.SetTexture(
				texNames[i],
				TextureNormalFileName);
		}
	}

	mTexNormal.End();

	// Cube Map
	mTexSkyCube.Begin(md3dDevice.Get(), mCommandList.Get(), mCbvHeap.Get());

	mTexSkyCube.SetTexture("skyCubeMap", L"../Resource/Textures/snowcube1024.dds");

	mTexSkyCube.End();
}

//Step 2
void FBXRenderer::BuildMaterials()
{
	int MatIndex = mMaterials.GetSize();

	std::vector<std::string> matName;
	std::vector<XMFLOAT4> diffuses;
	std::vector<XMFLOAT3> fresnels;
	std::vector<float> roughnesses;
	std::vector<int> texIndices;
	std::vector<int> texNormalIndices;

	matName.push_back("bricks0");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("bricksTex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	fresnels.push_back(XMFLOAT3(0.02f, 0.02f, 0.02f));
	roughnesses.push_back(0.1f);

	matName.push_back("bricks3");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("bricks3Tex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	fresnels.push_back(XMFLOAT3(0.02f, 0.02f, 0.02f));
	roughnesses.push_back(0.1f);

	matName.push_back("stone0");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("stoneTex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	fresnels.push_back(XMFLOAT3(0.05f, 0.05f, 0.05f));
	roughnesses.push_back(0.3f);

	matName.push_back("tundra0");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("tundraTex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	fresnels.push_back(XMFLOAT3(0.05f, 0.02f, 0.02f));
	roughnesses.push_back(0.1f);

	matName.push_back("ice0");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("iceTex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	fresnels.push_back(XMFLOAT3(0.05f, 0.02f, 0.02f));
	roughnesses.push_back(0.1f);

	matName.push_back("red");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("redTex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	fresnels.push_back(XMFLOAT3(0.05f, 0.02f, 0.02f));
	roughnesses.push_back(0.1f);

	matName.push_back("Transparency");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("redTex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f));
	fresnels.push_back(XMFLOAT3(0.05f, 0.02f, 0.02f));
	roughnesses.push_back(0.1f);

	matName.push_back("shadow0");
	texIndices.push_back(mTexDiffuse.GetTextureIndex("redTex"));
	texNormalIndices.push_back(mTexNormal.GetTextureIndex("bricksTex"));
	diffuses.push_back(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f));
	fresnels.push_back(XMFLOAT3(0.001f, 0.001f, 0.001f));
	roughnesses.push_back(0.0f);

	mMaterials.SetMaterial(
		matName, diffuses, fresnels, roughnesses,
		MatIndex, texIndices, texNormalIndices);


	// CUBE MAP
	mMaterials.SetMaterial(
		"sky", XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f), XMFLOAT3(0.001f, 0.001f, 0.001f), 0.0f,
		mMaterials.GetSize(), mTexSkyCubeOffset);
	
}

//Step 3
void FBXRenderer::BuildShapeGeometry()
{
	mMainLight.Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainLight.Strength = { 0.6f, 0.6f, 0.6f };

	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.5f, 0.5f, 1.5f, 3);
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(1000.0f, 1000.0f, 200, 200);
	GeometryGenerator::MeshData hpBar = geoGen.CreateGrid(20.0f, 20.0f, 20, 20);
	GeometryGenerator::MeshData sphere = geoGen.CreateGeosphere(0.5f, 3);
	GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);

	//
	// We are concatenating all the geometry into one big vertex/index buffer.  So
	// define the regions in the buffer each submesh covers.
	//

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT boxVertexOffset = 0;
	UINT gridVertexOffset = (UINT)box.Vertices.size();
	UINT hpBarVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
	UINT sphereVertexOffset = hpBarVertexOffset + (UINT)hpBar.Vertices.size();
	UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	UINT boxIndexOffset = 0;
	UINT gridIndexOffset = (UINT)box.Indices32.size();
	UINT hpBarIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
	UINT sphereIndexOffset = hpBarIndexOffset + (UINT)hpBar.Indices32.size();
	UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();

	// Define the SubmeshGeometry that cover different 
	// regions of the vertex/index buffers.
	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = gridIndexOffset;
	gridSubmesh.BaseVertexLocation = gridVertexOffset;

	SubmeshGeometry hpBarSubmesh;
	hpBarSubmesh.IndexCount = (UINT)hpBar.Indices32.size();
	hpBarSubmesh.StartIndexLocation = hpBarIndexOffset;
	hpBarSubmesh.BaseVertexLocation = hpBarVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry cylinderSubmesh;
	cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
	cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
	cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	auto totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		hpBar.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
		vertices[k].TexC = box.Vertices[i].TexC;
	}

	for (size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
		vertices[k].TexC = grid.Vertices[i].TexC;
	}

	for (size_t i = 0; i < hpBar.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = hpBar.Vertices[i].Position;
		vertices[k].Normal = hpBar.Vertices[i].Normal;
		vertices[k].TexC = hpBar.Vertices[i].TexC;
	}

	for (size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
		vertices[k].TexC = sphere.Vertices[i].TexC;
	}

	for (size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
		vertices[k].TexC = cylinder.Vertices[i].TexC;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(hpBar.GetIndices16()), std::end(hpBar.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "shapeGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArgs["grid"] = gridSubmesh;
	geo->DrawArgs["hpBar"] = hpBarSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	geo->DrawArgs["cylinder"] = cylinderSubmesh;

	mGeometries[geo->Name] = std::move(geo);
}

//Step 4 TODO Week 7: Creating the RenderNodes
void FBXRenderer::BuildRenderNodes()
{
	UINT objCBIndex = -1;

	//Uses Standard Geometry
	skyRenderNode = std::make_unique<RenderNode>();
	skyRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&skyRitem->World, XMMatrixScaling(5000.0f, 5000.0f, 5000.0f) * XMMatrixTranslation(0, 0, 0));
	skyRitem->TexTransform = MathHelper::Identity4x4();
	skyRitem->ObjCBIndex = ++objCBIndex;
	skyRitem->Mat = mMaterials.Get("sky");
	skyRitem->Geo = mGeometries["shapeGeo"].get();
	skyRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skyRitem->IndexCount = skyRitem->Geo->DrawArgs["sphere"].IndexCount;
	skyRitem->StartIndexLocation = skyRitem->Geo->DrawArgs["sphere"].StartIndexLocation;
	skyRitem->BaseVertexLocation = skyRitem->Geo->DrawArgs["sphere"].BaseVertexLocation;
	skyRenderNode->AddRenderItem(std::move(skyRitem), RenderLayer::Opaque);

	// Ground
	//Uses Standard Geometry
	float z = 500.0f;
	auto groundRenderNode = std::make_unique<RenderNode>();
	for (int i = 0; i < 4; ++i)
	{
		auto subRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&subRitem->TexTransform, XMMatrixScaling(30.0f, 30.0f, 1.0f));
		subRitem->ObjCBIndex = ++objCBIndex;
		subRitem->Geo = mGeometries["shapeGeo"].get();
		subRitem->Mat = mMaterials.Get("tundra0");
		subRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		subRitem->IndexCount = subRitem->Geo->DrawArgs["grid"].IndexCount;
		subRitem->StartIndexLocation = subRitem->Geo->DrawArgs["grid"].StartIndexLocation;
		subRitem->BaseVertexLocation = subRitem->Geo->DrawArgs["grid"].BaseVertexLocation;
		subRitem->Geo->DrawArgs["grid"].Bounds.Transform(subRitem->Bounds, XMLoadFloat4x4(&subRitem->World));

		z *= -1.0f;
		if (i > 1)
		{
			XMStoreFloat4x4(&subRitem->World, XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixTranslation(-500.0f, -0.1f, z));

			if (i == 3)
				subRitem->Mat = mMaterials.Get("stone0");
		}
		else
		{
			XMStoreFloat4x4(&subRitem->World, XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixTranslation(500.0f, -0.1f, z));

			if (i == 0)
				subRitem->Mat = mMaterials.Get("ice0");
		}

		groundRenderNode->AddRenderItem(std::move(subRitem), RenderLayer::Opaque);
	}

	mAllRenderNodes["groundRenderNode"] = std::move(groundRenderNode);

	BuildRenderNodeFbxGeometry("../Resource/FBX/Architecture/houseA/", "house");
	//// House
	auto houseRenderNode1 = std::make_unique<RenderNode>();
	XMMATRIX worldSR = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationRollPitchYaw(-XM_PIDIV2, -XM_PIDIV2, 0.0f);

	houseRenderNode1->BuildSubRitems(
		"house",
		"house0",
		RenderLayer::RenderNode,
		++objCBIndex,
		worldSR * XMMatrixTranslation(-300.0f, 0.0f, -200.0f),
		XMMatrixIdentity(), mMaterials, mGeometries);

	mAllRenderNodes["houseRenderNode1"] = std::move(houseRenderNode1);

	auto houseRenderNode2 = std::make_unique<RenderNode>();
	worldSR = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationRollPitchYaw(-XM_PIDIV2, -XM_PIDIV2, 0.0f);

	houseRenderNode2->BuildSubRitems(
		"house",
		"house0",
		RenderLayer::RenderNode,
		++objCBIndex,
		worldSR * XMMatrixTranslation(0.0f, 0.0f, 0.0f),
		XMMatrixIdentity(), mMaterials, mGeometries);

	mAllRenderNodes["houseRenderNode2"] = std::move(houseRenderNode2);

	//// Rocks

	BuildRenderNodeFbxGeometry("../Resource/FBX/Architecture/Rocks/RockCluster/", "RockCluster");
	auto rocksRenderNode1 = std::make_unique<RenderNode>();

	rocksRenderNode1->BuildSubRitems(
		"RockCluster",
		"RockCluster0",
		RenderLayer::RenderNode,
		++objCBIndex,
		XMMatrixScaling(10.0f, 10.0f, 10.0f) * XMMatrixRotationRollPitchYaw(-XM_PIDIV2, 0.0f, 0.0f) * XMMatrixTranslation(-100.0f, 3.0f, -200.0f),
		XMMatrixIdentity(), mMaterials, mGeometries);

	mAllRenderNodes["rocksRenderNode1"] = std::move(rocksRenderNode1);

	BuildRenderNodeFbxGeometry("../Resource/FBX/Architecture/Rocks/Rock/", "Rock");
	auto rocksRenderNode2 = std::make_unique<RenderNode>();
	rocksRenderNode2->BuildSubRitems(
		"Rock",
		"Rock0",
		RenderLayer::RenderNode,
		++objCBIndex,
		XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationRollPitchYaw(XM_PI, 0.0f, XM_PIDIV2) * XMMatrixTranslation(350.0f, 0.0f, 250.0f),
		XMMatrixIdentity(), mMaterials, mGeometries);
	mAllRenderNodes["rocksRenderNode2"] = std::move(rocksRenderNode2);
}

void FBXRenderer::BuildRenderNodeFbxGeometry(std::string folderPath, std::string fbxModelName)
{
	FBXGenerator fbxGen;

	fbxGen.Begin(md3dDevice.Get(), mCommandList.Get(), mCbvHeap.Get());

	fbxGen.LoadFBXStaticGeometry(folderPath, fbxModelName, mGeometries, mTexDiffuse, mTexNormal, mMaterials);

	fbxGen.End();
}

//Step 5 TODO Week 7: Have a look at the Common.hlsl shader and Default.hlsl Shader
void FBXRenderer::BuildRootSignature()
{
	const int texTableCount = 3;
	const int cbvTableCount = 3; 
	const int tableNumber = texTableCount + cbvTableCount;

	CD3DX12_DESCRIPTOR_RANGE texTable[texTableCount];
	CD3DX12_DESCRIPTOR_RANGE cbvTable[cbvTableCount];

	for (int i = 0; i < texTableCount; ++i)
		texTable[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);
	for (int i = 0; i < cbvTableCount; ++i)
		cbvTable[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i);


	// Root parameter can be a table, root descriptor or root constants.
	// Objects, Materials, Passes
	CD3DX12_ROOT_PARAMETER slotRootParameter[tableNumber];

	// Create root CBVs.
	for (int i = 0; i < texTableCount; ++i)
		slotRootParameter[i].InitAsDescriptorTable(1, &texTable[i], D3D12_SHADER_VISIBILITY_PIXEL);
	for (int i = texTableCount; i < tableNumber; ++i)
		slotRootParameter[i].InitAsDescriptorTable(1, &cbvTable[i - texTableCount]);


	auto staticSamplers = GetStaticSamplers();

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(tableNumber, slotRootParameter,
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf())));

}

//Step 6 TODO Week 7: Create our Descriptor Heap, And Set the offsets
void FBXRenderer::BuildDescriptorHeaps()
{
	UINT texCount = mTexDiffuse.GetSize();
	UINT texNormalCount = mTexDiffuse.GetSize();
	UINT texSkyCount = mTexSkyCube.GetSize();
	UINT objCount = Renderer::GetRenderer()->RenderItemSize;
	UINT passCount = 1;
	UINT matCount = mMaterials.GetSize();

	// Need a CBV descriptor for each object for each frame resource,
	// +1 for the perPass CBV for each frame resource.
	// +matCount for the Materials for each frame resources.
	UINT numDescriptors = texCount + texNormalCount + texSkyCount + (objCount + passCount + matCount) * gNumFrameResources;


	// Save an offset to the start of the pass CBVs.  These are the last 3 descriptors.
	mDrawParams.mTexNormalOffset = texCount;
	mTexSkyCubeOffset = texCount + texNormalCount;
	mDrawParams.mObjCbvOffset = mTexSkyCubeOffset + texSkyCount;
	mDrawParams.mMatCbvOffset = objCount * gNumFrameResources + mDrawParams.mObjCbvOffset;
	mPassCbvOffset = matCount * gNumFrameResources + mDrawParams.mMatCbvOffset;

	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = numDescriptors;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&mCbvHeap)));
}

//Step 7 TODO Week 7: Create our Input Layout, We will be using 2 shaders this time
void FBXRenderer::BuildShadersAndInputLayout()
{

	mShaders["standardVS"] = d3dUtil::CompileShader(L"..\\Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"..\\Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");

	mShaders["skyVS"] = d3dUtil::CompileShader(L"..\\Shaders\\Sky.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["skyPS"] = d3dUtil::CompileShader(L"..\\Shaders\\Sky.hlsl", nullptr, "PS", "ps_5_1");

	mInputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

//Step 8 TODO Week 7: Create Our Frame Resources
void FBXRenderer::BuildFrameResources()
{
	for (int i = 0; i < gNumFrameResources; ++i)
	{
		mFrameResources.push_back(std::make_unique<FrameResource>(
			md3dDevice.Get(),
			1, (UINT)Renderer::GetRenderer()->RenderItemSize,
			mMaterials.GetSize()));
	}
}

//Step 9 TODO Week 7: Create Our texture Buffer Views
void FBXRenderer::BuildTextureBufferViews()
{
	//Diffuse
	mTexDiffuse.Begin(md3dDevice.Get(), mCommandList.Get(), mCbvHeap.Get());
	mTexDiffuse.BuildConstantBufferViews(Textures::Type::TWO_DIMENTION);
	mTexDiffuse.End();

	//Normal
	mTexNormal.Begin(md3dDevice.Get(), mCommandList.Get(), mCbvHeap.Get());
	mTexNormal.BuildConstantBufferViews(Textures::Type::TWO_DIMENTION, mDrawParams.mTexNormalOffset);
	mTexNormal.End();

	//Sky Cube
	mTexSkyCube.Begin(md3dDevice.Get(), mCommandList.Get(), mCbvHeap.Get());
	mTexSkyCube.BuildConstantBufferViews(Textures::Type::CUBE, mTexSkyCubeOffset);
	mTexSkyCube.End();
}

//Step 10 TODO Week 7: Create Our Constant Buffer Views
void FBXRenderer::BuildConstantBufferViews()
{
	// Object 
	BuildConstantBufferViews(mDrawParams.mObjCbvOffset, Renderer::GetRenderer()->RenderItemSize, sizeof(ObjectConstants), eUploadBufferIndex::ObjectCB);

	// Material 
	BuildConstantBufferViews(mDrawParams.mMatCbvOffset, mMaterials.GetSize(), sizeof(MaterialConstants), eUploadBufferIndex::MaterialCB);

	// Pass 
	// Last three descriptors are the pass CBVs for each frame resource.
	BuildConstantBufferViews(mPassCbvOffset, 1, sizeof(PassConstants), eUploadBufferIndex::PassCB);

}
//Step 10 TODO Week 7: Create a Constant Buffer View, Called By the Above Function BuildConstantBufferViews()
void FBXRenderer::BuildConstantBufferViews(int cbvOffset, UINT itemSize, UINT cbSize, eUploadBufferIndex e)
{
	UINT CBByteSize = d3dUtil::CalcConstantBufferByteSize(cbSize);

	for (int frameIndex = 0; frameIndex < gNumFrameResources; ++frameIndex)
	{
		auto cbResource = FrameResource::GetResourceByIndex(mFrameResources[frameIndex].get(), e);

		for (UINT i = 0; i < itemSize; ++i)
		{
			D3D12_GPU_VIRTUAL_ADDRESS cbAddress = cbResource->GetGPUVirtualAddress();

			// Offset to the ith object constant buffer in the buffer.
			cbAddress += i * CBByteSize;

			// Offset to the object cbv in the descriptor heap.
			int heapIndex = cbvOffset + frameIndex * itemSize + i;
			auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(mCbvHeap->GetCPUDescriptorHandleForHeapStart());
			handle.Offset(heapIndex, mDrawParams.mCbvSrvDescriptorSize);

			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
			cbvDesc.BufferLocation = cbAddress;
			cbvDesc.SizeInBytes = CBByteSize;

			md3dDevice->CreateConstantBufferView(&cbvDesc, handle);
		}
	}
}


//Step 11 TODO Week 7: Build our PSO's 
void FBXRenderer::BuildPSOs()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	//
	// PSO for opaque objects.
	//
	ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	opaquePsoDesc.pRootSignature = mRootSignature.Get();
	opaquePsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()),
		mShaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};

	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = mDepthStencilFormat;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mPSOs["opaque"])));

	// PSO for opaque wireframe objects.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC opaqueWireframePsoDesc = opaquePsoDesc;
	opaqueWireframePsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaqueWireframePsoDesc, IID_PPV_ARGS(&mPSOs["opaque_wireframe"])));


	// PSO for sky.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC skyPsoDesc = opaquePsoDesc;

	// The camera is inside the sky sphere, so just turn off culling.
	skyPsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	skyPsoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	skyPsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	skyPsoDesc.pRootSignature = mRootSignature.Get();
	skyPsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mShaders["skyVS"]->GetBufferPointer()),
		mShaders["skyVS"]->GetBufferSize()
	};
	skyPsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mShaders["skyPS"]->GetBufferPointer()),
		mShaders["skyPS"]->GetBufferSize()
	};
	ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&skyPsoDesc, IID_PPV_ARGS(&mPSOs["sky"])));

}

//Step 12
void FBXRenderer::OnResize()
{
	Renderer::OnResize();

	// The window resized, so update the aspect ratio and recompute the projection matrix.
	//XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

	mCamera.SetProj(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);

}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> FBXRenderer::GetStaticSamplers()
{
	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}
