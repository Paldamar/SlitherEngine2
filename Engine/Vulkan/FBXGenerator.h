#pragma once

#include "VulkanRenderer.h"
#include <string>

class Textures;
//class Materials;
class VulkanFBXGenerator
{
public:
	VulkanFBXGenerator();
	~VulkanFBXGenerator();

	void Begin(VkDevice* device, VkCommandPool* cmdList/*, ID3D12DescriptorHeap * cbvHeap*/);
	void End();
	
	void BuildFBXTexture(/*std::vector<Material>& outMaterial, */std::string inTextureName,  Textures& mTexDiffuse, Textures& mTexturesNormal, Materials& mMaterials);
	
	void LoadFBXStaticGeometry(
								std::string folderPath,
								std::string fbxModelName,
								//std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& mGeometries, 
								Textures& mTexDiffuse, 
								Textures& mTexturesNormal
								/*Materials& mMaterials*/);

	void BuildFBXGeometry(const std::vector<VulkanVertex>& outVertices, const std::vector<std::uint32_t>& outIndices, const std::string& geoName, std::unordered_map<std::string, std::unique_ptr<MeshGeometry>>& mGeometries);

private:
	ID3D12Device * mDevice;
	ID3D12GraphicsCommandList* mCommandList;
	ID3D12DescriptorHeap* mCbvHeap;

	bool mInBeginEndPair;
};

