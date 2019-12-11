#pragma once

#include <fbxsdk.h>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include "../Math/MathPCH.h"
#include "VulkanPCH.h"

struct BoneIndexAndWeight
{
	BYTE mBoneIndices;
	float mBoneWeight;

	bool operator < (const BoneIndexAndWeight& rhs)
	{
		return (mBoneWeight > rhs.mBoneWeight);
	}
};

struct CtrlPoint
{
	vec3 mPosition;
	std::vector<BoneIndexAndWeight> mBoneInfo;
	std::string mBoneName;

	CtrlPoint()
	{
		mBoneInfo.reserve(4);
	}

	void SortBlendingInfoByWeight()
	{
		std::sort(mBoneInfo.begin(), mBoneInfo.end());
	}
};

class FbxLoader
{
public:
	FbxLoader();
	~FbxLoader();

	
	HRESULT LoadFBX(
		std::vector<VulkanVertex>& outVertexVector, 
		std::vector<uint32_t>& outIndexVector, 
		//std::vector<Material>& outMaterial,
		std::string fileName);



	bool LoadMesh(
		std::string fileName,
		std::vector<VulkanVertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector
		/*std::vector<Material>* outMaterial = nullptr*/);


	void GetControlPoints(fbxsdk::FbxNode * pFbxRootNode);
	
	void GetVerticesAndIndice(
		fbxsdk::FbxMesh * pMesh,
		std::vector<VulkanVertex>& outVertexVector,
		std::vector<uint32_t>& outIndexVector);


	void GetMaterials(fbxsdk::FbxNode * pNode/*, std::vector<Material>& outMaterial*/);

	void GetMaterialAttribute(fbxsdk::FbxSurfaceMaterial* pMaterial/*,Material& outMaterial*/);
	
	void GetMaterialTexture(fbxsdk::FbxSurfaceMaterial* pMaterial /*,Material& Mat*/);

	void ExportMesh(std::vector<VulkanVertex>& outVertexVector, std::vector<uint32_t>& outIndexVector, /*std::vector<Material>& outMaterial,*/ std::string fileName);
	
	void clear();

private:
	std::unordered_map<unsigned int, CtrlPoint*> mControlPoints;
};