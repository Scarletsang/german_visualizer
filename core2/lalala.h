#ifndef LALALA
#define LALALA

#define LLL_TRUE	1
#define LLL_FALSE	0
#define LLL_INVALID_INDEX	((lll_u32) -1)
#define LLL_INVALID_SIZE	((lll_u32) -1)
#define LLL_PAGE_SIZE	(4096)

typedef unsigned int  lll_u32;
typedef int           lll_i32;
typedef unsigned int  lll_b32;
typedef float         lll_f32;
typedef char          lll_i8;
typedef unsigned char lll_b8;
typedef unsigned char lll_u8;
typedef long          lll_u64;
typedef long          lll_usecond; // Note: microseconds
typedef int           lll_pid;

#ifdef DEBUG
void	lll_assert_explicit(lll_b8 check, const char* message, int line_number, const char* function_name, const char* file_name);
#define lll_assert(check, message) lll_assert_explicit(check, message, __LINE__, __FUNCTION__, __FILE__) 
#else
#define lll_assert(check, message) ((void) 0)
#endif

#define LLL_PRINT_ERROR(cstring) (write(2, cstring, sizeof(cstring) - 1))
#define LLL_OFFSET_OF(data_ptr, field_name) (((lll_u8*)&(data_ptr)->field_name) - (lll_u8*) (data_ptr))

typedef struct
{
	char*	data;
	lll_u32	length;
}	lll_string;

#define lll_cstring(c_string) ((lll_string) {c_string, sizeof(c_string) - 1})
lll_u32	lll_strlen(char* string);
lll_u32	lll_strchr(lll_string string, char c);
lll_b8	lll_string_is_equal(lll_string target, lll_string reference);
void	lll_memcpy(void* dest, void* src, lll_u32 size);
void	lll_memset(void* dest, lll_u32 size, char data);
lll_u32	lll_sprintf(lll_string buffer, const char* format, ...);
// Can only print 1024 characters at maximum
lll_u32	lll_fdprintf(int fd, const char* format, ...);
#define lll_printf(format, ...) lll_fdprintf(STDOUT_FILENO, format __VA_OPT__(,) __VA_ARGS__)

typedef struct
{
	void*	data;
	lll_u32	length;
}	lll_slice;

typedef struct
{
	lll_u8*	memory;
	lll_u32	used;
	lll_u32	capacity;
}	lll_arena;

typedef lll_u32	lll_arena_snapshot;

lll_b8	lll_arena_init(lll_arena* arena, lll_u32 size);
void	lll_arena_split(lll_arena* arena, lll_u32 size, lll_arena* output);
void*	lll_arena_alloc(lll_arena* arena, lll_u32 size, lll_u32 alignment);
void	lll_arena_clear(lll_arena* arena);
void  lll_arena_destroy(lll_arena* arena);
lll_arena_snapshot	lll_arena_cheese(lll_arena* arena);
void	lll_arena_rollback(lll_arena* arena, lll_arena_snapshot snapshot);

union lll_ht_entry
{
	struct
	{
		void*								data;
		union lll_ht_entry*	next;
		lll_u32             hash;
	};
	struct
	{
		lll_u64             is_occupied;
		union lll_ht_entry* next_deleted;
	};
};

typedef struct lll_ht
{
	union lll_ht_entry* entries;
	union lll_ht_entry* free_list;
	lll_u32             capacity;
	lll_arena           entries_arena;
}	lll_ht;

lll_b8	lll_ht_init(lll_ht*	hashtable, lll_u32 capacity, lll_u32 duplicated_capacity, lll_arena* arena);
void		lll_ht_set(lll_ht* hashtable, lll_u32 key, void* value);
void*		lll_ht_remove(lll_ht* hashtable, lll_u32 key);
void**	lll_ht_get(lll_ht* hashtable, lll_u32 key);
void		lll_ht_clear(lll_ht* hashtable);
lll_u32	lll_hash_string(lll_string string);

typedef struct
{
	lll_u32 x;
	lll_u32 y;
} lll_uv2;

typedef struct
{
	lll_f32 x;
	lll_f32 y;
} lll_v2;

typedef union
{
	struct
	{
		lll_f32 x;
		lll_f32 y;
		lll_f32 z;
	};
	struct
	{
		lll_f32 r;
		lll_f32 g;
		lll_f32 b;
	};
	lll_v2    xy;
} lll_v3;

typedef union
{
	struct
	{
		lll_f32 x;
		lll_f32 y;
		lll_f32 z;
		lll_f32 w;
	};
	struct
	{
		lll_f32 r;
		lll_f32 g;
		lll_f32 b;
		lll_f32 a;
	};
	struct
	{
		lll_v2  xy;
		lll_v2  zw;
	};
	lll_v3    xyz;
	lll_v3    rgb;
} lll_v4;

lll_u32 lll_math_u32_clamp(lll_u32 number, lll_u32 min, lll_u32 max);
#endif

#ifdef LALALA_IMPLEMENTATION
#ifndef _LALALA_IMPLEMENTATION
#define _LALALA_IMPLEMENTATION
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/mman.h>

void	lll_assert_explicit(lll_b8 check, const char* message, int line_number, const char* function_name, const char* file_name)
{
	if (!check)
	{
		lll_fdprintf(STDERR_FILENO, "ASSERT TRIGGERED in %s at function %s, line %d:\nReason: %s\n", file_name, function_name, line_number, message);
		kill(getpid(), SIGABRT);
	}
}

lll_u32	lll_strlen(char* string)
{
	lll_u32	length = 0;
	while (*string)
	{
		string++;
		length++;
	}
	return length;
}

lll_u32	lll_strchr(lll_string string, char c)
{
	for (lll_u32 i = 0; i < string.length; i++)
	{
		if (*string.data == c)
		{
			return i;
		}
		string.data++;
	}
	return LLL_INVALID_INDEX;
}

lll_b8	lll_string_is_equal(lll_string target, lll_string reference)
{
	lll_u32	matched = 0;
	while ((matched < target.length) &&
		   (matched < reference.length) &&
		   (*target.data == *reference.data))
	{
		target.data++;
		reference.data++;
		matched++;
	}
	return (matched == reference.length) && (matched == target.length);
}

void	lll_memcpy(void* dest, void* src, lll_u32 size)
{
	lll_u8* dest_ptr = (lll_u8*) dest;
	lll_u8* src_ptr = (lll_u8*) src;
	for (lll_u32 i = 0; i < size; i++)
	{
		*dest_ptr = *src_ptr;
		dest_ptr++;
		src_ptr++;
	}
}

void	lll_memset(void* dest, lll_u32 size, char data)
{
	lll_i8* dest_ptr = (lll_i8*) dest;
	for (lll_u32 i = 0; i < size; i++)
	{
		*dest_ptr = data;
		dest_ptr++;
	}
}

struct lll_sprintf_state
{
	lll_b8	is_left_justify : 1; // -
	lll_b8	is_force_positive_sign : 1; // +
	lll_b8	is_force_positive_space : 1; // ' '
	lll_b8	is_binary : 1; // b
	lll_b8	is_binary_big_endian : 1; // B
	lll_b8	is_binary_hex : 1; // x
	lll_b8	has_width : 1;
	lll_b8	has_precision : 1;
	lll_u32	width; // Note: minimum characters to print
	lll_u32	precision; // Note: for string, maximum characters to print; for number, minimum of digits to print
};

static lll_u8	lll_sprintf_binary(lll_u64 data, lll_u8 data_size, struct lll_sprintf_state state, char* output)
{
	lll_u8	output_size = 0;
	if (state.is_binary_big_endian)
	{
		if (state.is_binary_hex)
		{
			output_size = data_size * 2 + (data_size - 1);
			char* write_ptr = output + output_size - 2;
			for (int i = 0; i < output_size; i += 3)
			{
				if ((data & 0xf) >= 10)
				{
					write_ptr[1] = 'a' + (data & 0xf) - 10;
				}
				else
				{
					write_ptr[1] = '0' + (data & 0xf);
				}
				if (((data >> 4) & 0xf) >= 10)
				{
					write_ptr[0] = 'a' + ((data >> 4) & 0xf) - 10;
				}
				else
				{
					write_ptr[0] = '0' + ((data >> 4) & 0xf);
				}
				write_ptr[2] = ' ';
				data >>= 8;
				write_ptr -= 3;
			}
		}
		else
		{
			output_size = data_size * 8 + (data_size - 1);
			char* write_ptr = output + output_size - 1;
			for (int i = 0; i < (data_size * 8); i++)
			{
				*write_ptr = '0' + (data & 1);
				data >>= 1;
				write_ptr--;
				if (i % 8 == 7)
				{
					*write_ptr = ' ';
					write_ptr--;
				}
			}
		}
	}
	else if (state.is_binary || state.is_binary_hex)
	{
		if (state.is_binary_hex)
		{
			output_size = data_size * 2 + (data_size - 1);
			char* write_ptr = output;
			for (int i = 0; i < data_size; i++)
			{
				if ((data & 0xf) >= 10)
				{
					write_ptr[1] = 'a' + (data & 0xf) - 10;
				}
				else
				{
					write_ptr[1] = '0' + (data & 0xf);
				}
				if (((data >> 4) & 0xf) >= 10)
				{
					write_ptr[0] = 'a' + ((data >> 4) & 0xf) - 10;
				}
				else
				{
					write_ptr[0] = '0' + ((data >> 4) & 0xf);
				}
				write_ptr[2] = ' ';
				data >>= 8;
				write_ptr += 3;
			}
		}
		else
		{
			output_size = data_size * 8 + (data_size - 1);
			char* write_ptr = output + 8;
			*write_ptr = ' ';
			write_ptr--;
			for (int i = 0; i < data_size; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					*write_ptr = '0' + (data & 1);
					data >>= 1;
					write_ptr--;
				}
				write_ptr += 18;
				*write_ptr = ' ';
				write_ptr--;
			}
		}
	}
	return output_size;
}

static void	lll_sprintf_output(char** buffer_memory, lll_string* buffer, char* data, lll_u32 data_size)
{
	lll_u32	writable_length = buffer->length - (*buffer_memory - buffer->data);
	if (writable_length > data_size)
	{
		lll_memcpy(*buffer_memory, data, data_size);
		*buffer_memory += data_size;
	}
	else
	{
		lll_memcpy(*buffer_memory, data, writable_length);
		*buffer_memory += writable_length;
	}
}

static void	lll_sprintf_output_characters(char** buffer_memory, lll_string* buffer, char character, lll_u32 amount)
{
	lll_u32	writable_length = buffer->length - (*buffer_memory - buffer->data);
	lll_u32	write_length = 0;
	if (writable_length > amount)
	{
		write_length = amount;
	}
	else
	{
		write_length = writable_length;
	}
	for (lll_u32 i = 0; i < write_length; i++)
	{
		**buffer_memory = character;
		(*buffer_memory)++;
	}
}

lll_u32	lll_vsprintf(lll_string buffer, const char* format, va_list args)
{
	char*	buffer_memory = buffer.data;
#define lll_sprintf_has_space(n) ((buffer_memory - buffer.data + n) < buffer.length)
	while (*format && lll_sprintf_has_space(0))
	{
		if (*format == '%')
		{
			format++;
			struct lll_sprintf_state state = {0};
			// Note: sprintf flags
			for (lll_i32 i = 0; (i < 4) && *format; i++)
			{
				switch (*format)
				{
				case '-': state.is_left_justify         = LLL_TRUE; break;
				case '+': state.is_force_positive_sign  = LLL_TRUE; break;
				case ' ': state.is_force_positive_space = LLL_TRUE; break;
				case 'b': state.is_binary               = LLL_TRUE; break;
				case 'B': state.is_binary_big_endian    = LLL_TRUE; break;
				case 'x': state.is_binary_hex           = LLL_TRUE; break;
				default: goto no_flags;
				}
				format++;
			}
no_flags:
			// Note: sprintf width
			if (*format == '*')
			{
				state.has_width = LLL_TRUE;
				state.width = va_arg(args, lll_u32);
				format++;
			}
			else
			{
				lll_u32	width = 0;
				const char*	start = format;
				while ((*format >= '0') && (*format <= '9'))
				{
					width = width * 10 + (*format - '0');
					format++;
				}
				if ((format - start) > 0)
				{
					state.has_width = LLL_TRUE;
					state.width = width;
				}
			}
			// Note: sprintf precision
			if (*format == '.')
			{
				format++;
				state.has_precision = LLL_TRUE;
				if (*format == '*')
				{
					state.precision = va_arg(args, lll_u32);
					format++;
				}
				else
				{
					lll_u32	precision = 0;
					while ((*format >= '0') && (*format <= '9'))
					{
						precision = precision * 10 + (*format - '0');
						format++;
					}
					state.precision = precision;
				}
			}
			static char	substitution_buffer[64 + 7];
			lll_u8		substitution_buffer_size = 0;
			switch (*format)
			{
			case 'd':
			case 'i':
			{
				lll_i32	number = va_arg(args, lll_i32);
				substitution_buffer_size = lll_sprintf_binary(number, sizeof(number), state, substitution_buffer);
				if (substitution_buffer_size == 0)
				{
					if (number == 0)
					{
						*substitution_buffer = '0';
						substitution_buffer_size = 1;
					}
					else
					{
						lll_i32	temp = number;
						while (temp != 0)
						{
							temp /= 10;
							substitution_buffer_size++;
						}
						temp = number;
						char*	start = substitution_buffer + substitution_buffer_size - 1;
						for (int i = substitution_buffer_size - 1; i >= 0; i--)
						{
							if (number > 0)
							{
								*start = '0' + (temp % 10);
							}
							else
							{
								*start = '0' + -(temp % 10);
							}
							temp /= 10;
							start--;
						}
					}
					lll_u32	zero_padding_length = 0;
					if (state.has_precision && (state.precision > substitution_buffer_size))
					{
						zero_padding_length = state.precision - substitution_buffer_size;
					}
					lll_u32	space_padding_length = 0;
					if (state.has_width && (state.width > (substitution_buffer_size + zero_padding_length)))
					{
						space_padding_length = state.width - (substitution_buffer_size + zero_padding_length);
					}
					if (state.is_left_justify)
					{
						lll_sprintf_output_characters(&buffer_memory, &buffer, '0', zero_padding_length);
						if ((number < 0) && lll_sprintf_has_space(0))
						{
							*buffer_memory = '-';
							buffer_memory++;
						}
						else if ((number >= 0) && state.is_force_positive_sign && lll_sprintf_has_space(0))
						{
							*buffer_memory = '+';
							buffer_memory++;
						}
						lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);
						lll_sprintf_output_characters(&buffer_memory, &buffer, ' ', space_padding_length); 
					}
					else
					{
						lll_sprintf_output_characters(&buffer_memory, &buffer, ' ', space_padding_length); 
						lll_sprintf_output_characters(&buffer_memory, &buffer, '0', zero_padding_length); 
						if ((number < 0) && lll_sprintf_has_space(0))
						{
							*buffer_memory = '-';
							buffer_memory++;
						}
						else if ((number >= 0) && state.is_force_positive_sign && lll_sprintf_has_space(0))
						{
							*buffer_memory = '+';
							buffer_memory++;
						}
						lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);
					}
				}
				else
				{
					lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);
				}
				format++;
			} break;
			case 'u':
			{
				lll_u32	number = va_arg(args, lll_u32);
				substitution_buffer_size = lll_sprintf_binary(number, sizeof(number), state, substitution_buffer);
				if (substitution_buffer_size == 0)
				{
					if (number == 0)
					{
						*substitution_buffer = '0';
						substitution_buffer_size = 1;
					}
					else
					{
						lll_u32	temp = number;
						while (temp != 0)
						{
							temp /= 10;
							substitution_buffer_size++;
						}
						temp = number;
						char*	start = substitution_buffer + substitution_buffer_size - 1;
						for (int i = substitution_buffer_size - 1; i >= 0; i--)
						{
							*start = '0' + (temp % 10);
							temp /= 10;
							start--;
						}
					}
					lll_u32	zero_padding_length = 0;
					if (state.has_precision && (state.precision > substitution_buffer_size))
					{
						zero_padding_length = state.precision - substitution_buffer_size;
					}
					lll_u32	space_padding_length = 0;
					if (state.has_width && (state.width > (substitution_buffer_size + zero_padding_length)))
					{
						space_padding_length = state.width - (substitution_buffer_size + zero_padding_length);
					}
					if (state.is_left_justify)
					{
						lll_sprintf_output_characters(&buffer_memory, &buffer, '0', zero_padding_length); 
						lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);
						lll_sprintf_output_characters(&buffer_memory, &buffer, ' ', space_padding_length); 
					}
					else
					{
						lll_sprintf_output_characters(&buffer_memory, &buffer, ' ', space_padding_length); 
						lll_sprintf_output_characters(&buffer_memory, &buffer, '0', zero_padding_length); 
						lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);

					}
				}
				else
				{
					lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);
				}
				format++;
			} break;
			case 'p':
			{
				void*	pointer = va_arg(args, void*);
				if (pointer == 0)
				{
					*substitution_buffer = '0';
					substitution_buffer_size = 1;
				}
				else
				{
					lll_u64	temp = (lll_u64) pointer;
					while (temp != 0)
					{
						temp /= 16;
						substitution_buffer_size++;
					}
					temp = (lll_u64) pointer;
					char*	start = substitution_buffer + substitution_buffer_size - 1;
					for (int i = substitution_buffer_size - 1; i >= 0; i--)
					{
						if (temp % 16 >= 10)
						{
							*start = 'a' + (temp % 16) - 10;
						}
						else
						{
							*start = '0' + (temp % 16);
						}
						temp /= 16;
						start--;
					}
				}
				lll_sprintf_output(&buffer_memory, &buffer, "0x", 2);
				lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);
				format++;
			} break;
			case 's':
			{
				char*	string = va_arg(args, char*);
				lll_u32	output_length = 0;
				if (state.has_precision)
				{
					output_length = state.precision;
				}
				else
				{
					output_length = lll_strlen(string);
				}
				if (state.has_width && (state.width > output_length))
				{
					// Note: Write padding
					lll_u32	padding_length = state.width - output_length;
					if (state.is_left_justify)
					{
						lll_sprintf_output(&buffer_memory, &buffer, string, output_length);
						lll_sprintf_output_characters(&buffer_memory, &buffer, ' ', padding_length);
					}
					else
					{
						lll_sprintf_output_characters(&buffer_memory, &buffer, ' ', padding_length);
						lll_sprintf_output(&buffer_memory, &buffer, string, output_length);
					}
				}
				else
				{
					lll_sprintf_output(&buffer_memory, &buffer, string, output_length);
				}
				format++;
			} break;
			case 'c':
			{
				char	character = va_arg(args, lll_i32);
				substitution_buffer_size = lll_sprintf_binary(character, sizeof(character), state, substitution_buffer);
				if (substitution_buffer_size == 0)
				{
					if (lll_sprintf_has_space(0))
					{
						*buffer_memory = character;
						buffer_memory++;
					}
				}
				else
				{
					lll_sprintf_output(&buffer_memory, &buffer, substitution_buffer, substitution_buffer_size);
				}
				format++;
			} break;
			case '%':
			{
				if (lll_sprintf_has_space(0))
				{
					*buffer_memory = '%';
					buffer_memory++;
				}
				format++;
			} break;
			case 0: // Note: syntax error but no need to produce an error
			default:
				break;
			}
		}
		else
		{
			*buffer_memory = *format;
			buffer_memory++;
			format++;
		}
	}
#undef lll_sprintf_has_space
	return (buffer_memory - buffer.data);
}

lll_u32	lll_sprintf(lll_string buffer, const char* format, ...)
{
	va_list	args;
	va_start(args, format);
	lll_u32 result = lll_vsprintf(buffer, format, args);
	va_end(args);
	return result;
}

lll_u32	lll_fdprintf(int fd, const char* format, ...)
{
	static char buffer_memory[1024];
	static lll_string buffer = {buffer_memory, 1024};
	va_list	args;
	va_start(args, format);
	lll_u32 result = lll_vsprintf(buffer, format, args);
	write(fd, buffer_memory, result);
	va_end(args);
	return result;
}

lll_b8	lll_arena_init(lll_arena* arena, lll_u32 size)
{
	arena->memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	lll_assert((arena->memory != NULL), "mmap failed");
	arena->capacity = size;
	arena->used = 0;
	return (arena->memory != ((void*) -1));
}

void	lll_arena_split(lll_arena* arena, lll_u32 size, lll_arena* output)
{
	output->memory = lll_arena_alloc(arena, size, 8);
	output->used = 0;
	output->capacity = size;
}

void*	lll_arena_alloc(lll_arena* arena, lll_u32 size, lll_u32 alignment)
{
	lll_assert(((alignment & (alignment - 1)) == 0), "Alignment is not power of two");
	lll_u8*	result = arena->memory + arena->used;
	lll_u64 modulo = (lll_u64)result & (alignment - 1);
	if (modulo != 0)
	{
		result += (alignment - modulo);
		size += (alignment - modulo);
	}
	lll_assert((arena->used + size) <= (arena->capacity), "Arena overflow");
	arena->used += size;
	return (void*) result;
}

void	lll_arena_clear(lll_arena* arena)
{
	arena->used = 0;
}

void  lll_arena_destroy(lll_arena* arena)
{
	munmap(arena->memory, arena->capacity);
	*arena = (lll_arena) {0};
}

lll_arena_snapshot	lll_arena_cheese(lll_arena* arena)
{
	return arena->used;
}

void	lll_arena_rollback(lll_arena* arena, lll_arena_snapshot snapshot)
{
	arena->used = snapshot;
}

lll_b8	lll_ht_init(lll_ht*	hashtable, lll_u32 capacity, lll_u32 duplicated_capacity, lll_arena* arena)
{
	lll_b8 result = LLL_FALSE;
	lll_u32 size = sizeof(union lll_ht_entry) * (capacity + duplicated_capacity);
	if (arena)
	{
		lll_arena_split(arena, size, &hashtable->entries_arena);
		result = LLL_TRUE;
	}
	else
	{
		result = lll_arena_init(&hashtable->entries_arena, size);
	}
	hashtable->entries = (union lll_ht_entry*) hashtable->entries_arena.memory;
	hashtable->free_list = NULL;
	hashtable->capacity = capacity;
	return result;
}

void	lll_ht_set(lll_ht* hashtable, lll_u32 key, void* value)
{
	lll_assert(value != NULL, "hashtable value cannot be zero");
	union lll_ht_entry* entry = hashtable->entries + (key % hashtable->capacity);
	if (!entry->is_occupied)
	{
		entry->data = value;
		entry->next = NULL;
		entry->hash = key;
	}
	else
	{
		lll_b8	found_myself = LLL_FALSE;
		while (LLL_TRUE)
		{
			if (entry->hash == key)
			{
				found_myself = LLL_TRUE;
				break;
			}
			if (entry->next)
			{
				entry = entry->next;
			}
			else
			{
				break;
			}
		}
		if (found_myself)
		{
			entry->data = value;
		}
		else
		{
			if (hashtable->free_list)
			{
				entry->next = hashtable->free_list;
				hashtable->free_list = entry->next->next_deleted;
			}
			else
			{
				entry->next = lll_arena_alloc(&hashtable->entries_arena, sizeof(union lll_ht_entry), 8);
			}
			entry = entry->next;
			entry->data = value;
			entry->next = NULL;
			entry->hash = key;
		}
	}
}

void*	lll_ht_remove(lll_ht* hashtable, lll_u32 key)
{
	void*	result = NULL;
	union lll_ht_entry* entry = hashtable->entries + (key % hashtable->capacity);
	if (entry->is_occupied)
	{
		if (entry->hash == key)
		{
			result = entry->data;
			if (entry->next)
			{
				union lll_ht_entry* next = entry->next;
				*entry = *next;
				next->is_occupied = LLL_FALSE;
				next->next_deleted = hashtable->free_list;
				hashtable->free_list = next;
			}
			else
			{
				entry->is_occupied = LLL_FALSE;
				entry->next_deleted = NULL;
			}
		}
		else
		{
			union lll_ht_entry* entry_previous = entry;
			entry = entry->next;
			while (entry)
			{
				if (entry->hash == key)
				{
					result = entry->data;
					entry_previous->next = entry->next;
					entry->is_occupied = LLL_FALSE;
					entry->next_deleted = hashtable->free_list;
					hashtable->free_list = entry;
					break;
				}
				entry_previous = entry;
				entry = entry->next;
			}
		}
	}
	return result;
}

void**	lll_ht_get(lll_ht* hashtable, lll_u32 key)
{
	void**	result = NULL;
	union lll_ht_entry* entry = hashtable->entries + (key % hashtable->capacity);
	if (entry->is_occupied)
	{
		while (LLL_TRUE)
		{
			if (entry->hash == key)
			{
				result = &entry->data;
				break;
			}
			if (entry->next)
			{
				entry = entry->next;
			}
			else
			{
				break;
			}
		}
	}
	return result;
}

void		lll_ht_clear(lll_ht* hashtable)
{
	lll_memset(hashtable->entries, sizeof(union lll_ht_entry) * hashtable->capacity, 0);
	hashtable->free_list = NULL;
	lll_arena_clear(&hashtable->entries_arena);
}

lll_u32	lll_hash_string(lll_string string)
{
	lll_u32 seed = 5381;
	for (lll_u32 i = 0; i < string.length; i++)
	{
		seed = ((seed << 5) + seed) + *string.data;
		string.data++;
	}
	return seed;
}

lll_u32 lll_math_u32_clamp(lll_u32 number, lll_u32 min, lll_u32 max)
{
	if (number < min)
	{
		number = min;
	}
	if (number > max)
	{
		number = max;
	}
	return number;
}
#endif
#endif
