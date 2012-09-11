/**
 * Platform and compiler code
 * @author Bruno Evangelista
 *
 * THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */
#pragma once

#if defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS		// Disable non-secure function calls warning
#pragma warning(disable:4200)		// Disable zero sized arrays warning
#else
#include <unistd.h>
#endif

#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#if defined _MSC_VER
#define INLINE inline
INLINE void* memalign(size_t alignment, size_t size) { return _aligned_malloc(size, alignment); }
INLINE void freealign(void* address) { _aligned_free(address); }
#define PACKED( _DECL ) __pragma( pack(push, 1) ) _DECL __pragma( pack(pop) )
#elif defined __GNUC__
#define PACKED( _DECL ) _DECL __attribute__((packed))
#define INLINE inline
#else
#error Compiled not defined!
#endif

#ifndef NULL
#define NULL 0L
#endif

// Only works with power of 2
#ifndef ALIGN
#define ALIGN(alignment, value) (((value)+(alignment)-1) & ~((alignment)-1))
#endif

#ifndef SAFE_ALIGNED_FREE
#define SAFE_ALIGNED_FREE(a) do { if((a) != NULL) { freealign(a); (a) = NULL; } } while(false)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(a) do { if((a) != NULL) { delete(a); (a) = NULL; } } while(false)
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(a) do { if((a) != NULL) { delete [] (a); (a) = NULL; } } while(false)
#endif

// TODO: Needs to be improved
#ifndef COUNTOF
#define COUNTOF(a) (sizeof(a)/sizeof(*(a)))
#endif

#ifndef MIN
#define MIN(a, b) ( ((a) < (b))? (a) : (b) )
#endif

#ifndef MAX
#define MAX(a, b) ( ((a) > (b))? (a) : (b) )
#endif

#ifndef UNUSED
#define UNUSED(a) (void)(a)
#endif

// Temporary: TODO FIX
#include <assert.h>
#ifndef EFW_ASSERT
	#if defined _DEBUG
	#define EFW_ASSERT(a) assert(a)
	#else 
	#define EFW_ASSERT(a) UNUSED(a)
	#endif
#endif

// 
namespace Path
{
	const int32_t kMaxDirectoryLength = 1024;
	const int32_t kMaxFileNameLength = 256;
	const int32_t kMaxFullPathLength = ALIGN(4, (kMaxDirectoryLength + kMaxFileNameLength));
};

// Generic types
namespace efwErrs
{
enum efwErr
{
	kOk,
	kCorruptedData,
	kInvalidInput,
	kInvalidState,
	kInvalidOperation
};
}

typedef efwErrs::efwErr efwErr;