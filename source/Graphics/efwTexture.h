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

namespace efw
{

namespace Graphics
{
	namespace TextureFileTypes
	{
		const int32_t kUnknown = 0;
		const int32_t kBMP = 1;
		const int32_t kDDS = 2;
		const int32_t kTGA = 3;
	}

	namespace TextureFormats
	{
		const int32_t kUnknown = 0;
		const int32_t kL8 = 1;
		const int32_t kRGB = 2;
		const int32_t kRGBA = 3;
		const int32_t kABGR = 4;
		const int32_t kDXT1 = 5;
		const int32_t kDXT3 = 6;
		const int32_t kDXT5 = 7;
	}

	// TODO Maybe separate this concept as Image2D and Texture?
	// A texture will also need to store: type (cube, volume?), filters, etc? Or should we separate samplers and texture states?
	struct TextureDesc
	{
		int32_t width;
		int32_t height;
		int32_t depth;
		int32_t mipCount;
		//int32_t arrayCount; // Future
		int32_t pitch;
		int32_t format;
	};

	struct Texture
	{
		TextureDesc desc;

		int32_t dataSize;
		void* data;
	};

} // Graphics
} // efw