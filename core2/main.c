#define LALALA_IMPLEMENTATION
#include "lalala.h"

#define GLASS_IMPLEMENTATION
#include "glass.h"

#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>
#include <sys/stat.h>

#define MAX_FRAMES_IN_FLIGHT (2)

struct glass_vertex
{
	lll_v2  position;
	lll_v3  color;
};

struct glass_vertex  vertices[] = {
	{{.x =  0.0f, .y = -0.5f}, {.r = 1.0f, .g = 0.0f, .b = 0.0f}},
	{{.x =  0.5f, .y =  0.5f}, {.r = 0.0f, .g = 1.0f, .b = 0.0f}},
	{{.x = -0.5f, .y =  0.5f}, {.r = 0.0f, .g = 0.0f, .b = 1.0f}},
};

#ifdef DEBUG
    const lll_b8 enable_validation_layers = LLL_TRUE;
#else
    const lll_b8 enable_validation_layers = LLL_FALSE;
#endif
static char*  validation_layers[] = {"VK_LAYER_KHRONOS_validation"};
static char*  required_device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

static void vk_framebuffer_resize_callback(GLFWwindow* window, lll_i32 width, lll_i32 height)
{
	(void) width;
	(void) height;
	lll_b8* is_framebuffer_resized = glfwGetWindowUserPointer(window);
	*is_framebuffer_resized = LLL_TRUE;
}

static lll_b8	vk_check_validation_layer_support(lll_arena* arena)
{
	lll_u32 layer_count;
	lll_b8 layer_found = LLL_FALSE;
	vkEnumerateInstanceLayerProperties(&layer_count, NULL);
	if (layer_count > 0)
	{
		layer_found = LLL_TRUE;
		lll_arena_snapshot  snapshot = lll_arena_cheese(arena);
		VkLayerProperties* avaliable_layers = lll_arena_alloc(arena, sizeof(VkLayerProperties) * layer_count, 4);
		vkEnumerateInstanceLayerProperties(&layer_count, avaliable_layers);
		char** validation_layer = validation_layers;
		for (lll_u32 i = 0; i < (sizeof(validation_layers) / sizeof(char*)); i++)
		{
			lll_b8 layer_found_here = LLL_TRUE;
			lll_string validation_layer_name = {*validation_layer, lll_strlen(*validation_layer)};
			VkLayerProperties* avaliable_layer = avaliable_layers;
			for (lll_u32 j = 0; j < layer_count; j++)
			{
				if (lll_string_is_equal(validation_layer_name, (lll_string) {avaliable_layer->layerName, lll_strlen(avaliable_layer->layerName)}))
				{
					layer_found_here = LLL_TRUE;
					break;
				}
				avaliable_layer++;
			}
			if (!layer_found_here)
			{
				layer_found = LLL_FALSE;
				break;
			}
			validation_layer++;
		}
		lll_arena_rollback(arena, snapshot);
	}
	return layer_found;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data)
{
	(void) message_severity;
	(void) message_type;
	(void) user_data;
	lll_fdprintf(STDERR_FILENO, "Validation: %s\n", callback_data->pMessage);
	return VK_FALSE;
}

lll_b8 vk_create_instance(VkInstance* output, lll_arena* temp_arena)
{
	lll_assert(*output == VK_NULL_HANDLE, "VkInstance should be zero-initialized");
	VkApplicationInfo app_info = {0};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Hello Triangle";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo create_info = {0};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	uint32_t glfw_extension_count = 0;
	const char** glfw_extensions;
	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	// Note: Beginning with the 1.3.216 Vulkan SDK, the VK_KHR_PORTABILITY_subset extension is mandatory. So add this extension here
	char** required_extensions = lll_arena_alloc(temp_arena, (glfw_extension_count + 2) * sizeof(char*), 1);
	for(lll_u32 i = 0; i < glfw_extension_count; i++)
	{
		required_extensions[i] = ((char**) glfw_extensions)[i];
	}
	required_extensions[glfw_extension_count] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
	create_info.ppEnabledExtensionNames = (const char**) required_extensions;
	if (enable_validation_layers)
	{
		required_extensions[glfw_extension_count + 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
		create_info.enabledExtensionCount = glfw_extension_count + 2;
	}
	else
	{
		create_info.enabledExtensionCount = glfw_extension_count + 1;
	}
	create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {0};
	// Note: validation layer setup
	if (enable_validation_layers)
	{
		if (!vk_check_validation_layer_support(temp_arena))
		{
			LLL_PRINT_ERROR("Error: Validation layer requested but not avaliable\n");
			return LLL_FALSE;
		}
		else
		{
			create_info.enabledLayerCount = sizeof(validation_layers) / sizeof(char*);
			create_info.ppEnabledLayerNames = (const char**) validation_layers;

			debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debug_create_info.pfnUserCallback = vk_debug_callback;
			debug_create_info.pUserData = NULL;
			create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
		}
	}
	else
	{
		create_info.enabledLayerCount = 0;
	}

	VkResult res = vkCreateInstance(&create_info, NULL, output);
	if (res == VK_ERROR_INCOMPATIBLE_DRIVER)
	{
		LLL_PRINT_ERROR("Error: Failed to create Vulkan Instance due to no found compatible Vulkan ICD\n");
		return LLL_FALSE;
	}
	else if (res)
	{
		LLL_PRINT_ERROR("Error: Failed to create Vulkan Instance due to unknown reasons\n");
		return LLL_FALSE;
	}
	lll_arena_clear(temp_arena);
	return LLL_TRUE;
}

lll_b8 vk_create_debug_messenger(VkInstance instance, VkDebugUtilsMessengerEXT* output)
{
	VkDebugUtilsMessengerCreateInfoEXT  debug_create_info = {0};
	debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	debug_create_info.pfnUserCallback = vk_debug_callback;
	debug_create_info.pUserData = NULL;
	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if ((func == NULL) || (func(instance, &debug_create_info, NULL, output) != VK_SUCCESS))
	{
		LLL_PRINT_ERROR("Warning: Failed to set up messege debugger\n");
		return LLL_FALSE;
	}
	return LLL_TRUE;
}

struct vk_queue_family_indices
{
	union
	{
		struct
		{
			lll_b8   is_valid_graphics_family : 1;
			lll_b8   is_valid_present_family : 1;
		};
		lll_u32    flags;
	};
	union
	{
		struct
		{
			lll_u32  graphics_family_index;
			lll_u32  present_family_index;
		};
		lll_u32	family_indices[2];
	};
};

struct vk_queue_family_indices vk_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface, lll_arena* arena)
{
	struct vk_queue_family_indices indices = {0};
	lll_u32  queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
	if (queue_family_count > 0)
	{
		lll_arena_snapshot  snapshot = lll_arena_cheese(arena);
		VkQueueFamilyProperties* queue_families = lll_arena_alloc(arena, sizeof(VkQueueFamilyProperties) * queue_family_count, 4);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);
		VkQueueFamilyProperties* queue_family = queue_families;
		for (lll_u32 i = 0; i < queue_family_count; i++)
		{
			if (queue_family->queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				indices.is_valid_graphics_family = LLL_TRUE;
				indices.graphics_family_index = i;
			}
			lll_b32 has_present_support = LLL_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &has_present_support);
			if (has_present_support)
			{
				indices.is_valid_present_family = LLL_TRUE;
				indices.present_family_index = i;
			}
			if (indices.is_valid_graphics_family && indices.is_valid_present_family)
			{
				break;
			}
			queue_family++;
		}
		lll_arena_rollback(arena, snapshot);
	}
	return indices;
}

struct vk_swapchain_support_details
{
	VkSurfaceCapabilitiesKHR  capabilities;
	VkSurfaceFormatKHR*       formats;
	VkPresentModeKHR*         present_modes;
	lll_u32                   formats_size;
	lll_u32                   present_modes_size;
};

void	vk_query_swapchain_support(VkPhysicalDevice device, VkSurfaceKHR surface, struct vk_swapchain_support_details* details, lll_arena* arena)
{
	struct vk_swapchain_support_details result = {0};
	lll_assert(device != VK_NULL_HANDLE, "Physical device should not be NULL");
	lll_assert(surface != VK_NULL_HANDLE, "Surface should not be NULL");
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &result.capabilities);
	lll_u32  format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, NULL);
	if (format_count > 0)
	{
		result.formats = lll_arena_alloc(arena, sizeof(VkSurfaceFormatKHR) * format_count, 4);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, result.formats);
		result.formats_size = format_count;
	}
	lll_u32  present_modes_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes_count, NULL);
	if (present_modes_count > 0)
	{
		result.present_modes = lll_arena_alloc(arena, sizeof(VkPresentModeKHR) * present_modes_count, 4);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_modes_count, result.present_modes);
		result.present_modes_size = present_modes_count;
	}
	*details = result;
}

lll_b8 vk_find_suitable_physical_device(VkInstance instance, VkSurfaceKHR surface, VkPhysicalDevice* output_physical_device, struct vk_queue_family_indices* output_queue_family_indices, lll_arena* temp_arena)
{
	lll_assert(instance != VK_NULL_HANDLE, "VkInstance should not be NULL");
	lll_assert(surface != VK_NULL_HANDLE, "VkSurfaceKHR should not be NULL");
	lll_assert(*output_physical_device == VK_NULL_HANDLE, "VkPhysicalDevice should be zero-initialized");
	lll_assert(output_queue_family_indices->flags == 0, "struct vk_queue_family_indices should be zero-initialized");
	lll_u32 device_count = 0;
	vkEnumeratePhysicalDevices(instance, &device_count, NULL);
	if (device_count == 0)
	{
		LLL_PRINT_ERROR("Error: Failed to find GPU with Vulkan support\n");
		return LLL_FALSE;
	}
	lll_printf("%u physical device avaliable\n", device_count);
	VkPhysicalDevice* devices = lll_arena_alloc(temp_arena, sizeof(VkPhysicalDevice) * device_count, 8);
	vkEnumeratePhysicalDevices(instance, &device_count, devices);
	for (lll_u32 i = 0; i < device_count; i++)
	{
		lll_b8 is_suitable = LLL_FALSE;
		// 	VkPhysicalDeviceProperties deviceProperties;
		// 	vkGetPhysicalDeviceProperties(*devices, &deviceProperties);
		// 	VkPhysicalDeviceFeatures deviceFeatures;
		// 	vkGetPhysicalDeviceFeatures(*devices, &deviceFeatures);
		// 	if (deviceFeatures.geometryShader)
		// 	{
		// 		is_suitable = LLL_TRUE;
		// 	}
		*output_queue_family_indices = vk_find_queue_families(*devices, surface, temp_arena);
		if (output_queue_family_indices->is_valid_graphics_family == LLL_TRUE)
		{
			is_suitable = LLL_TRUE;
		}
		if (is_suitable)
		{
			is_suitable = LLL_FALSE;
			// Note: Check if device support extensions
			lll_u32  extension_count = 0;
			vkEnumerateDeviceExtensionProperties(*devices, NULL, &extension_count, NULL);
			lll_arena_snapshot snapshot = lll_arena_cheese(temp_arena);
			VkExtensionProperties* extension_properties = lll_arena_alloc(temp_arena, sizeof(VkExtensionProperties) * extension_count, 4);
			vkEnumerateDeviceExtensionProperties(*devices, NULL, &extension_count, extension_properties);
			(void) required_device_extensions;
			char** current = required_device_extensions;
			lll_string current_as_string = {*current, lll_strlen(*current)};
			for (lll_u32 j = 0; j < extension_count; j++)
			{
				lll_string name = {extension_properties[j].extensionName, lll_strlen(extension_properties[j].extensionName)};
				if (lll_string_is_equal(name, current_as_string))
				{
					current++;
					if (current >= required_device_extensions + (sizeof(required_device_extensions) / sizeof(char*)))
					{
						is_suitable = LLL_TRUE;
						break;
					}
					current_as_string = (lll_string) {*current, lll_strlen(*current)};
				}
			}
			lll_arena_rollback(temp_arena, snapshot);
		}
		if (is_suitable)
		{
			is_suitable = LLL_FALSE;
			// Note: Check if swap chain has at least one supported format and present mode
			struct vk_swapchain_support_details details = {0};
			lll_arena_snapshot snapshot = lll_arena_cheese(temp_arena);
			vk_query_swapchain_support(*devices, surface, &details, temp_arena);
			if ((details.formats_size > 0) && (details.present_modes_size > 0))
			{
				is_suitable = LLL_TRUE;
			}
			lll_arena_rollback(temp_arena, snapshot);
		}
		if (is_suitable)
		{
			*output_physical_device = *devices;
			break;
		}
		devices++;
	}
	if (*output_physical_device == VK_NULL_HANDLE)
	{
		LLL_PRINT_ERROR("Error: Failed to find a suitable GPU\n");
		return LLL_FALSE;
	}
	lll_arena_clear(temp_arena);
	return LLL_TRUE;
}

lll_b8 vk_create_logical_device(VkPhysicalDevice physical_device, struct vk_queue_family_indices queue_family_indices, VkDevice* output)
{
	lll_assert(physical_device != VK_NULL_HANDLE, "Physical device should not be NULL");
	lll_assert(queue_family_indices.is_valid_graphics_family == LLL_TRUE, "Graphics queue family is not valid");
	lll_assert(queue_family_indices.is_valid_present_family == LLL_TRUE, "Present queue family is not valid");
	lll_assert(*output == VK_NULL_HANDLE, "logical device should be zero-initialized");
	lll_f32 queue_priority = 1.0f;
	VkDeviceQueueCreateInfo	create_infos[2];
	lll_u32 create_infos_size = 2;
	if (queue_family_indices.present_family_index == queue_family_indices.graphics_family_index)
	{
		create_infos_size = 1;
	}
	for (lll_u32 i = 0; i < create_infos_size; i++)
	{
		VkDeviceQueueCreateInfo queue_create_info = {0};
		queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_info.queueFamilyIndex = queue_family_indices.family_indices[i];
		queue_create_info.queueCount = 1;
		queue_create_info.pQueuePriorities = &queue_priority;
		create_infos[i] = queue_create_info;
	}

	VkPhysicalDeviceFeatures device_features = {0};
	VkDeviceCreateInfo  create_info = {0};
	create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	create_info.pQueueCreateInfos = create_infos;
	create_info.queueCreateInfoCount = create_infos_size;
	create_info.pEnabledFeatures = &device_features;
	create_info.ppEnabledExtensionNames = (const char**) required_device_extensions;
	create_info.enabledExtensionCount = sizeof(required_device_extensions) / sizeof(char*);

	// Note: For backward compatibility only. Validation layer on device is no longer supported
	if (enable_validation_layers)
	{
		create_info.enabledLayerCount = sizeof(validation_layers) / sizeof(char*);
		create_info.ppEnabledLayerNames = (const char**) validation_layers;
	}
	else
	{
		create_info.enabledLayerCount = 0;
	}

	lll_assert(physical_device != NULL, "physical device is not valid");
	VkResult res = vkCreateDevice(physical_device, &create_info, NULL, output);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create logical device\n");
		return LLL_FALSE;
	}
	return LLL_TRUE;
}

struct vk_swapchain_images
{
	VkImage*     data;
	VkImageView* views;
	lll_u32      size;
	VkFormat     format;
	VkExtent2D   extent;
};

lll_b8  vk_create_swapchain(GLFWwindow* window, VkSurfaceKHR surface, VkPhysicalDevice physical_device, VkDevice logical_device, struct vk_queue_family_indices queue_family_indices, VkSwapchainKHR* output_swapchain, struct vk_swapchain_images* output_swapchain_images, lll_arena* arena)
{
	lll_assert(window != NULL, "Window should not be NULL");
	lll_assert(physical_device != VK_NULL_HANDLE, "Physical device should not be NULL");
	lll_assert(logical_device != VK_NULL_HANDLE, "Logical device should not be NULL");
	lll_assert(*output_swapchain == VK_NULL_HANDLE, "Swapchain should be zero-initialized");
	struct vk_swapchain_support_details swapchain_details = {0};
	lll_arena_snapshot snapshot = lll_arena_cheese(arena);
	vk_query_swapchain_support(physical_device, surface, &swapchain_details, arena);
	// Note: choose swapchain surface format
	VkSurfaceFormatKHR surface_format = swapchain_details.formats[0];
	{
		VkSurfaceFormatKHR* current = swapchain_details.formats;
		for (lll_u32 i = 0; i < swapchain_details.formats_size; i++)
		{
			if ((current->format == VK_FORMAT_B8G8R8A8_SRGB) && (current->colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR))
			{
				surface_format = *current;
			}
			current++;
		}
	}
	// Note: choose swapchain present mode
	VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
	{
		VkPresentModeKHR* current = swapchain_details.present_modes;
		for (lll_u32 i = 0; i < swapchain_details.present_modes_size; i++)
		{
			if (*current == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
			}
			current++;
		}
	}
	// Note: choose swapchain resolution
	VkExtent2D extent = swapchain_details.capabilities.currentExtent;
	if (swapchain_details.capabilities.currentExtent.width == LLL_INVALID_SIZE)
	{
		lll_i32 width, height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actual_extent = {(lll_u32) width, (lll_u32) height};
		actual_extent.width = lll_math_u32_clamp(actual_extent.width, swapchain_details.capabilities.minImageExtent.width, swapchain_details.capabilities.maxImageExtent.width);
		actual_extent.height = lll_math_u32_clamp(actual_extent.height, swapchain_details.capabilities.minImageExtent.height, swapchain_details.capabilities.maxImageExtent.height);
		extent = actual_extent;
	}

	VkSwapchainCreateInfoKHR create_info = {0};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;
	lll_u32 image_count = swapchain_details.capabilities.minImageCount + 1;
	if ((swapchain_details.capabilities.maxImageCount > 0) && (image_count > swapchain_details.capabilities.maxImageCount))
	{
		image_count = swapchain_details.capabilities.maxImageCount;
	}
	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	if (queue_family_indices.graphics_family_index != queue_family_indices.present_family_index)
	{
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queue_family_indices.family_indices;
	}
	else
	{
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0; // Optional
		create_info.pQueueFamilyIndices = NULL; // Optional
	}
	create_info.preTransform = swapchain_details.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;
	create_info.oldSwapchain = VK_NULL_HANDLE;
	VkResult res = vkCreateSwapchainKHR(logical_device, &create_info, NULL, output_swapchain);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create swap chain\n");
		return LLL_FALSE;
	}
	lll_arena_rollback(arena, snapshot);
	vkGetSwapchainImagesKHR(logical_device, *output_swapchain, &output_swapchain_images->size, NULL);
	output_swapchain_images->data = lll_arena_alloc(arena, sizeof(VkImage) * output_swapchain_images->size, 8);
	vkGetSwapchainImagesKHR(logical_device, *output_swapchain, &output_swapchain_images->size, output_swapchain_images->data);
	output_swapchain_images->format = surface_format.format;
	output_swapchain_images->extent = extent;
	return LLL_TRUE;
}

lll_b8  vk_create_image_view(VkDevice logical_device, struct vk_swapchain_images* output, lll_arena* arena)
{
	lll_assert(logical_device != VK_NULL_HANDLE, "Logical device should not be NULL");
	output->views = lll_arena_alloc(arena, sizeof(VkImageView) * output->size, 8);
	VkImageViewCreateInfo create_info_common = {0};
	create_info_common.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	create_info_common.viewType = VK_IMAGE_VIEW_TYPE_2D;
	create_info_common.format = output->format;
	create_info_common.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info_common.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info_common.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info_common.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	create_info_common.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	create_info_common.subresourceRange.baseMipLevel = 0;
	create_info_common.subresourceRange.levelCount = 1;
	create_info_common.subresourceRange.baseArrayLayer = 0;
	create_info_common.subresourceRange.layerCount = 1;
	for (lll_u32 i = 0; i < output->size; i++)
	{
		VkImageViewCreateInfo create_info = create_info_common;
		create_info.image = output->data[i];
		VkResult res = vkCreateImageView(logical_device, &create_info, NULL, output->views + i);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create image views\n");
			return LLL_FALSE;
		}
	}
	lll_printf("Info: Create %u swapchain image views\n", output->size);
	return LLL_TRUE;
}

lll_b8 vk_create_render_pass(VkDevice logical_device, VkFormat swapchain_images_format, VkRenderPass* output)
{
	lll_assert(logical_device != VK_NULL_HANDLE, "Logical device should not be NULL");
	lll_assert(*output == VK_NULL_HANDLE, "Render pass should be zero-initialized");
	VkAttachmentDescription color_attachment = {0};
	color_attachment.format = swapchain_images_format;
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref = {0};;
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {0};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;

	VkSubpassDependency dependency = {0};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo render_pass_info = {0};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.attachmentCount = 1;
	render_pass_info.pAttachments = &color_attachment;
	render_pass_info.subpassCount = 1;
	render_pass_info.pSubpasses = &subpass;
	render_pass_info.dependencyCount = 1;
	render_pass_info.pDependencies = &dependency;

	VkResult res = vkCreateRenderPass(logical_device, &render_pass_info, NULL, output);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create render pass\n");
		return LLL_FALSE;
	}
	return LLL_TRUE;
}

lll_string   read_entire_file(char* filename)
{
	char* file_data = 0;
	unsigned long	file_size = 0;
	int input_file_fd = open(filename, O_RDONLY);
	if (input_file_fd >= 0)
	{
		struct stat input_file_stat = {0};
		stat(filename, &input_file_stat);
		file_size = input_file_stat.st_size;
		file_data = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, input_file_fd, 0);
		close(input_file_fd);
	}
	return (lll_string){file_data, file_size};
}

lll_b8  vk_create_pipeline(VkDevice logical_device, VkRenderPass render_pass, VkPipelineLayout* output_pipeline_layout, VkPipeline* output_pipeline)
{
	lll_assert(logical_device != VK_NULL_HANDLE, "Logical device should not be NULL");
	lll_assert(render_pass != VK_NULL_HANDLE, "Render pass should not be NULL");
	lll_assert(*output_pipeline_layout == VK_NULL_HANDLE, "Pipeline layout should be zero-initialized");
	lll_assert(*output_pipeline == VK_NULL_HANDLE, "Pipeline should be zero-initialized");
	lll_string shader_vertex   = read_entire_file("./shaders/vert.spv");
	lll_string shader_fragment = read_entire_file("./shaders/frag.spv");
	VkShaderModule shader_module_vertex = VK_NULL_HANDLE;
	{
		VkShaderModuleCreateInfo create_info = {0};
		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = shader_vertex.length;
		create_info.pCode = (lll_u32*) shader_vertex.data;
		VkResult res = vkCreateShaderModule(logical_device, &create_info, NULL, &shader_module_vertex);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create shader module for vertex shader\n");
			return LLL_FALSE;
		}
	}
	VkShaderModule shader_module_fragment = VK_NULL_HANDLE;
	{
		VkShaderModuleCreateInfo createInfo = {0};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = shader_fragment.length;
		createInfo.pCode = (lll_u32*) shader_fragment.data;
		VkResult res = vkCreateShaderModule(logical_device, &createInfo, NULL, &shader_module_fragment);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create shader module for fragment shader\n");
			return LLL_FALSE;
		}
	}

	VkPipelineShaderStageCreateInfo	shader_stages[2] = {0};
	{
		VkPipelineShaderStageCreateInfo shader_stage_info_vertex = {0};
		shader_stage_info_vertex.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stage_info_vertex.stage = VK_SHADER_STAGE_VERTEX_BIT;
		shader_stage_info_vertex.module = shader_module_vertex;
		shader_stage_info_vertex.pName = "main";

		VkPipelineShaderStageCreateInfo shader_stage_info_fragment = {0};
		shader_stage_info_fragment.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stage_info_fragment.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shader_stage_info_fragment.module = shader_module_fragment;
		shader_stage_info_fragment.pName = "main";

		shader_stages[0] = shader_stage_info_vertex;
		shader_stages[1] = shader_stage_info_fragment;
	}

	VkPipelineLayoutCreateInfo pipeline_layout_info = {0};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 0;
	pipeline_layout_info.pushConstantRangeCount = 0;

	VkResult res = vkCreatePipelineLayout(logical_device, &pipeline_layout_info, NULL, output_pipeline_layout);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create graphics pipeline layout\n");
		vkDestroyShaderModule(logical_device, shader_module_vertex, NULL);
		vkDestroyShaderModule(logical_device, shader_module_fragment, NULL);
		return LLL_FALSE;
	}

	VkDynamicState  dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	VkPipelineDynamicStateCreateInfo dynamic_state = {0};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = sizeof(dynamic_states) / sizeof(VkDynamicState);
	dynamic_state.pDynamicStates = dynamic_states;

	// TODO: Actually descript the format of the data
	VkPipelineVertexInputStateCreateInfo vertex_input_info = {0};
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	VkVertexInputBindingDescription  binding_description = {0};
	binding_description.binding = 0;
	binding_description.stride = sizeof(struct glass_vertex);
	binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertex_input_info.vertexBindingDescriptionCount = 1;
	vertex_input_info.pVertexBindingDescriptions = &binding_description;
	VkVertexInputAttributeDescription  attribute_descriptions[2] = {0};
	attribute_descriptions[0].binding = 0;
	attribute_descriptions[0].location = 0;
	attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attribute_descriptions[0].offset = LLL_OFFSET_OF(vertices, position);
	attribute_descriptions[1].binding = 0;
	attribute_descriptions[1].location = 1;
	attribute_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribute_descriptions[1].offset = LLL_OFFSET_OF(vertices, color);
	vertex_input_info.vertexAttributeDescriptionCount = sizeof(attribute_descriptions) / sizeof(VkVertexInputAttributeDescription);
	vertex_input_info.pVertexAttributeDescriptions = attribute_descriptions;

	VkPipelineInputAssemblyStateCreateInfo input_assembly = {0};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewport_state = {0};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer = {0};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling = {0};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState color_blend_attachment = {0};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo color_blending = {0};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;

	VkGraphicsPipelineCreateInfo pipeline_info = {0};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = 2;
	pipeline_info.pStages = shader_stages;
	pipeline_info.pVertexInputState = &vertex_input_info;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pDepthStencilState = NULL; // Optional
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = &dynamic_state;
	pipeline_info.layout = *output_pipeline_layout;
	pipeline_info.renderPass = render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipeline_info.basePipelineIndex = -1; // Optional
	res = vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, &pipeline_info, NULL, output_pipeline);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create graphics pipeline\n");
		vkDestroyShaderModule(logical_device, shader_module_vertex, NULL);
		vkDestroyShaderModule(logical_device, shader_module_fragment, NULL);
		return LLL_FALSE;
	}
	vkDestroyShaderModule(logical_device, shader_module_vertex, NULL);
	vkDestroyShaderModule(logical_device, shader_module_fragment, NULL);
	munmap(shader_vertex.data, shader_vertex.length);
	munmap(shader_fragment.data, shader_fragment.length);
	return LLL_TRUE;
}

lll_b8 vk_create_framebuffer(VkDevice logical_device, struct vk_swapchain_images* swapchain_images, VkRenderPass render_pass, VkFramebuffer** output_swapchain_framebuffers, lll_u32* output_swapchain_framebuffer_size, lll_arena* arena)
{
	lll_assert(logical_device != VK_NULL_HANDLE, "Logical device should not be NULL");
	lll_assert(render_pass != VK_NULL_HANDLE, "Render pass should not be NULL");
	*output_swapchain_framebuffers = lll_arena_alloc(arena, sizeof(VkFramebuffer) * swapchain_images->size, 8);
	*output_swapchain_framebuffer_size = swapchain_images->size;
	for (lll_u32 i = 0; i < *output_swapchain_framebuffer_size; i++)
	{
		VkImageView attachments[] = {swapchain_images->views[i]};

		VkFramebufferCreateInfo framebufferInfo = {0};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = render_pass;;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapchain_images->extent.width;
		framebufferInfo.height = swapchain_images->extent.height;
		framebufferInfo.layers = 1;

		VkResult res = vkCreateFramebuffer(logical_device, &framebufferInfo, NULL, *output_swapchain_framebuffers + i);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create frame buffer\b");
			return LLL_FALSE;
		}
	}
	return LLL_TRUE;
}

lll_b8  vk_create_command_pool(VkDevice logical_device, struct vk_queue_family_indices queue_family_indices, VkCommandPool* output)
{
	lll_assert(logical_device != VK_NULL_HANDLE, "Logical device should not be NULL");
	VkCommandPoolCreateInfo pool_info = {0};
	pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	pool_info.queueFamilyIndex = queue_family_indices.graphics_family_index;

	VkResult res = vkCreateCommandPool(logical_device, &pool_info, NULL, output);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create command pool\n");
		return LLL_FALSE;
	}
	return LLL_TRUE;
}

lll_b8 vk_create_sync_objects(VkDevice logical_device, VkSemaphore* output_semaphores_image_avaliable, VkSemaphore* output_semaphores_render_finished, VkFence* output_fences_in_flight)
{
	VkSemaphoreCreateInfo  semaphore_create_info = {0};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo  fence_create_info = {0};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (lll_u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		VkResult res_1 = vkCreateSemaphore(logical_device, &semaphore_create_info, NULL, output_semaphores_image_avaliable + i);
		VkResult res_2 = vkCreateSemaphore(logical_device, &semaphore_create_info, NULL, output_semaphores_render_finished + i);
		VkResult res_3 = vkCreateFence(logical_device, &fence_create_info, NULL, output_fences_in_flight + i);
		if ((res_1 != VK_SUCCESS) || (res_2 != VK_SUCCESS) || (res_3 != VK_SUCCESS))
		{
			LLL_PRINT_ERROR("Error: Failed to create semaphores\n");
			return LLL_FALSE;
		}
	}
	return LLL_TRUE;
}

lll_b8 vk_create_command_buffers(VkDevice logical_device, VkCommandPool command_pool, VkCommandBuffer* output)
{
	lll_assert(logical_device != VK_NULL_HANDLE, "Logical device should not be NULL");
	lll_assert(command_pool != VK_NULL_HANDLE, "Command pool should not be NULL");
	VkCommandBufferAllocateInfo alloc_info = {0};
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool = command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

	VkResult res = vkAllocateCommandBuffers(logical_device, &alloc_info, output);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create command buffer\n");
		return LLL_FALSE;
	}
	return LLL_TRUE;
}

lll_u32  vk_find_memory_type(VkPhysicalDevice physical_device, lll_u32 type_filter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memory_properties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memory_properties);
	for (lll_u32 i = 0; i < memory_properties.memoryTypeCount; i++)
	{
		if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}
	lll_assert(FALSE, "Cannot find suitable memory type");
	return LLL_INVALID_INDEX;
}

lll_b8 vk_create_vertex_buffer(VkPhysicalDevice physical_device, VkDevice logical_device, VkBuffer* output_buffer_vertex, VkDeviceMemory* output_buffer_vertex_memory)
{
	VkBufferCreateInfo buffer_info = {0};
	buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.size = sizeof(vertices);
	buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult res = vkCreateBuffer(logical_device, &buffer_info, NULL, output_buffer_vertex);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to create vertex buffer\n");
		return LLL_FALSE;
	}
	VkMemoryRequirements memory_requirements = {0};
	vkGetBufferMemoryRequirements(logical_device, *output_buffer_vertex, &memory_requirements);
	VkMemoryAllocateInfo alloc_info = {0};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = memory_requirements.size;
	alloc_info.memoryTypeIndex = vk_find_memory_type(physical_device, memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	res = vkAllocateMemory(logical_device, &alloc_info, NULL, output_buffer_vertex_memory);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to allocate vertex buffer memory\n");
		return LLL_FALSE;
	}
	vkBindBufferMemory(logical_device, *output_buffer_vertex, *output_buffer_vertex_memory, 0);
	void* data;
	vkMapMemory(logical_device, *output_buffer_vertex_memory, 0, buffer_info.size, 0, &data);
	lll_memcpy(data, vertices, buffer_info.size);
	vkUnmapMemory(logical_device, *output_buffer_vertex_memory);
	return LLL_TRUE;
}

lll_b8  vk_record_command_buffer(VkCommandBuffer command_buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, VkExtent2D swapchain_extent, VkPipeline pipeline, VkBuffer vertex_buffer)
{
	VkCommandBufferBeginInfo begin_info = {0};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags = 0; // Optional
	begin_info.pInheritanceInfo = NULL; // Optional

	VkResult res = vkBeginCommandBuffer(command_buffer, &begin_info);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to begin recording command buffer\n");
		return LLL_FALSE;
	}
	VkRenderPassBeginInfo render_pass_info = {0};
	render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_info.renderPass = render_pass;
	render_pass_info.framebuffer = framebuffer;
	render_pass_info.renderArea.offset = (VkOffset2D) {0, 0};
	render_pass_info.renderArea.extent = swapchain_extent;

	VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
	render_pass_info.clearValueCount = 1;
	render_pass_info.pClearValues = &clear_color;
	vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
	{
		vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		VkViewport viewport = {0};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (lll_f32) swapchain_extent.width;
		viewport.height = (lll_f32) swapchain_extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(command_buffer, 0, 1, &viewport);

		VkRect2D scissor = {0};
		scissor.offset = (VkOffset2D) {0, 0};
		scissor.extent = swapchain_extent;
		vkCmdSetScissor(command_buffer, 0, 1, &scissor);
		VkBuffer vertex_buffers[] = {vertex_buffer};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
		vkCmdDraw(command_buffer, sizeof(vertices) / sizeof(struct glass_vertex), 1, 0, 0);
	}
	vkCmdEndRenderPass(command_buffer);
	res = vkEndCommandBuffer(command_buffer);
	if (res != VK_SUCCESS)
	{
		LLL_PRINT_ERROR("Error: Failed to record command buffer\n");
		return LLL_FALSE;
	}
	return LLL_TRUE;
}

struct vk_globals
{
	GLFWwindow*                window;
	VkInstance                 instance;
	VkDebugUtilsMessengerEXT   debug_messenger;
	VkSurfaceKHR               surface;
	VkDevice                   logical_device;
	VkSwapchainKHR             swapchain;
	struct vk_swapchain_images swapchain_images;
	VkRenderPass               render_pass;
	VkPipelineLayout           pipeline_layout;
	VkPipeline                 pipeline;
	VkFramebuffer*             swapchain_framebuffers;
	lll_u32                    swapchain_framebuffer_size;
	VkBuffer                   buffer_vertex;
	VkDeviceMemory             buffer_vertex_memory;
	VkCommandPool              command_pool;
	VkCommandBuffer            command_buffers[MAX_FRAMES_IN_FLIGHT];
	VkSemaphore                semaphores_image_avaliable[MAX_FRAMES_IN_FLIGHT];
	VkSemaphore                semaphores_render_finished[MAX_FRAMES_IN_FLIGHT];
	VkFence                    fences_in_flight[MAX_FRAMES_IN_FLIGHT];
};

lll_b8  vk_recreate_swapchain(struct vk_globals* globals, VkPhysicalDevice physical_device, struct vk_queue_family_indices queue_family_indices, lll_arena* arena)
{
	lll_i32 width, height;
	glfwGetFramebufferSize(globals->window, &width, &height);
	while ((width == 0) || (height == 0))
	{
		glfwGetFramebufferSize(globals->window, &width, &height);
		glfwWaitEvents();
	}
	vkDeviceWaitIdle(globals->logical_device);
	// Note: destroy framebuffer, image view and swapchain
	for (lll_u32 i = 0; i < globals->swapchain_framebuffer_size; i++)
	{
		vkDestroyFramebuffer(globals->logical_device, globals->swapchain_framebuffers[i], NULL);
	}
	for (lll_u32 i = 0; i < globals->swapchain_images.size; i++)
	{
		vkDestroyImageView(globals->logical_device, globals->swapchain_images.views[i], NULL);
	}
	vkDestroySwapchainKHR(globals->logical_device, globals->swapchain, NULL);
	lll_arena_clear(arena);
	globals->swapchain = (VkSwapchainKHR){0};
	globals->swapchain_images = (struct vk_swapchain_images){0};
	globals->swapchain_framebuffers = NULL;
	globals->swapchain_framebuffer_size = 0;

	if (!vk_create_swapchain(globals->window, globals->surface, physical_device, globals->logical_device, queue_family_indices, &globals->swapchain, &globals->swapchain_images, arena) ||
      !vk_create_image_view(globals->logical_device, &globals->swapchain_images, arena) ||
      !vk_create_framebuffer(globals->logical_device, &globals->swapchain_images, globals->render_pass, &globals->swapchain_framebuffers, &globals->swapchain_framebuffer_size, arena))
	{
		return LLL_FALSE;
	}
	return LLL_TRUE;
}

void vk_cleanup(struct vk_globals* globals)
{
	for (lll_u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (globals->semaphores_image_avaliable[i] != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(globals->logical_device, globals->semaphores_image_avaliable[i], NULL);
		}
	}
	for (lll_u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (globals->semaphores_render_finished[i] != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(globals->logical_device, globals->semaphores_render_finished[i], NULL);
		}
	}
	for (lll_u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (globals->fences_in_flight[i] != VK_NULL_HANDLE)
		{
			vkDestroyFence(globals->logical_device, globals->fences_in_flight[i], NULL);
		}
	}
	if (globals->buffer_vertex != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(globals->logical_device, globals->buffer_vertex, NULL);
	}
	if (globals->buffer_vertex_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(globals->logical_device, globals->buffer_vertex_memory, NULL);
	}
	if (globals->command_pool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(globals->logical_device, globals->command_pool, NULL);
	}
	for (lll_u32 i = 0; i < globals->swapchain_framebuffer_size; i++)
	{
		vkDestroyFramebuffer(globals->logical_device, globals->swapchain_framebuffers[i], NULL);
	}
	if (globals->pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(globals->logical_device, globals->pipeline, NULL);
	}
	if (globals->pipeline_layout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(globals->logical_device, globals->pipeline_layout, NULL);
	}
	if (globals->render_pass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(globals->logical_device, globals->render_pass, NULL);
	}
	for (lll_u32 i = 0; i < globals->swapchain_images.size; i++)
	{
		vkDestroyImageView(globals->logical_device, globals->swapchain_images.views[i], NULL);
	}
	if (globals->swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(globals->logical_device, globals->swapchain, NULL);
	}
	if (globals->logical_device != VK_NULL_HANDLE)
	{
		vkDestroyDevice(globals->logical_device, NULL);
	}
	if (globals->surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(globals->instance, globals->surface, NULL);
	}
	if (enable_validation_layers && (globals->debug_messenger != VK_NULL_HANDLE))
	{
		PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(globals->instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != NULL)
		{
			func(globals->instance, globals->debug_messenger, NULL);
		}
	}
	if (globals->instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(globals->instance, NULL);
	}
	glfwDestroyWindow(globals->window);
	glfwTerminate();
}

int main(void)
{
	glass_init(2 LLL_GB, 32, 1024, 512);
	static lll_arena	temp_arena;
	static lll_arena	swapchain_arena;
	lll_arena_init(&temp_arena, LLL_PAGE_SIZE * 16);
	lll_arena_init(&swapchain_arena, LLL_PAGE_SIZE * 16);
	struct vk_globals globals = {0};
	lll_b8            is_framebuffer_resized = LLL_FALSE;
	// Note: Create window
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		globals.window = glfwCreateWindow(800, 600, "Vulkan window", NULL, NULL);
		glfwSetWindowUserPointer(globals.window, &is_framebuffer_resized);
		glfwSetFramebufferSizeCallback(globals.window, &vk_framebuffer_resize_callback);
	}
	if (!vk_create_instance(&globals.instance, &temp_arena))
	{
		vk_cleanup(&globals);
		return 1;
	}
	if (enable_validation_layers)
	{
		vk_create_debug_messenger(globals.instance, &globals.debug_messenger);
	}
	// Note: Create a surface
	{
		VkResult res = glfwCreateWindowSurface(globals.instance, globals.window, NULL, &globals.surface);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create window surface\n");
			vk_cleanup(&globals);
			return 1;
		}
	}
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	struct vk_queue_family_indices	queue_family_indices = {0};
	if (!vk_find_suitable_physical_device(globals.instance, globals.surface, &physical_device, &queue_family_indices, &temp_arena))
	{
		vk_cleanup(&globals);
		return 1;
	}
	if (!vk_create_logical_device(physical_device, queue_family_indices, &globals.logical_device))
	{
		vk_cleanup(&globals);
		return 1;
	}
	VkQueue graphics_queue;
	vkGetDeviceQueue(globals.logical_device, queue_family_indices.graphics_family_index, 0, &graphics_queue);
	VkQueue present_queue;
	vkGetDeviceQueue(globals.logical_device, queue_family_indices.present_family_index, 0, &present_queue);
	if (!vk_create_swapchain(globals.window, globals.surface, physical_device, globals.logical_device, queue_family_indices, &globals.swapchain, &globals.swapchain_images, &swapchain_arena) ||
      !vk_create_image_view(globals.logical_device, &globals.swapchain_images, &swapchain_arena) ||
      !vk_create_render_pass(globals.logical_device, globals.swapchain_images.format, &globals.render_pass) ||
      !vk_create_pipeline(globals.logical_device, globals.render_pass, &globals.pipeline_layout, &globals.pipeline) ||
      !vk_create_framebuffer(globals.logical_device, &globals.swapchain_images, globals.render_pass, &globals.swapchain_framebuffers, &globals.swapchain_framebuffer_size, &swapchain_arena) ||
      !vk_create_command_pool(globals.logical_device, queue_family_indices, &globals.command_pool) ||
      !vk_create_vertex_buffer(physical_device, globals.logical_device, &globals.buffer_vertex, &globals.buffer_vertex_memory) ||
      !vk_create_command_buffers(globals.logical_device, globals.command_pool, globals.command_buffers) ||
      !vk_create_sync_objects(globals.logical_device, globals.semaphores_image_avaliable, globals.semaphores_render_finished, globals.fences_in_flight))
	{
		vk_cleanup(&globals);
		return 1;
	}

	lll_u32 current_frame_index = 0;
	while(!glfwWindowShouldClose(globals.window))
	{
		glfwPollEvents();
		vkWaitForFences(globals.logical_device, 1, globals.fences_in_flight + current_frame_index, VK_TRUE, (lll_u64) -1);
		lll_u32 image_index;
		VkResult res = vkAcquireNextImageKHR(globals.logical_device, globals.swapchain, (lll_u64) -1, globals.semaphores_image_avaliable[current_frame_index], NULL, &image_index);
		if (res == VK_ERROR_OUT_OF_DATE_KHR)
		{
			vk_recreate_swapchain(&globals, physical_device, queue_family_indices, &swapchain_arena);
			continue;
		}
		else if ((res != VK_SUCCESS) && (res != VK_SUBOPTIMAL_KHR))
		{
			LLL_PRINT_ERROR("Error: Failed to acquire swap chain image\n");
			break;
		}
		vkResetFences(globals.logical_device, 1, globals.fences_in_flight + current_frame_index);
		vkResetCommandBuffer(globals.command_buffers[current_frame_index], 0);
		if (!vk_record_command_buffer(globals.command_buffers[current_frame_index], globals.render_pass, globals.swapchain_framebuffers[image_index], globals.swapchain_images.extent, globals.pipeline, globals.buffer_vertex))
		{
			break;
		}
		VkSubmitInfo submit_info = {0};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore wait_semaphores[] = {globals.semaphores_image_avaliable[current_frame_index]};
		VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = wait_semaphores;
		submit_info.pWaitDstStageMask = wait_stages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = globals.command_buffers + current_frame_index;
		VkSemaphore signal_semaphores[] = {globals.semaphores_render_finished[current_frame_index]};
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = signal_semaphores;
		res = vkQueueSubmit(graphics_queue, 1, &submit_info, globals.fences_in_flight[current_frame_index]);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to submit draw command buffer\n");
			break;
		}
		VkPresentInfoKHR present_info = {0};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = signal_semaphores;
		VkSwapchainKHR swap_chains[] = {globals.swapchain};
		present_info.swapchainCount = 1;
		present_info.pSwapchains = swap_chains;
		present_info.pImageIndices = &image_index;
		present_info.pResults = NULL; // Optional
		res = vkQueuePresentKHR(present_queue, &present_info);
		if ((res == VK_ERROR_OUT_OF_DATE_KHR) || (res == VK_SUBOPTIMAL_KHR) || is_framebuffer_resized)
		{
			is_framebuffer_resized = LLL_FALSE;
			vk_recreate_swapchain(&globals, physical_device, queue_family_indices, &swapchain_arena);
		}
		else if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to present swap chain image\n");
			break;
		}
		static lll_b8 is_increase = LLL_TRUE;
		void* data;
		if ((vertices[0].color.g > 1.0f) || (vertices[0].color.g < 0))
		{
			is_increase = !is_increase;
		}
		if (is_increase)
		{
			vertices[0].color.g += 0.01f;
		}
		else
		{
			vertices[0].color.g -= 0.01f;
		}
		vkMapMemory(globals.logical_device, globals.buffer_vertex_memory, 0, sizeof(vertices), 0, &data);
		lll_memcpy(data, vertices, sizeof(vertices));
		vkUnmapMemory(globals.logical_device, globals.buffer_vertex_memory);
		current_frame_index = (current_frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
	}
	vkDeviceWaitIdle(globals.logical_device);
	vk_cleanup(&globals);
	return 0;
}
