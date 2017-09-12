#include "PrimitiveMaterial.h"



PrimitiveMaterial::PrimitiveMaterial(Logger *logger, LogicDeviceWrapper device, string normalMapName, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, string imageName)
	:diffuse(diffuse), specular(specular), ambient(ambient), Device(device)
{
	this->logger = logger;
	
	NormalImage = CreateImage(normalMapName);
	NormalMap = CreateImageView(NormalImage);

	TextureImage = CreateImage(imageName);
	TextureView = CreateImageView(TextureImage);
}

VkImageView PrimitiveMaterial::GetNormalMap()
{
	return NormalMap;
}

VkImageView PrimitiveMaterial::GetTexture()
{
	return TextureView;
}

VkImage PrimitiveMaterial::CreateImage(string normalMapName)
{
	int texWidth, texHeight, texChannels;
	stbi_uc* NormalMapPixels = stbi_load(normalMapName.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	VkImageCreateInfo NormalImageInfo;
	NormalImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	NormalImageInfo.pNext = NULL;
	NormalImageInfo.flags = 0;
	NormalImageInfo.imageType = VK_IMAGE_TYPE_2D;
	NormalImageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	NormalImageInfo.extent.height = texHeight;
	NormalImageInfo.extent.width = texWidth;
	NormalImageInfo.extent.depth = 1;
	NormalImageInfo.mipLevels = 1;
	NormalImageInfo.arrayLayers = 1;
	NormalImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	NormalImageInfo.tiling = VK_IMAGE_TILING_LINEAR;
	NormalImageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	vector<uint32_t> indexes;
	if (Device.GetQueuesFamilies().size() == 1)
	{
		NormalImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		NormalImageInfo.queueFamilyIndexCount = 1;
		uint32_t index = Device.GetQueuesFamilies()[0].Index;
		NormalImageInfo.pQueueFamilyIndices = &index;
	}
	else
	{
		NormalImageInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
		NormalImageInfo.queueFamilyIndexCount = Device.GetQueuesFamilies().size();

		for (int i(0); i < Device.GetQueuesFamilies().size(); i++)
		{
			indexes.push_back(Device.GetQueuesFamilies()[i].Index);
		}
		NormalImageInfo.pQueueFamilyIndices = indexes.data();
	}
	NormalImageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;// VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkImage Image;
	logger->checkResults(vkCreateImage(Device.GetLogicDevice(), &NormalImageInfo, nullptr, &Image));

	VkMemoryRequirements memReqs;
	vkGetImageMemoryRequirements(Device.GetLogicDevice(), Image, &memReqs);

	VkMemoryAllocateInfo MemoryAllocInfo;
	MemoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	MemoryAllocInfo.pNext = NULL;
	MemoryAllocInfo.allocationSize = memReqs.size;
	MemoryAllocInfo.memoryTypeIndex = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;// | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT

	VkDeviceMemory memory;
	vkAllocateMemory(Device.GetLogicDevice(), &MemoryAllocInfo, nullptr, &memory);

	vkBindImageMemory(Device.GetLogicDevice(), Image, memory, 0);



	VkImageSubresource subresource = {};
	subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource.mipLevel = 0;
	subresource.arrayLayer = 0;

	VkSubresourceLayout stagingImageLayout;
	vkGetImageSubresourceLayout(Device.GetLogicDevice(), Image, &subresource, &stagingImageLayout);

	void* data;
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	vkMapMemory(Device.GetLogicDevice(), memory, 0, imageSize, 0, &data);

	if (stagingImageLayout.rowPitch == texWidth * 4) {
		memcpy(data, NormalMapPixels, (size_t)imageSize);
	}
	else {
		uint8_t* dataBytes = reinterpret_cast<uint8_t*>(data);

		for (int y = 0; y < texHeight; y++) {
			memcpy(&dataBytes[y * stagingImageLayout.rowPitch], &NormalMapPixels[y * texWidth * 4], texWidth * 4);
		}
	}

	vkUnmapMemory(Device.GetLogicDevice(), memory);
	return Image;
}

VkImageView PrimitiveMaterial::CreateImageView(VkImage Image)
{
	VkImageSubresourceRange SubRange;
	SubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	SubRange.baseMipLevel = 0;
	SubRange.levelCount = 1;
	SubRange.baseArrayLayer = 0;
	SubRange.layerCount = 1;

	VkImageViewCreateInfo NormalImageViewInfo{};
	NormalImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	NormalImageViewInfo.pNext = NULL;
	NormalImageViewInfo.flags = 0;
	NormalImageViewInfo.image = Image;
	NormalImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	NormalImageViewInfo.format = NormalFormat;
	NormalImageViewInfo.subresourceRange = SubRange;

	VkImageView ImageView;
	vkCreateImageView(Device.GetLogicDevice(), &NormalImageViewInfo, nullptr, &ImageView);
	return ImageView;
}

PrimitiveMaterial::~PrimitiveMaterial()
{
}
