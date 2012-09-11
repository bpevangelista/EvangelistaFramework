/**
 * TODO
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

#include <string>
//#include <wchar.h>

namespace efw
{
	namespace PathHelper
	{
		int32_t GetDirectory(char* outPath, int32_t outPathLength, const char* fullPath);
		int32_t GetFileName(char* outPath, int32_t outPathLength, const char* fullPath);
		int32_t GetFileExtension(char* outPath, int32_t outPathLength, const char* path);
		int32_t GetFileNameWithoutExtension(char* outPath, int32_t outPathLength, const char* fullPath);
		
		int32_t Combine(char* outPath, int32_t outPathLength, const char* path, const char* fileName);
	}

}