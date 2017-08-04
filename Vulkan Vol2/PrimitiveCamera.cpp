#include "PrimitiveCamera.h"



PrimitiveCamera::PrimitiveCamera(Logger * logger, LogicDeviceWrapper device, VkSurfaceFormatKHR surfaceFormat, int ScreenX, int ScreenY, float zoom)
	:SurfaceFormat(surfaceFormat), Device(device),MainMatrixes(device, ScreenX, ScreenX, zoom)
{
	this->logger = logger;
	Pipelines.push_back(new PrimitivePipelineWrapper(logger, device, surfaceFormat, MainMatrixes.GetBuffer()));
	Pipelines.push_back(new ColorPipelineWrapper(logger, device, surfaceFormat, MainMatrixes.GetBuffer()));
}

void PrimitiveCamera::keyPressed(uint32_t keyCode)
{
	switch (keyCode)
	{
		case 'w':
		case 'W':
		{
			MainMatrixes.GoForward();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}
		case 's':
		case 'S':
		{
			MainMatrixes.GoBackward();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}
		case 'a':
		case 'A':
		{
			MainMatrixes.StrafeLeft();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}		
		case 'd':
		case 'D':
		{
			MainMatrixes.StrafeRight();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}
		case VK_SPACE:
		{
			MainMatrixes.Up();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}
		case VK_SHIFT:
		{
			MainMatrixes.Down();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}

		case 'q':
		case 'Q':
		{
			MainMatrixes.RotateLeft();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}

		case 'e':
		case 'E':
		{
			MainMatrixes.RotateRight();

			static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			static_cast<ColorPipelineWrapper*>(Pipelines[1])->UpdateViewPtojMatrixDescriptor(MainMatrixes.GetBuffer());
			break;
		}
	default:
		break;
	}
}


void PrimitiveCamera::Draw(VkCommandBuffer CmdBuffer, VkImage image)
{
	VkImageSubresourceRange SubRange;
	SubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	SubRange.baseMipLevel = 0;
	SubRange.levelCount = 1;
	SubRange.baseArrayLayer = 0;
	SubRange.layerCount = 1;

	VkImageViewCreateInfo ColorImageViewInfo{};
	ColorImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ColorImageViewInfo.pNext = NULL;
	ColorImageViewInfo.flags = 0;
	ColorImageViewInfo.image = image;
	ColorImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ColorImageViewInfo.format = SurfaceFormat.format;
	ColorImageViewInfo.subresourceRange = SubRange;

	VkImageView ColorView;
	vkCreateImageView(Device.GetLogicDevice(), &ColorImageViewInfo, nullptr, &ColorView);

	VkCommandBufferBeginInfo BeginInfo;
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.pNext = NULL;
	BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	BeginInfo.pInheritanceInfo = NULL;

	logger->checkResults(vkBeginCommandBuffer(CmdBuffer, &BeginInfo));

	vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<ColorPipelineWrapper*>(Pipelines[1])->GetPipeline());

	static_cast<ColorPipelineWrapper*>(Pipelines[1])->Draw(CmdBuffer, { ColorView });
		vkCmdBindPipeline(CmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->GetPipeline());

		static_cast<PrimitivePipelineWrapper*>(Pipelines[0])->Draw(CmdBuffer, { ColorView });
	logger->checkResults(vkEndCommandBuffer(CmdBuffer));
}

PrimitiveCamera::~PrimitiveCamera()
{
}
