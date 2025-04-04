/*
 * @file        pu-erh_lab/src/include/image/image_loader.hpp
 * @brief       A handler to load images directly into the sleeve or pass to raw decoder
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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once


#include "type/type.hpp"
#include "image/image.hpp"
#include "decoders/raw_decoder.hpp"
#include "utils/queue/queue.hpp"

#include <cstdint>
#include <vector>
namespace puerhlab {

struct ImageLoadRequest {
  image_path_t image_path;
  
};

class ImageLoader {
 private:
  std::vector<image_path_t> _queued_files;
  NonBlockingQueue<ImageLoadRequest> _load_queue;

  std::atomic<uint32_t> _total_request_size;
  std::atomic<uint32_t> _completed_request_size;

 public:
  
};
};