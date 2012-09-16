/**
 * Mesh intermediate format
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
	struct UnprocessedMaterial
	{
		Texture* albedoTexture;
		Texture* normalMapTexture;

		char albedoTextureFilename[Path::kMaxFullPathLength];
		char normalMapTextureFilename[Path::kMaxFullPathLength];
		//UnprocessedMaterialTexture specularTexture;

		static const int32_t kMaxNameLength = 64;
		int64_t nameHash;
		char name[kMaxNameLength];
	};

	struct UnprocessedMaterialLib
	{
		int32_t materialCount;
		UnprocessedMaterial materials[];
	};
}
}