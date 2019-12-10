#pragma once
#include "VulkanPCH.h"
#include "../Math/MathPCH.h"
#include <optional>
#include <array>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> g_ValidationLayers = {
	"VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> g_DeviceExtensions = {
VK_KHR_SWAPCHAIN_EXTENSION_NAME
};



#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct VulkanVertex
{
	vec4 position;
	vec4 color;

	VulkanVertex(vec4 inPos, vec4 inCol)
	{
		position = inPos;
		color = inCol;
	}

	static VkVertexInputBindingDescription GetBindingDescription() 
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(VulkanVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() 
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(VulkanVertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VulkanVertex, color);

		return attributeDescriptions;
	}
};

struct UniformBufferObject
{
	MyMatrix model;
	MyMatrix view;
	MyMatrix proj;
};

const std::vector<VulkanVertex> g_Vertices = {
	VulkanVertex(vec4(-0.5f, -0.5f, 0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)),
	VulkanVertex(vec4(0.5f, -0.5f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f)),
	VulkanVertex(vec4(0.5f, 0.5f, 0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f)),
	VulkanVertex(vec4(-0.5f, 0.5f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f))
};

const std::vector<uint16_t> g_Indices = {
	0, 1, 2, 2, 3, 0
};

class VulkanRenderer {
public:
	void Run();

	GLFWwindow* GetWindow() {
		return m_Window;
	};

private:
	friend class VulkanSubsystem;
#pragma region Window
	GLFWwindow* m_Window;
	VkInstance m_Instance;
	VkQueue m_GraphicsQueue;
	VkQueue m_PresentQueue;
	VkSurfaceKHR m_Surface;
#pragma endregion
#pragma region Devices
	VkDevice m_Device;
	VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
#pragma endregion
#pragma region Swapchain
	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImageView> m_SwapChainImageViews;
	std::vector<VkFramebuffer> m_SwapChainFramebuffers;
#pragma endregion
#pragma region Pipeline
	VkRenderPass m_RenderPass;
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;
	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;
	std::vector<VkSemaphore> m_ImageAvailableSemaphores;
	std::vector<VkSemaphore> m_RenderFinishedSemaphores;
	std::vector<VkFence> m_InFlightFences;
	std::vector<VkFence> m_ImagesInFlight;
	size_t m_CurrentFrame = 0;
#pragma endregion
#pragma region Buffers&Indices
	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexBufferMemory;
	VkBuffer m_IndexBuffer;
	VkDeviceMemory m_IndexBufferMemory;
#pragma endregion
VkDescriptorSetLayout m_DescriptorSetLayout;

	bool CheckValidationLayerSupport();

	std::vector<const char*> GetRequiredExtensions();

	void InitWindow();

	void InitVulkan();

	void PickPhysicalDevice();

	bool IsDeviceSuitable(VkPhysicalDevice device);

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	int RateDeviceSuitability(VkPhysicalDevice device);

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

#pragma region SwapChain

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void CreateSwapChain();

	void CreateImageViews();

#pragma endregion

	void CreateInstance();

	void CreateLogicalDevice();

	void CreateSurface();

	void CreateGraphicsPipeline();

	void CreateRenderPass();

	void CreateFrameBuffers();

	void CreateCommandPool();

	void CreateCommandBuffers();

	void CreateSyncObjects();

	void CreateVertexBuffer();

	void CreateIndexBuffer();

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	void CreateDescriptorSetLayout();

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void MainLoop();

	void DrawFrame();

	void CleanUp();

	void CleanUpSwapChain();

	static std::vector<char> ReadFile(const std::string& filename);

	VkShaderModule CreateShaderModule(const std::vector<char>& code);


	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}
};