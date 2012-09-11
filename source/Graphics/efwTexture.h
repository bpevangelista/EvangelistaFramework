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

	namespace TextureDataFormats
	{
		const int32_t kUnknown = 0;
		const int32_t kRGB = 1;
		const int32_t kRGBA = 2;
		const int32_t kABGR = 3;
		const int32_t kDXT1 = 4;
		const int32_t kDXT3 = 5;
		const int32_t kDXT5 = 6;
	}

} // Graphics
} // efw