#ifndef GLASS
#define GLASS

#include "lalala.h"

typedef lll_u32 glass_index;

enum glass_block_size_measure_method
{
	SIZE_FIT_CHILDREN, // Note: No Paramter
	SIZE_PX,	// Note: <f32> [reference target] [f32], works for grid
	SIZE_PERCENT, // Note: <f32> <reference target> [f32], works for grid
	SIZE_ASPECT_RATIO, // Note: <f32>
	SIZE_WEIGHED, // Note: <f32>, works for grid
};

enum glass_block_size_refernce_target
{
	SIZE_TARGET_NONE,
	SIZE_TARGET_FONT_HEIGHT,
	SIZE_TARGET_PARENT,
	SIZE_TARGET_GRID
};

struct glass_block_size
{
	enum glass_block_size_measure_method  method;
	enum glass_block_size_refernce_target reference_target;
	lll_f32                               number;
	lll_f32                               number_grid;
};

enum glass_block_position_measure_method
{
	POSITION_RELATIVE_TO_PARENT, // Note: <px/percent> <f32>
	POSITION_RELATIVE_TO_SCREEN, // Note: <px/percent> <f32>
	POSITION_USE_PARENT_LAYOUT, // Note: <px/percent> <f32> [f32] Can still offset after the parent's layout
};

enum glass_block_position_unit
{
	POSITION_UNIT_PX,
	POSITION_UNIT_PERCENT,
};

struct glass_block_position
{
	enum glass_block_position_measure_method method;
	enum glass_block_position_unit           unit;
	lll_f32                                  number;
	lll_f32                                  number_grid;
};

enum glass_block_layout_type
{
	LAYOUT_NONE,
	LAYOUT_FLEX_ROW,
	LAYOUT_FLEX_COLUMN,
	LAYOUT_GRID
};

enum glass_block_layout_flex_type
{
	FLEX_START,
	FLEX_CENTER,
	FLEX_END
};

struct glass_block_layout_flex
{
	lll_b8                            x_is_reversed : 1;
	lll_b8                            y_is_reversed : 1;
	enum glass_block_layout_flex_type x_type : 3;
	enum glass_block_layout_flex_type y_type : 3;
};

struct glass_block_layout_grid
{
	lll_uv2     dimension;
	glass_index positions;
};

struct glass_block_layout
{
	enum glass_block_layout_type     type : 2;
	union
	{
		struct glass_block_layout_flex flex;
		struct glass_block_layout_grid grid;
	};
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

#define GLASS_BLOCK_RENDER_BACKGROUND (0b1);
#define GLASS_BLOCK_RENDER_FONT (0b10);
#define GLASS_BLOCK_RENDER_BORDER (0b100);

struct glass_block
{
	glass_index parent;
	glass_index child_first;
	glass_index child_last;
	glass_index sibling_previous;
	glass_index sibling_next;

	struct glass_block_size     width;
	struct glass_block_size     height;
	struct glass_block_position x;
	struct glass_block_position y;
	lll_u32                     hash;
	lll_u32                     flags;
	lll_string                  text;
	lll_u32                     color_background;
	struct glass_block_border   border;

	lll_u32                     calculated_depth;
	lll_uv2                     calculated_dimension;
	lll_uv2                     calculated_position;
};

struct glass_tree
{
	lll_arena            arena;
	struct glass_block** parent_stack;
	lll_u32              parent_stack_used;
	lll_u32              parent_stack_capacity;
	struct glass_block*  root;
	struct glass_block*  blocks;
	lll_u32              blocks_used;
	lll_u32              blocks_capacity;
	struct lll_ht        blocks_hashed;
	struct glass_block_position* grid_positions;
	lll_u32                      grid_positions_used;
	lll_u32                      grid_positions_capacity;
};

struct glass
{
	struct glass_tree   trees[2];
	lll_u32             current_tree_index;
};

extern struct glass glass_root;

lll_b8              glass_init(lll_u32 tree_size_in_byte, lll_u32 parent_stack_size, lll_u32 blocks_size, lll_u32 grid_positions_size);
lll_b8              glass_tree_init(struct glass_tree* tree, lll_u32 total_size_in_byte, lll_u32 parent_stack_size, lll_u32 blocks_size, lll_u32 grid_positions_size);
void                glass_tree_memory_print(struct glass_tree* tree);
struct glass_tree*  glass_tree_get_current();
struct glass_tree*  glass_tree_get_previous();
void                glass_tree_calculate_blocks_dimension(struct glass_tree* tree);
void                glass_tree_calculate_blocks_position(struct glass_tree* tree);

struct glass_block* glass_block_get(glass_index index, struct glass_tree* tree);
struct glass_block* glass_block_get_hashed(lll_u32 hash, struct glass_tree* tree);
glass_index         glass_block_get_index(struct glass_block* block, struct glass_tree* tree);

struct glass_block* glass_block_create(struct glass_tree* tree);
void                glass_block_push_parent(struct glass_block* block, struct glass_tree* tree);
void                glass_block_pop_parent(struct glass_tree* tree);

struct glass_block_position*  glass_block_layout_grid_get_positions(glass_index index, struct glass_tree* tree);
struct glass_block_position*  glass_block_layout_grid_create_positions(struct glass_block_layout_grid* grid, lll_uv2 dimension, struct glass_tree* tree);

#endif
#ifdef GLASS_IMPLEMENTATION
#ifndef _GLASS_IMPLEMENTATION
#define _GLASS_IMPLEMENTATION

#ifndef NULL
#define NULL (0)
#endif
struct glass glass_root = {0};

lll_b8  glass_init(lll_u32 tree_size_in_byte, lll_u32 parent_stack_size, lll_u32 blocks_size, lll_u32 grid_positions_size)
{
	if (!glass_tree_init(glass_root.trees, tree_size_in_byte, parent_stack_size, blocks_size, grid_positions_size))
	{
		return LLL_FALSE;
	}
	if (!glass_tree_init(glass_root.trees + 1, tree_size_in_byte, parent_stack_size, blocks_size, grid_positions_size))
	{
		lll_arena_destroy(&glass_root.trees[0].arena);
		return LLL_FALSE;
	}
	glass_root.current_tree_index = 0;
	return LLL_TRUE;
}

lll_b8  glass_tree_init(struct glass_tree* tree, lll_u32 total_size_in_byte, lll_u32 parent_stack_size, lll_u32 blocks_size, lll_u32 grid_positions_size)
{
	lll_u32 parent_stack_size_in_byte = sizeof(struct glass_block*) * parent_stack_size;
	lll_u32 blocks_size_in_byte = sizeof(struct glass_block) * blocks_size;
	lll_u32 blocks_ht_size_in_byte = sizeof(union lll_ht_entry) * (blocks_size + (blocks_size / 2));
	lll_u32 grid_positions_size_in_byte = sizeof(struct glass_block_position) * grid_positions_size;
	lll_u32 actual_total_size_in_byte = parent_stack_size_in_byte + blocks_size_in_byte + blocks_ht_size_in_byte + grid_positions_size_in_byte;
	if (total_size_in_byte < actual_total_size_in_byte)
	{
		lll_fdprintf(STDERR_FILENO, "Error: Requested %u bytes, but requires %u bytes\n", total_size_in_byte, actual_total_size_in_byte);
		return LLL_FALSE;
	}
	*tree = (struct glass_tree) {0};
	if (!lll_arena_init(&tree->arena, total_size_in_byte))
	{
		LLL_PRINT_ERROR("Error: Failed to allocate memory\n");
		return LLL_FALSE;
	}
	tree->parent_stack = lll_arena_alloc(&tree->arena, parent_stack_size_in_byte, 8);
	tree->parent_stack_capacity = parent_stack_size;
	tree->blocks = lll_arena_alloc(&tree->arena, blocks_size_in_byte, 8);
	tree->blocks_capacity = blocks_size;
	lll_ht_init(&tree->blocks_hashed, blocks_size, blocks_size / 2, &tree->arena);
	tree->grid_positions = lll_arena_alloc(&tree->arena, grid_positions_size_in_byte, 4);
	tree->grid_positions_capacity = grid_positions_size;
	glass_tree_memory_print(tree);
	return LLL_TRUE;
}

void  glass_tree_memory_print(struct glass_tree* tree)
{
	lll_u32 parent_stack_size_in_byte = sizeof(struct glass_block*) * tree->parent_stack_capacity;
	lll_u32 parent_stack_usage_in_byte = sizeof(struct glass_block*) * tree->parent_stack_used;
	lll_f32 parent_stack_usage = (lll_f32) tree->parent_stack_used / (lll_f32) tree->grid_positions_capacity;

	lll_u32 blocks_size_in_byte = sizeof(struct glass_block) * tree->blocks_capacity;
	lll_u32 blocks_usage_in_byte = sizeof(struct glass_block) * tree->blocks_used;
	lll_f32 blocks_usage = (lll_f32) tree->blocks_used / (lll_f32) tree->blocks_capacity;

	lll_u32 blocks_ht_in_byte = tree->blocks_hashed.entries_arena.capacity;
	lll_u32 blocks_ht_usage_in_byte = tree->blocks_hashed.entries_arena.used;
	lll_f32 blocks_ht_usage = (lll_f32) tree->blocks_hashed.entries_arena.used / (lll_f32) tree->blocks_hashed.entries_arena.capacity;

	lll_u32 grid_positions_size_in_byte = sizeof(struct glass_block_position) * tree->grid_positions_capacity;
	lll_u32 grid_positions_usage_in_byte = sizeof(struct glass_block_position) * tree->grid_positions_used;
	lll_f32 grid_positions_usage = (lll_f32) tree->grid_positions_used / (lll_f32) tree->grid_positions_capacity;

	lll_u32 tree_in_byte = sizeof(struct glass_tree);

	lll_u32 total_in_byte = parent_stack_size_in_byte + blocks_size_in_byte + blocks_ht_in_byte + grid_positions_size_in_byte + tree_in_byte;
	lll_f32 total_usage = (lll_f32) (parent_stack_usage_in_byte + blocks_usage_in_byte + blocks_ht_usage_in_byte + grid_positions_usage_in_byte + tree_in_byte) / (lll_f32) total_in_byte;
	lll_printf("%20s %10s %10s\n%20s %10u %10f%%\n%20s %10u %10f%%\n%20s %10u %10f%%\n%20s %10u %10f%%\n%20s %10u %10f%%\n%20s %10u %10f%%\n\n",
		"Memory area", "capacity", "usage",
		"parent stack", parent_stack_size_in_byte, parent_stack_usage * 100,
		"glass blocks", blocks_size_in_byte, blocks_usage * 100,
		"glass blocks ht", blocks_ht_in_byte, blocks_ht_usage * 100,
		"grid positions", grid_positions_size_in_byte, grid_positions_usage * 100,
		"tree infos", tree_in_byte, 100.0f,
	  "total", total_in_byte, total_usage * 100);
	// 1.5625 * 2^6
}

struct glass_tree*  glass_tree_get_current()
{
	return glass_root.trees + glass_root.current_tree_index;
}

struct glass_tree*  glass_tree_get_previous()
{
	return glass_root.trees + ((glass_root.current_tree_index + 1) % 2);
}

void  glass_tree_calculate_blocks_dimension(struct glass_tree* tree)
{
	for (struct glass_block* root = tree->root; root != NULL; root = glass_block_get(root->sibling_next, tree))
	{

	}
}

void  glass_tree_calculate_blocks_position(struct glass_tree* tree)
{
	for (struct glass_block* root = tree->root; root != NULL; root = glass_block_get(root->sibling_next, tree))
	{

	}
}

struct glass_block*  glass_block_get(glass_index index, struct glass_tree* tree)
{
	if (index == LLL_INVALID_INDEX)
	{
		return NULL;
	}
	lll_assert(index < tree->blocks_used, "Invalid block index");
	return tree->blocks + index;
}

struct glass_block*  glass_block_get_hashed(lll_u32 hash, struct glass_tree* tree)
{
	struct glass_block** result = (struct glass_block**) lll_ht_get(&tree->blocks_hashed, hash);
	if (result)
	{
		return *result;
	}
	lll_assert(result != NULL, "The hashed block is not found");
	return NULL;
}

glass_index  glass_block_get_index(struct glass_block* block, struct glass_tree* tree)
{
	lll_assert(block >= tree->blocks, "Invalid block pointer");
	glass_index result = block - tree->blocks;
	lll_assert(result < tree->blocks_used, "Invalid block pointer");
	return result;
}

struct glass_block*  glass_block_create(struct glass_tree* tree)
{
	lll_assert(tree->blocks_used < tree->blocks_capacity, "Out of memory when creating glass block");
	struct glass_block* result = tree->blocks + tree->blocks_used;
	tree->blocks_used++;
	*result = (struct glass_block) {0};
	result->parent = LLL_INVALID_INDEX;
	result->child_first = LLL_INVALID_INDEX;
	result->child_last = LLL_INVALID_INDEX;
	result->sibling_previous = LLL_INVALID_INDEX;
	result->sibling_next = LLL_INVALID_INDEX;
	if (tree->parent_stack_used > 0)
	{
		glass_index index = tree->blocks_used - 1;
		struct glass_block* parent = tree->parent_stack[tree->parent_stack_used - 1];
		result->parent = glass_block_get_index(parent, tree);
		if (parent->child_first == LLL_INVALID_INDEX)
		{
			// Note: first child
			parent->child_first = index;
			parent->child_last = index;
		}
		else if (parent->child_last != LLL_INVALID_INDEX)
		{
			// Note: Has siblings
			glass_index         sibling_index = parent->child_last;
			struct glass_block* sibling = glass_block_get(sibling_index, tree);
			lll_assert(sibling->sibling_next == LLL_INVALID_INDEX, "Last sibling should have no next sibling");
			sibling->sibling_next = index;
			result->sibling_previous = sibling_index;
			parent->child_last = index;
		}
	}
	return result;
}

void  glass_block_push_parent(struct glass_block* block, struct glass_tree* tree)
{
	lll_assert(tree->parent_stack_used < tree->parent_stack_capacity, "Out of memory when pushing to parent stack");
	tree->parent_stack[tree->parent_stack_used] = block;
	tree->parent_stack_used++;
}

void  glass_block_pop_parent(struct glass_tree* tree)
{
	lll_assert(tree->parent_stack_used > 0, "There is no more blocks to pop in the parent stack");
	tree->parent_stack_used--;
}

struct glass_block_position*  glass_block_layout_grid_get_positions(glass_index index, struct glass_tree* tree)
{
	if (index == LLL_INVALID_INDEX)
	{
		return NULL;
	}
	lll_assert(index < tree->grid_positions_used, "Invalid grid positions index");
	return tree->grid_positions + index;
}

struct glass_block_position*  glass_block_layout_grid_create_positions(struct glass_block_layout_grid* grid, lll_uv2 dimension, struct glass_tree* tree)
{
	lll_assert((dimension.x > 0) && (dimension.y > 0), "Dimension of grid must be at least (1, 1)");
	lll_u32 positions_size = dimension.x + dimension.y;
	struct glass_block_position* result = NULL;
	if ((tree->grid_positions_capacity - tree->grid_positions_used) >= positions_size)
	{
		grid->dimension = dimension;
		grid->positions = tree->grid_positions_used;
		result = tree->grid_positions + tree->grid_positions_used;
		tree->grid_positions_used += positions_size;
	}
	else
	{
		lll_assert(LLL_FALSE, "Out of memory when creating grid positions");
	}
	return result;
}
#endif
#endif
