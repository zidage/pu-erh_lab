/*
 * @file        pu-erh_lab/src/include/image/image_loader.hpp
 * @brief       A module to load images into memory
 * @author      Yurun Zi
 * @date        2025-03-25
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

#pragma once

#include "decoders/decoder_scheduler.hpp"
#include "image/image_loader.hpp"
#include "sleeve/sleeve_base.hpp"
#include "sleeve/sleeve_buffer/sleeve_buffer.hpp"
#include "image/image.hpp"
#include "type/type.hpp"
#include "utils/queue/queue.hpp"


#include <cstddef>
#include <cstdint>
#include <future>
#include <memory>
#include <vector>
namespace puerhlab {

class ImageLoader {
private:
  // Type alias
  using BufferQueue = NonBlockingQueue<std::shared_ptr<Image>>;
  // Image decoding part
  std::shared_ptr<BufferQueue> _buffer_decoded;
  uint32_t _buffer_size;
  size_t _use_thread;
  uint32_t _start_id;
  uint32_t _next_id;
  DecoderScheduler _decoder_scheduler;
  
  std::vector<std::shared_ptr<std::promise<uint32_t>>> promises;
  std::vector<std::future<uint32_t>> futures;

  
public:
  explicit ImageLoader(uint32_t buffer_size, size_t _use_thread,
                       uint32_t start_id);

  void StartLoading(std::vector<image_path_t> images, DecodeType decode_type);
	auto LoadImage() -> std::shared_ptr<Image>;
  };
}; // namespace puerhlab