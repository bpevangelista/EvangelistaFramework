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
#include "Graphics/efwTexture.h"

namespace efw
{

namespace Graphics
{
	// Forward declarations
	struct UnprocessedMaterialTexture;

	namespace TextureReader
	{
		int32_t GetTextureFileType(int32_t* outTextureFileType, const char* textureName);
		
		int32_t ReadImage(UnprocessedMaterialTexture** outTexture, const char* textureName);
		int32_t ReadTGA(UnprocessedMaterialTexture** outTexture, const char* textureName);
	}

} // Graphics
} // efw