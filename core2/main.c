#define LALALA_IMPLEMENTATION
#include "lalala.h"
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

lll_b8	check_vk_validation_layer_support(lll_arena* arena)
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

int main(void)
{
	lll_arena	temp_arena;
	lll_arena_init(&temp_arena, LLL_PAGE_SIZE);
	// Note: Create window
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", NULL, NULL);

	// Note: Init Vulkan instance
	VkInstance instance;
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
		char** required_extensions = lll_arena_alloc(&temp_arena, (glfw_extension_count + 1) * sizeof(char*), 1);
		for(lll_u32 i = 0; i < glfw_extension_count; i++)
		{
			required_extensions[i] = ((char**) glfw_extensions)[i];
		}
		required_extensions[glfw_extension_count] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
		create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		create_info.enabledExtensionCount = glfw_extension_count + 1;
		create_info.ppEnabledExtensionNames = (const char**) required_extensions;

		// Note: validation layer setup
		if (enable_validation_layers)
		{
			if (!check_vk_validation_layer_support(&temp_arena))
			{
				LLL_PRINT_ERROR("Error: Validation layer requested but not avaliable\n");
				glfwDestroyWindow(window);
				glfwTerminate();
				return 1;
			}
			else
			{
				create_info.enabledLayerCount = sizeof(validation_layers) / sizeof(char*);
				create_info.ppEnabledLayerNames = (const char**) validation_layers;
			}
		}
		else
		{
			create_info.enabledLayerCount = 0;
		}

		VkResult res = vkCreateInstance(&create_info, NULL, &instance);
		if (res == VK_ERROR_INCOMPATIBLE_DRIVER)
		{
			LLL_PRINT_ERROR("Error: Failed to create Vulkan Instance due to no found compatible Vulkan ICD\n");
			glfwDestroyWindow(window);
			glfwTerminate();
			return 1;
		}
		else if (res)
		{
			LLL_PRINT_ERROR("Error: Failed to create Vulkan Instance due to unknown reasons\n");
			glfwDestroyWindow(window);
			glfwTerminate();
			return 1;
		}
		lll_arena_clear(&temp_arena);
	}

	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	vkDestroyInstance(instance, NULL);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
