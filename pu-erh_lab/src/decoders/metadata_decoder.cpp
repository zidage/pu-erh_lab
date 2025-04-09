/*
 * @file        pu-erh_lab/src/include/decoders/metadata_decoder.hpp
 * @brief       A decoder used to read metadata in a image file, no image data will be loaded.
 * @author      Yurun Zi
 * @date        2025-04-08
 * @license     MIT
 *
 * @copyright   Copyright (c) 2025 Yurun Zi
 */

// Copyright (c) 2025 Yurun Zi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "decoders/metadata_decoder.hpp"
#include "type/type.hpp"
#include <cstdlib>
#include <filesystem>

namespace puerhlab {
/**
 * @brief A callback used to parse the basic information of a image file
 * 
 * @param buffer 
 * @param file_path 
 * @param result 
 * @param id 
 * @param promise 
 */
void MetadataDecoder::Decode(std::vector<char> buffer, std::filesystem::path file_path,
            std::shared_ptr<BufferQueue> result, image_id_t id,
            std::shared_ptr<std::promise<image_id_t>> promise) {
  
  try {
    auto exiv2_img = Exiv2::ImageFactory::open(
        (const Exiv2::byte *)buffer.data(), buffer.size());
    Exiv2::ExifData &exifData = exiv2_img->exifData();

    // Push the decoded image into the buffer queue
    std::shared_ptr<Image> img = std::make_shared<Image>(
        id, file_path.wstring(), ImageType::DEFAULT, Exiv2::ExifData(exifData));
    result->push(img);
    promise->set_value(id);
	} catch (std::exception &e) {
    // TODO: Append error message to log
  }
}
};