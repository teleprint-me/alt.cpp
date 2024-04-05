#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_vulkan.h"
#include "../imgui/imgui.h"
#include <vulkan/vulkan.h>

// ... (initialize Vulkan here)

// #define APP_USE_UNLIMITED_FRAME_RATE
#ifdef _DEBUG
    #define APP_USE_VULKAN_DEBUG_REPORT
#endif

// Data
static VkAllocationCallbacks*   g_Allocator      = nullptr;
static VkInstance               g_Instance       = VK_NULL_HANDLE;
static VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
static VkDevice                 g_Device         = VK_NULL_HANDLE;
static uint32_t                 g_QueueFamily    = (uint32_t)-1;
static VkQueue                  g_Queue          = VK_NULL_HANDLE;
static VkDebugReportCallbackEXT g_DebugReport    = VK_NULL_HANDLE;
static VkPipelineCache          g_PipelineCache  = VK_NULL_HANDLE;
static VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

// Initialize ImGui
ImGui::CreateContext();
ImGui_ImplVulkan_InitInfo init_info = {};
init_info.Instance                  = vkInstance;
init_info.PhysicalDevice            = physicalDevice;
init_info.Device                    = device;
init_info.Queue Families            = queueFamilies;
init_info.Queue                     = graphicsQueue;
init_info.PipelineCache             = VK_NULL_HANDLE;
init_info.DescriptorPoolSize        = {VK_DESCRIPTOR_TYPE_SAMPLER, 1000};
init_info.CheckVkResultFn           = checkVkResult;
ImGui_ImplVulkan_Init(&init_info, renderPass);

// Create a window with ImGui
ImGui::Begin("Hello world!");

// Render the text using ImGui
ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Hello world!");

// End the window
ImGui::End();

// Render the ImGui frame using Vulkan
ImGui_ImplVulkan\_RenderDrawData(ImGui::GetDrawData(), cmdBuf);

// Submit the command buffer and present the rendered image to the screen
vkCmdExecuteCommand(cmdBuf);
vkEndCommandBuffer(cmdBuf);

VkSubmitInfo submitInfo         = {};
submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
submitInfo.commandBufferCount   = 1;
submitInfo.pCommandBuffers      = &cmdBuf;
submitInfo.signalSemaphoreCount = 1;
submitInfo.pSignalSemaphores    = &imageAcquiredSemaphore;

vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

VkPresentInfoKHR presentInfo   = {};
presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
presentInfo.waitSemaphoreCount = 1;
presentInfo.pWaitSemaphores    = &imageAcquiredSemaphore;
presentInfo.swapchainCount     = 1;
presentInfo.pSwapchains        = &swapChain;
presentInfo.pImageIndices      = &currentImageIndex;

vkQueuePresentKHR(graphicsQueue, &presentInfo);
