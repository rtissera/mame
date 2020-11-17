// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    a64log.h

    AArch64 code logging helpers.

***************************************************************************/

#ifndef MAME_CPU_A64LOG_H
#define MAME_CPU_A64LOG_H

#pragma once

#include <cstdint>
#include <cassert>


/***************************************************************************
    CONSTANTS
***************************************************************************/

/* comment parameters */
constexpr int A64LOG_MAX_COMMENTS{4000};
constexpr int A64LOG_MAX_DATA_RANGES{1000};
constexpr int A64LOG_COMMENT_POOL_SIZE{A64LOG_MAX_COMMENTS * 40};


/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

// use a64code * to reference generated code
typedef uint8_t       a64code;


/* code logging info */
struct a64log_comment
{
	a64code* base = nullptr;
	const char* string = nullptr;
};


/* data ranges */
struct a64log_data_range_t
{
	a64code* base = nullptr;
	a64code* end = nullptr;
	int size = 0;
};


/* the code logging context */
struct a64log_context
{
	std::string filename;                       // name of the file
	FILE* file = nullptr;                       // file we are logging to

	a64log_data_range_t data_range[A64LOG_MAX_DATA_RANGES];   // list of data ranges
	int data_range_count = 0;                   // number of data ranges

	a64log_comment comment_list[A64LOG_MAX_COMMENTS];     // list of comments
	int comment_count = 0;                      // number of live comments

	char comment_pool[A64LOG_COMMENT_POOL_SIZE];       // string pool to hold comments
	char* comment_pool_next = nullptr;          // pointer to next string pool location
};



/***************************************************************************
    FUNCTION PROTOTYPES
***************************************************************************/

/* create a new context */
a64log_context* a64log_create_context(const char* filename);

/* release a context */
void a64log_free_context(a64log_context* log) noexcept;

/* add a comment associated with a given code pointer */
template <typename... Ts>
inline void a64log_add_comment(
	a64log_context* log, a64code* base, const char* format, Ts&&... xs);

/* mark a given range as data for logging purposes */
void a64log_mark_as_data(
	a64log_context* log, a64code* base, a64code* end, int size) noexcept;

/* disassemble a range of code and reset accumulated information */
void a64log_disasm_code_range(
	a64log_context* log, const char* label, a64code* start, a64code* stop);

/* manually printf information to the log file */
template <typename... Ts>
inline void a64log_printf(a64log_context* log, const char* format, Ts&&... xs);


/*-------------------------------------------------
    a64log_add_comment - add a comment associated
    with a given code pointer
-------------------------------------------------*/

template <typename... Ts>
inline void a64log_add_comment(
	a64log_context* log, a64code* base, const char* format, Ts&&... xs)
{
	char* string = log->comment_pool_next;
	a64log_comment* comment;

	assert(log->comment_count < A64LOG_MAX_COMMENTS);
	assert(log->comment_pool_next + strlen(format) + 256 <
			log->comment_pool + A64LOG_COMMENT_POOL_SIZE);

	/* we assume comments are registered in order; enforce this */
	assert(log->comment_count == 0 ||
			base >= log->comment_list[log->comment_count - 1].base);

	/* if we exceed the maxima, skip it */
	if(log->comment_count >= A64LOG_MAX_COMMENTS) return;
	if(log->comment_pool_next + strlen(format) + 256 >=
		log->comment_pool + A64LOG_COMMENT_POOL_SIZE)
		return;

	/* do the printf to the string pool */
	log->comment_pool_next +=
		sprintf(log->comment_pool_next, format, std::forward<Ts>(xs)...) + 1;

	/* fill in the new comment */
	comment = &log->comment_list[log->comment_count++];
	comment->base = base;
	comment->string = string;
}


/*-------------------------------------------------
    a64log_printf - manually printf information to
    the log file
-------------------------------------------------*/

template <typename... Ts>
inline void a64log_printf(a64log_context* log, const char* format, Ts&&... xs)
{
	/* open the file, creating it if necessary */
	if(log->file == nullptr)
	{
		log->file = fopen(log->filename.c_str(), "w");

		if(log->file == nullptr) return;
	}

	assert(log->file != nullptr);

	/* do the printf */
	fprintf(log->file, format, std::forward<Ts>(xs)...);

	/* flush the file */
	fflush(log->file);
}

#endif // MAME_CPU_A64LOG_H
