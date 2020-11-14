// license:BSD-3-Clause
// copyright-holders:Vas Crabb
/***************************************************************************

    eigccarm.h

    arm (32 and 64-bit) inline implementations for GCC compilers. This
    code is automatically included if appropriate by eminline.h.

***************************************************************************/

#ifndef MAME_OSD_EIGCCARM_H
#define MAME_OSD_EIGCCARM_H

/***************************************************************************
    INLINE MATH FUNCTIONS
***************************************************************************/

/*-------------------------------------------------
    mul_32x32 - perform a signed 32 bit x 32 bit
    multiply and return the full 64 bit result
-------------------------------------------------*/

#if defined(__aarch64__)
#define mul_32x32 _mul_32x32
inline int64_t ATTR_CONST ATTR_FORCE_INLINE
_mul_32x32(int32_t a, int32_t b)
{
	int64_t result;
	__asm__ (
		" smull  %x[result], %w[a], %w[b] ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r"  (b)        // 'b' should also be in a register
		: "cc"                     // Clobbers condition codes
	);
	return result;
}
#endif


/*-------------------------------------------------
    mulu_32x32 - perform an unsigned 32 bit x
    32 bit multiply and return the full 64 bit
    result
-------------------------------------------------*/

#if defined(__aarch64__)
#define mulu_32x32 _mulu_32x32
inline uint64_t ATTR_CONST ATTR_FORCE_INLINE
_mulu_32x32(uint32_t a, uint32_t b)
{
	uint64_t result;
	__asm__ (
		" umull  %x[result], %w[a], %w[b] ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r"  (b)        // 'b' can be memory or register
		: "cc"                     // Clobbers condition codes
	);
	return result;
}
#endif


#if defined(__aarch64__)
/*-------------------------------------------------
    mul_32x32_hi - perform a signed 32 bit x 32 bit
    multiply and return the upper 32 bits of the
    result
-------------------------------------------------*/

#define mul_32x32_hi _mul_32x32_hi
inline int32_t ATTR_CONST ATTR_FORCE_INLINE
_mul_32x32_hi(int32_t a, int32_t b)
{
	int32_t result;
	__asm__ (
		" smull  %x[result], %w[a], %w[b] ;"
		" lsr    %x[result], %x[result], 32 ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r"  (b)        // 'b' should also be in a register
		: "cc"                     // Clobbers condition codes
	);
	return result;
}


/*-------------------------------------------------
    mulu_32x32_hi - perform an unsigned 32 bit x
    32 bit multiply and return the upper 32 bits
    of the result
-------------------------------------------------*/

#define mulu_32x32_hi _mulu_32x32_hi
inline uint32_t ATTR_CONST ATTR_FORCE_INLINE
_mulu_32x32_hi(uint32_t a, uint32_t b)
{
	uint32_t result;
	__asm__ (
		" umull  %x[result], %w[a], %w[b] ;"
		" lsr    %x[result], %x[result], 32 ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r"  (b)        // 'b' should also be in a register
		: "cc"                     // Clobbers condition codes
	);
	return result;
}
#endif

/*-------------------------------------------------
    mul_32x32_shift - perform a signed 32 bit x
    32 bit multiply and shift the result by the
    given number of bits before truncating the
    result to 32 bits
-------------------------------------------------*/

#if defined(__aarch64__)
#define mul_32x32_shift _mul_32x32_shift
inline int32_t ATTR_CONST ATTR_FORCE_INLINE
_mul_32x32_shift(int32_t a, int32_t b, uint8_t shift)
{
	int32_t result;

	// Valid for (0 <= shift <= 63)
	__asm__ (
		" smull  %x[result], %w[a], %w[b] ;"
		" lsr    %x[result], %x[result], %[shift] ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r"  (b)        // 'b' should also be in a register
		, [shift]  "L"  (shift)    // 'shift' should be a immediate constant
		: "cc"                     // Clobbers condition codes
	);
	return result;
}
#endif

/*-------------------------------------------------
    mulu_32x32_shift - perform an unsigned 32 bit x
    32 bit multiply and shift the result by the
    given number of bits before truncating the
    result to 32 bits
-------------------------------------------------*/

#if defined(__aarch64__)
#define mulu_32x32_shift _mulu_32x32_shift
inline uint32_t ATTR_CONST ATTR_FORCE_INLINE
_mulu_32x32_shift(uint32_t a, uint32_t b, uint8_t shift)
{
	uint32_t result;

	// Valid for (0 <= shift <= 63)
	__asm__ (
		" umull  %x[result], %w[a], %w[b] ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r" (b)         // 'b' should also be in a register
		, [shift]  "L"  (shift)    // 'shift' should be a immediate constant
		: "cc"                     // Clobbers condition codes
	);
	return result;
}
#endif


/*-------------------------------------------------
    div_64x32 - perform a signed 64 bit x 32 bit
    divide and return the 32 bit quotient
-------------------------------------------------*/

#if defined(__aarch64__)
#define div_64x32 _div_64x32
inline int32_t ATTR_CONST ATTR_FORCE_INLINE
_div_64x32(int64_t a, int32_t b)
{
	int32_t result;
	__asm__ (
		" sxtw   %x[b], %w[b] ;"
		" sdiv   %x[result], %x[a], %x[b] ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r"  (b)        // 'b' should also be in a register
		: "cc"                     // Clobbers condition codes
	);
	return result;
}
#endif


/*-------------------------------------------------
    divu_64x32 - perform an unsigned 64 bit x 32 bit
    divide and return the 32 bit quotient
-------------------------------------------------*/

#if defined(__aarch64__)
#define divu_64x32 _divu_64x32
inline uint32_t ATTR_CONST ATTR_FORCE_INLINE
_divu_64x32(uint64_t a, uint32_t b)
{
	uint32_t result;
	__asm__ (
//		" uxtw   %x[b], %w[b] ;"
		" udiv   %x[result], %x[a], %x[b] ;"
		: [result] "=r" (result)   // result in a register
		: [a]      "%r" (a)        // 'a' should also in a register
		, [b]      "r"  (b)        // 'b' should also be in a register
		: "cc"                     // Clobbers condition codes
	);
	return result;
}
#endif

/*-------------------------------------------------
    div_64x32_rem - perform a signed 64 bit x 32
    bit divide and return the 32 bit quotient and
    32 bit remainder
-------------------------------------------------*/

#if defined(__aarch64__)
#define div_64x32_rem _div_64x32_rem
inline int32_t ATTR_FORCE_INLINE
_div_64x32_rem(int64_t dividend, int32_t divisor, int32_t *remainder)
{
	int32_t quotient;
	int32_t remains;
        __asm__ (
                " sxtw   %x[divisor], %w[divisor] ;"
                " sdiv   %x[quotient], %x[dividend], %x[divisor] ;"
		" msub   %x[remains], %x[quotient], %x[divisor], %x[dividend] ;"
                : [quotient]  "=r" (quotient)   // quotient in a register
		, [remains]   "=r" (remains)    // remains in a register
                : [dividend]  "%r" (dividend)   // 'dividend' should also in a register
                , [divisor]   "r"  (divisor)    // 'divisor' should also be in a register
                : "cc"                          // Clobbers condition codes
        );
	*remainder = (int32_t)remains;
        return quotient;
}
#endif

/*-------------------------------------------------
    divu_64x32_rem - perform an unsigned 64 bit x
    32 bit divide and return the 32 bit quotient
    and 32 bit remainder
-------------------------------------------------*/

#if defined(__aarch64__)
#define divu_64x32_rem _divu_64x32_rem
inline uint32_t ATTR_FORCE_INLINE
_divu_64x32_rem(uint64_t dividend, uint32_t divisor, uint32_t *remainder)
{
        uint32_t quotient;
        uint32_t remains;
        __asm__ (
//              " uxtw   %x[divisor], %w[divisor] ;"
                " udiv   %x[quotient], %x[dividend], %x[divisor] ;"
                " msub   %x[remains], %x[quotient], %x[divisor], %x[dividend] ;"
                : [quotient]  "=r" (quotient)   // quotient in a register
                , [remains]   "=r" (remains)    // remains in a register
                : [dividend]  "%r" (dividend)   // 'dividend' should also in a register
                , [divisor]   "r"  (divisor)    // 'divisor' should also be in a register
                : "cc"                          // Clobbers condition codes
        );
        *remainder = (uint32_t)remains;
        return quotient;
}
#endif

#if 0 // ZERO

/*-------------------------------------------------
    div_32x32_shift - perform a signed divide of
    two 32 bit values, shifting the first before
    division, and returning the 32 bit quotient
-------------------------------------------------*/

#ifndef __x86_64__
#define div_32x32_shift _div_32x32_shift
inline int32_t ATTR_CONST ATTR_FORCE_INLINE
_div_32x32_shift(int32_t a, int32_t b, uint8_t shift)
{
	int32_t result;

	// Valid for (0 <= shift <= 31)
	// Throws arithmetic exception if result doesn't fit in 32 bits
	__asm__ (
		" cdq                          ;"
		" shldl  %[shift], %[a], %%edx ;"
		" shll   %[shift], %[a]        ;"
		" idivl  %[b]                  ;"
		: [result] "=&a" (result)   // result ends up in eax
		: [a]      "0"   (a)        // 'a' should also be in eax on entry
		, [b]      "rm"  (b)        // 'b' can be memory or register
		, [shift]  "Ic"  (shift)    // 'shift' must be constant in 0-31 range or in cl
		: "%edx", "cc"              // clobbers edx and condition codes
	);

	return result;
}
#endif


/*-------------------------------------------------
    divu_32x32_shift - perform an unsigned divide of
    two 32 bit values, shifting the first before
    division, and returning the 32 bit quotient
-------------------------------------------------*/

#ifndef __x86_64__
#define divu_32x32_shift _divu_32x32_shift
inline uint32_t ATTR_CONST ATTR_FORCE_INLINE
_divu_32x32_shift(uint32_t a, uint32_t b, uint8_t shift)
{
	int32_t result;

	// Valid for (0 <= shift <= 31)
	// Throws arithmetic exception if result doesn't fit in 32 bits
	__asm__ (
		" clr    %%edx                 ;"
		" shldl  %[shift], %[a], %%edx ;"
		" shll   %[shift], %[a]        ;"
		" divl   %[b]                  ;"
		: [result] "=&a" (result)   // result ends up in eax
		: [a]      "0"   (a)        // 'a' should also be in eax on entry
		, [b]      "rm"  (b)        // 'b' can be memory or register
		, [shift]  "Ic"  (shift)    // 'shift' must be constant in 0-31 range or in cl
		: "%edx", "cc"              // clobbers edx and condition codes
	);

	return result;
}
#endif


/*-------------------------------------------------
    mod_64x32 - perform a signed 64 bit x 32 bit
    divide and return the 32 bit remainder
-------------------------------------------------*/

#ifndef __x86_64__
#define mod_64x32 _mod_64x32
inline int32_t ATTR_CONST ATTR_FORCE_INLINE
_mod_64x32(int64_t a, int32_t b)
{
	int32_t result, temp;

	// Throws arithmetic exception if quotient doesn't fit in 32 bits
	__asm__ (
		" idivl  %[b] ;"
		: [result] "=d" (result)    // Result ends up in edx
		, [temp]   "=a" (temp)      // This is effectively a clobber
		: [a]      "A"  (a)         // 'a' in edx:eax
		, [b]      "rm" (b)         // 'b' in register or memory
		: "cc"                      // Clobbers condition codes
	);

	return result;
}
#endif


/*-------------------------------------------------
    modu_64x32 - perform an unsigned 64 bit x 32 bit
    divide and return the 32 bit remainder
-------------------------------------------------*/

#ifndef __x86_64__
#define modu_64x32 _modu_64x32
inline uint32_t ATTR_CONST ATTR_FORCE_INLINE
_modu_64x32(uint64_t a, uint32_t b)
{
	uint32_t result, temp;

	// Throws arithmetic exception if quotient doesn't fit in 32 bits
	__asm__ (
		" divl  %[b] ;"
		: [result] "=d" (result)    // Result ends up in edx
		, [temp]   "=a" (temp)      // This is effectively a clobber
		: [a]      "A"  (a)         // 'a' in edx:eax
		, [b]      "rm" (b)         // 'b' in register or memory
		: "cc"                      // Clobbers condition codes
	);

	return result;
}
#endif


/*-------------------------------------------------
    recip_approx - compute an approximate floating
    point reciprocal
-------------------------------------------------*/

#ifdef __SSE2__
#define recip_approx _recip_approx
inline float ATTR_CONST
_recip_approx(float value)
{
	__m128 const value_xmm = _mm_set_ss(value);
	__m128 const result_xmm = _mm_rcp_ss(value_xmm);
	float result;
	_mm_store_ss(&result, result_xmm);
	return result;
}
#endif


/*-------------------------------------------------
    mul_64x64 - perform a signed 64 bit x 64 bit
    multiply and return the full 128 bit result
-------------------------------------------------*/

#ifdef __x86_64__
#define mul_64x64 _mul_64x64
inline int64_t ATTR_FORCE_INLINE
_mul_64x64(int64_t a, int64_t b, int64_t *hi)
{
	__int128 const r(__int128(a) * b);
	*hi = int64_t(uint64_t((unsigned __int128)r >> 64));
	return int64_t(uint64_t((unsigned __int128)r));
}
#endif


/*-------------------------------------------------
    mulu_64x64 - perform an unsigned 64 bit x 64
    bit multiply and return the full 128 bit result
-------------------------------------------------*/

#ifdef __x86_64__
#define mulu_64x64 _mulu_64x64
inline uint64_t ATTR_FORCE_INLINE
_mulu_64x64(uint64_t a, uint64_t b, uint64_t *hi)
{
	unsigned __int128 const r((unsigned __int128)a * b);
	*hi = uint64_t(r >> 64);
	return uint64_t(r);
}
#endif



/***************************************************************************
    INLINE BIT MANIPULATION FUNCTIONS
***************************************************************************/

/*-------------------------------------------------
    count_leading_zeros - return the number of
    leading zero bits in a 32-bit value
-------------------------------------------------*/

#define count_leading_zeros _count_leading_zeros
inline uint8_t ATTR_CONST ATTR_FORCE_INLINE
_count_leading_zeros(uint32_t value)
{
	uint32_t result;
	__asm__ (
		" bsrl    %[value], %[result] ;"
		" cmovzl  %[bias], %[result]  ;"
		: [result] "=&r" (result)       // result can be in any register
		: [value]  "rm"  (value)        // 'value' can be register or memory
		, [bias]   "rm"  (~uint32_t(0)) // 'bias' can be register or memory
		: "cc"                          // clobbers condition codes
	);
	return 31U - result;
}


/*-------------------------------------------------
    count_leading_ones - return the number of
    leading one bits in a 32-bit value
-------------------------------------------------*/

#define count_leading_ones _count_leading_ones
inline uint8_t ATTR_CONST ATTR_FORCE_INLINE
_count_leading_ones(uint32_t value)
{
	uint32_t result;
	__asm__ (
		" bsrl    %[value], %[result] ;"
		" cmovzl  %[bias], %[result]  ;"
		: [result] "=&r" (result)       // result can be in any register
		: [value]  "rm"  (~value)       // 'value' can be register or memory
		, [bias]   "rm"  (~uint32_t(0)) // 'bias' can be register or memory
		: "cc"                          // clobbers condition codes
	);
	return 31U - result;
}

#endif // ZERO

#endif // MAME_OSD_EIGCCARM_H
