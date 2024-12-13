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

int main(void)
{
	return 1;
}
