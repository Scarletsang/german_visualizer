#include "vulkan/vulkan_core.h"
#define LALALA_IMPLEMENTATION
#include "lalala.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include <sys/stat.h>

enum glass_block_size_measure_method
{
	SIZE_FIT_CHILDREN, // Note: No Paramter
	SIZE_PX,	// Note: <f32> [reference target]
	SIZE_PERCENT, // Note: <f32> <reference target>
	SIZE_ASPECT_RATIO, // Note: <f32>
	SIZE_WEIGHED, // Note: <f32>
};

enum glass_block_size_refernce_target
{
	SIZE_TARGET_NONE,
	SIZE_TARGET_FONT_HEIGHT,
	SIZE_TARGET_PARENT,
};

struct glass_block_size
{
	enum glass_block_size_measure_method  method;
	enum glass_block_size_refernce_target reference_target;
	lll_f32                               number;
};

enum glass_block_position_measure_method
{
	POSITION_RELATIVE_TO_PARENT, // Note: <px/percent> <f32>
	POSITION_RELATIVE_TO_SCREEN, // Note: <px/percent> <f32>
	POSITION_LAYOUT, // Note: <layout_type>
};

enum glass_block_position_layout
{
	POSITION_LAYOUT_START,
	POSITION_LAYOUT_END,
	POSITION_LAYOUT_CENTER,
};

enum glass_block_position_unit
{
	POSITION_UNIT_PX,
	POSITION_UNIT_PERCENT,
};

struct glass_block_position
{
	enum glass_block_position_measure_method method;
	union
	{
		enum glass_block_position_unit         unit;
		enum glass_block_position_layout       layout_type;
	};
	lll_f32                                  number;
};

struct glass_block_border
{
	union
	{
		lll_u32	color[4];
		struct
		{
			lll_u32 color_top;
			lll_u32 color_bottom;
			lll_u32 color_left;
			lll_u32 color_right;
		};
	};
	lll_u32 size;
};

enum glass_block_type
{
	GLASS_BLOCK_SINGLE,
	GLASS_BLOCK_COLUMN,
	GLASS_BLOCK_ROW,
	GLASS_BLOCK_GRID
};

#define GLASS_BLOCK_RENDER_BACKGROUND (0b1);
#define GLASS_BLOCK_RENDER_FONT (0b10);
#define GLASS_BLOCK_RENDER_BORDER (0b100);

struct glass_block
{
	struct glass_block* parent;
	struct glass_block* child_first;
	struct glass_block* child_last;

	struct glass_block* row_next;
	struct glass_block* row_previous;
	struct glass_block* column_next;
	struct glass_block* column_previous;

	struct glass_block_size     width;
	struct glass_block_size     height;
	struct glass_block_position x;
	struct glass_block_position y;
	enum glass_block_type	      type;
	lll_u32                     hash;
	lll_u32                     flags;
	lll_string                  text;
	lll_u32                     color_background;
	struct glass_block_border   border;
};

#ifdef NDEBUG
    const lll_b8 enable_validation_layers = LLL_FALSE;
#else
    const lll_b8 enable_validation_layers = LLL_TRUE;
#endif
static char*  validation_layers[] = {"VK_LAYER_KHRONOS_validation"};
static char*  required_device_extensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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

lll_b8	vk_check_validation_layer_support(lll_arena* arena)
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

struct vk_queue_family_indices
{
	lll_b8   is_valid_graphics_family : 1;
	lll_b8   is_valid_present_family : 1;
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
	lll_assert(device != VK_NULL_HANDLE, "Physical device is not valid");
	lll_assert(surface != VK_NULL_HANDLE, "Surface is not valid");
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

struct vk_globals
{
	GLFWwindow*              window;
	VkInstance               instance;
	VkDebugUtilsMessengerEXT debug_messenger;
	VkSurfaceKHR             surface;
	VkDevice                 logical_device;
	VkSwapchainKHR           swapchain;
	VkImageView*             swapchain_image_views;
	lll_u32                  swapchain_image_view_size;
};

void vk_cleanup(struct vk_globals* globals)
{
	for (lll_u32 i = 0; i < globals->swapchain_image_view_size; i++)
	{
		vkDestroyImageView(globals->logical_device, globals->swapchain_image_views[i], NULL);
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
	static lll_arena	temp_arena;
	static lll_arena	permenant_arena;
	lll_arena_init(&temp_arena, LLL_PAGE_SIZE * 16);
	lll_arena_init(&permenant_arena, LLL_PAGE_SIZE * 16);
	struct vk_globals globals = {0};
	// Note: Create window
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	globals.window = glfwCreateWindow(800, 600, "Vulkan window", NULL, NULL);

	// Note: Init Vulkan instance
	globals.instance = VK_NULL_HANDLE;
	{
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
		char** required_extensions = lll_arena_alloc(&temp_arena, (glfw_extension_count + 2) * sizeof(char*), 1);
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
			if (!vk_check_validation_layer_support(&temp_arena))
			{
				LLL_PRINT_ERROR("Error: Validation layer requested but not avaliable\n");
				vk_cleanup(&globals);
				return 1;
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

		VkResult res = vkCreateInstance(&create_info, NULL, &globals.instance);
		if (res == VK_ERROR_INCOMPATIBLE_DRIVER)
		{
			LLL_PRINT_ERROR("Error: Failed to create Vulkan Instance due to no found compatible Vulkan ICD\n");
			vk_cleanup(&globals);
			return 1;
		}
		else if (res)
		{
			LLL_PRINT_ERROR("Error: Failed to create Vulkan Instance due to unknown reasons\n");
			vk_cleanup(&globals);
			return 1;
		}
		lll_arena_clear(&temp_arena);
	}
	// Note: setup debug messenger
	globals.debug_messenger = VK_NULL_HANDLE;
	{
		VkDebugUtilsMessengerCreateInfoEXT  debug_create_info = {0};
		debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debug_create_info.pfnUserCallback = vk_debug_callback;
		debug_create_info.pUserData = NULL;
		PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(globals.instance, "vkCreateDebugUtilsMessengerEXT");
		if ((func == NULL) || (func(globals.instance, &debug_create_info, NULL, &globals.debug_messenger) != VK_SUCCESS))
		{
			LLL_PRINT_ERROR("Warning: Failed to set up messege debugger\n");
		}
	}
	// Note: Create a surface
	globals.surface = VK_NULL_HANDLE;
	{
		VkResult res = glfwCreateWindowSurface(globals.instance, globals.window, NULL, &globals.surface);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create window surface\n");
			vk_cleanup(&globals);
			return 1;
		}
	}

	// Note: Find suitable GPU
	VkPhysicalDevice physical_device = VK_NULL_HANDLE;
	struct vk_queue_family_indices	queue_family_indices = {0};
	{
		lll_u32 device_count = 0;
		vkEnumeratePhysicalDevices(globals.instance, &device_count, NULL);
		if (device_count == 0)
		{
			LLL_PRINT_ERROR("Error: Failed to find GPU with Vulkan support\n");
			vk_cleanup(&globals);
			return 1;
		}
		lll_printf("%u physical device avaliable\n", device_count);
		VkPhysicalDevice* devices = lll_arena_alloc(&temp_arena, sizeof(VkPhysicalDevice) * device_count, 8);
		vkEnumeratePhysicalDevices(globals.instance, &device_count, devices);
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
			queue_family_indices = vk_find_queue_families(*devices, globals.surface, &temp_arena);
			if (queue_family_indices.is_valid_graphics_family == LLL_TRUE)
			{
				is_suitable = LLL_TRUE;
			}
			if (is_suitable)
			{
				is_suitable = LLL_FALSE;
				// Note: Check if device support extensions
				lll_u32  extension_count = 0;
				vkEnumerateDeviceExtensionProperties(*devices, NULL, &extension_count, NULL);
				lll_arena_snapshot snapshot = lll_arena_cheese(&temp_arena);
				VkExtensionProperties* extension_properties = lll_arena_alloc(&temp_arena, sizeof(VkExtensionProperties) * extension_count, 4);
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
				lll_arena_rollback(&temp_arena, snapshot);
			}
			if (is_suitable)
			{
				is_suitable = LLL_FALSE;
				// Note: Check if swap chain has at least one supported format and present mode
				struct vk_swapchain_support_details details = {0};
				lll_arena_snapshot snapshot = lll_arena_cheese(&temp_arena);
				vk_query_swapchain_support(*devices, globals.surface, &details, &temp_arena);
				if ((details.formats_size > 0) && (details.present_modes_size > 0))
				{
					is_suitable = LLL_TRUE;
				}
				lll_arena_rollback(&temp_arena, snapshot);
			}
			if (is_suitable)
			{
				physical_device = *devices;
				break;
			}
			devices++;
		}
		if (physical_device == VK_NULL_HANDLE)
		{
			LLL_PRINT_ERROR("Error: Failed to find a suitable GPU\n");
			vk_cleanup(&globals);
			return 1;
		}
		lll_arena_clear(&temp_arena);
	}
	// Note: Set up logical device
	globals.logical_device = VK_NULL_HANDLE;
	{
		lll_assert(queue_family_indices.is_valid_graphics_family == LLL_TRUE, "graphics queue family index is not valid");
		lll_f32 queue_priority = 1.0f;
		VkDeviceQueueCreateInfo	create_infos[2];
		lll_u32 create_infos_size = 2;
		lll_assert(queue_family_indices.is_valid_graphics_family == LLL_TRUE, "Graphics queue family is not valid");
		lll_assert(queue_family_indices.is_valid_present_family == LLL_TRUE, "Present queue family is not valid");
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
		VkResult res = vkCreateDevice(physical_device, &create_info, NULL, &globals.logical_device);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create logical device\n");
			vk_cleanup(&globals);
			return 1;
		}
	}
	VkQueue graphics_queue;
	{
		vkGetDeviceQueue(globals.logical_device, queue_family_indices.graphics_family_index, 0, &graphics_queue);
	}
	VkQueue present_queue;
	{
		vkGetDeviceQueue(globals.logical_device, queue_family_indices.present_family_index, 0, &present_queue);
	}
	// Note: create swapchains
	globals.swapchain = VK_NULL_HANDLE;
	VkImage*   swapchain_images = NULL;
	lll_u32    swapchain_images_size = 0;
	VkFormat   swapchain_image_format = {0};
	VkExtent2D swapchain_extent = {0};
	{
		struct vk_swapchain_support_details swapchain_details = {0};
		vk_query_swapchain_support(physical_device, globals.surface, &swapchain_details, &temp_arena);
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
			glfwGetFramebufferSize(globals.window, &width, &height);
			VkExtent2D actual_extent = {(lll_u32) width, (lll_u32) height};
			actual_extent.width = lll_math_u32_clamp(actual_extent.width, swapchain_details.capabilities.minImageExtent.width, swapchain_details.capabilities.maxImageExtent.width);
			actual_extent.height = lll_math_u32_clamp(actual_extent.height, swapchain_details.capabilities.minImageExtent.height, swapchain_details.capabilities.maxImageExtent.height);
			extent = actual_extent;
		}

		VkSwapchainCreateInfoKHR create_info = {0};
		create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.surface = globals.surface;
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
		VkResult res = vkCreateSwapchainKHR(globals.logical_device, &create_info, NULL, &globals.swapchain);
		if (res != VK_SUCCESS)
		{
			LLL_PRINT_ERROR("Error: Failed to create swap chain\n");
			vk_cleanup(&globals);
			return 1;
		}
		lll_arena_clear(&temp_arena);
		vkGetSwapchainImagesKHR(globals.logical_device, globals.swapchain, &swapchain_images_size, NULL);
		swapchain_images = lll_arena_alloc(&permenant_arena, sizeof(VkImage) * swapchain_images_size, 8);
		vkGetSwapchainImagesKHR(globals.logical_device, globals.swapchain, &swapchain_images_size, swapchain_images);
		swapchain_image_format = surface_format.format;
		swapchain_extent = extent;
	}
	(void) swapchain_image_format;
	(void) swapchain_extent;
	// Note: create image view
	{
		globals.swapchain_image_views = lll_arena_alloc(&permenant_arena, sizeof(VkImageView) * swapchain_images_size, 8);
		globals.swapchain_image_view_size = swapchain_images_size;
		VkImageViewCreateInfo create_info_common = {0};
		create_info_common.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		create_info_common.viewType = VK_IMAGE_VIEW_TYPE_2D;
		create_info_common.format = swapchain_image_format;
		create_info_common.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info_common.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info_common.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info_common.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		create_info_common.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		create_info_common.subresourceRange.baseMipLevel = 0;
		create_info_common.subresourceRange.levelCount = 1;
		create_info_common.subresourceRange.baseArrayLayer = 0;
		create_info_common.subresourceRange.layerCount = 1;
		for (lll_u32 i = 0; i < globals.swapchain_image_view_size; i++)
		{
			VkImageViewCreateInfo create_info = create_info_common;
			create_info.image = swapchain_images[i];
			VkResult res = vkCreateImageView(globals.logical_device, &create_info, NULL, globals.swapchain_image_views + i);
			if (res != VK_SUCCESS)
			{
				LLL_PRINT_ERROR("Error: Failed to create image views\n");
				vk_cleanup(&globals);
				return 1;
			}
		}
		lll_printf("Info: Create %u swapchain image views\n", globals.swapchain_image_view_size);
	}
	// Note: Load shader binary
	{
		lll_string shader_vertex   = read_entire_file("./shaders/vert.spv");
		lll_string shader_fragment = read_entire_file("./shaders/frag.spv");
		VkShaderModule shader_module_vertex = VK_NULL_HANDLE;
		{
			VkShaderModuleCreateInfo create_info = {0};
			create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			create_info.codeSize = shader_vertex.length;
			create_info.pCode = (lll_u32*) shader_vertex.data;
			VkResult res = vkCreateShaderModule(globals.logical_device, &create_info, NULL, &shader_module_vertex);
			if (res != VK_SUCCESS)
			{
				LLL_PRINT_ERROR("Error: Failed to create shader module for vertex shader\n");
				vk_cleanup(&globals);
				return 1;
			}
		}
		VkShaderModule shader_module_fragment = VK_NULL_HANDLE;
		{
			VkShaderModuleCreateInfo createInfo = {0};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = shader_fragment.length;
			createInfo.pCode = (lll_u32*) shader_fragment.data;
			VkResult res = vkCreateShaderModule(globals.logical_device, &createInfo, NULL, &shader_module_fragment);
			if (res != VK_SUCCESS)
			{
				LLL_PRINT_ERROR("Error: Failed to create shader module for fragment shader\n");
				vk_cleanup(&globals);
				return 1;
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

		vkDestroyShaderModule(globals.logical_device, shader_module_vertex, NULL);
		vkDestroyShaderModule(globals.logical_device, shader_module_fragment, NULL);
		munmap(shader_vertex.data, shader_vertex.length);
		munmap(shader_fragment.data, shader_fragment.length);
	}

	while(!glfwWindowShouldClose(globals.window))
	{
		glfwPollEvents();
	}
	vk_cleanup(&globals);
	return 0;
}
