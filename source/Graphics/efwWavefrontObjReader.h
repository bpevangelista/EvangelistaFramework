/**
 * Wavefront OBJ Reader
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
	// Forward declarations
	struct UnprocessedTriModel;
	struct UnprocessedMaterialLib;

	typedef int32_t (*ReadFileFunc_t)(void** outData, int32_t* outSize, const char* filename, int32_t requiredAlignment);

	namespace WavefrontObjReader
	{
		int32_t ReadModelAndMaterials(UnprocessedTriModel** outModel, UnprocessedMaterialLib** outMaterialLib, const char* fullFilePath,
			ReadFileFunc_t customReadFileFunction = NULL);

		int32_t ReadMaterialLib(UnprocessedMaterialLib** outMaterial, const char* fullFilePath, ReadFileFunc_t readFileFunc);

		// Deprecated
		//int32_t ReadModelFromStream(UnprocessedTriModel** outModel, const void* objFileData, uint32_t objFileDataSize);
	}

} // Graphics
} // efw