/**
 * Console
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
	namespace Console
	{
		const int32_t kWarning = 1;
		const int32_t kError = 2;
	}

	namespace Console
	{
		void Write(const char* str, ...);
		void WriteLine(const char* str, ...);

		void Write(int32_t messageType, const char* str, ...);
		void WriteLine(int32_t messageType, const char* str, ...);
	}

}