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
#include "Foundation/efwFile.h"

namespace efw
{
	namespace FileReader
	{
		int32_t Read(void* outData, int32_t outDataSize, const char* filename);
		int32_t ReadAll(void** outData, int32_t* outSize, const char* filename, int32_t requiredAlignment = File::kDefaultDataAlignment);
	}

} // efw