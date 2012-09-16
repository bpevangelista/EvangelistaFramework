/**
 * 
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

#include "Foundation/efwPlatform.h"
#include <math.h>

namespace efw
{
	namespace Math
	{
		inline uint32_t Min(uint32_t v1, uint32_t v2) { return (v1<v2)? v1 : v2; }
		inline uint32_t Max(uint32_t v1, uint32_t v2) { return (v1<v2)? v2 : v1; }
		inline int32_t Min(int32_t v1, int32_t v2) { return (v1<v2)? v1 : v2; }
		inline int32_t Max(int32_t v1, int32_t v2) { return (v1<v2)? v2 : v1; }
		inline float Min(float v1, float v2) { return (v1<v2)? v2 : v1; }
		inline float Max(float v1, float v2) { return (v1<v2)? v2 : v1; }

		inline float Sqrt(float v) { return sqrtf(v); }
	}

} // efw