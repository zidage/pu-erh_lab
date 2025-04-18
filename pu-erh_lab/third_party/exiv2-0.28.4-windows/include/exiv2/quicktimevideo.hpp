// ***************************************************************** -*- C++ -*-
/*
 * Copyright (C) 2004-2021 Exiv2 authors
 * This program is part of the Exiv2 distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301 USA.
 */
#ifndef QUICKTIMEVIDEO_HPP_
#define QUICKTIMEVIDEO_HPP_

// *****************************************************************************
#include "exiv2lib_export.h"

// included header files
#include "exif.hpp"
#include "image.hpp"

// *****************************************************************************
// namespace extensions
namespace Exiv2 {

// *****************************************************************************
// class definitions

/*!
  @brief Class to access QuickTime video files.
 */
class EXIV2API QuickTimeVideo : public Image {
 public:
  //! @name Creators
  //@{
  /*!
    @brief Constructor for a QuickTime video. Since the constructor
        can not return a result, callers should check the good() method
        after object construction to determine success or failure.
    @param io An auto-pointer that owns a BasicIo instance used for
        reading and writing image metadata. \b Important: The constructor
        takes ownership of the passed in BasicIo instance through the
        auto-pointer. Callers should not continue to use the BasicIo
        instance after it is passed to this method. Use the Image::io()
        method to get a temporary reference.
   */
  explicit QuickTimeVideo(BasicIo::UniquePtr io, size_t max_recursion_depth = 1000);
  //@}

  //! @name Manipulators
  //@{
  void readMetadata() override;
  void writeMetadata() override;
  //@}

  //! @name Accessors
  //@{
  [[nodiscard]] std::string mimeType() const override;
  //@}

 protected:
  /*!
    @brief Check for a valid tag and decode the block at the current IO
    position. Calls tagDecoder() or skips to next tag, if required.
   */
  void decodeBlock(size_t recursion_depth, std::string const& entered_from = "");
  /*!
    @brief Interpret tag information, and call the respective function
        to save it in the respective XMP container. Decodes a Tag
        Information and saves it in the respective XMP container, if
        the block size is small.
    @param buf Data buffer which contains tag ID.
    @param size Size of the data block used to store Tag Information.
   */
  void tagDecoder(Exiv2::DataBuf& buf, size_t size, size_t recursion_depth);

 private:
  /*!
    @brief Interpret file type of the video, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void fileTypeDecoder(size_t size);
  /*!
    @brief Interpret Media Header Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void mediaHeaderDecoder(size_t size);
  /*!
    @brief Interpret Video Header Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void videoHeaderDecoder(size_t size);
  /*!
    @brief Interpret Movie Header Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void movieHeaderDecoder(size_t size);
  /*!
    @brief Interpret Track Header Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void trackHeaderDecoder(size_t size);
  /*!
    @brief Interpret Handler Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void handlerDecoder(size_t size);
  /*!
    @brief Interpret Tag which contain other sub-tags,
        and save it in the respective XMP container.
   */
  void multipleEntriesDecoder(size_t recursion_depth);
  /*!
    @brief Interpret Sample Description Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void sampleDesc(size_t size);
  /*!
    @brief Interpret Image Description Tag, and save it
        in the respective XMP container.
   */
  void imageDescDecoder();
  /*!
    @brief Interpret User Data Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void userDataDecoder(size_t size, size_t recursion_depth);
  /*!
    @brief Interpret Preview Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void previewTagDecoder(size_t size);
  /*!
    @brief Interpret Meta Keys Tags, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void keysTagDecoder(size_t size);
  /*!
    @brief Interpret Track Aperture Tags, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void trackApertureTagDecoder(size_t size);
  /*!
    @brief Interpret Nikon Tag, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void NikonTagsDecoder(size_t size);
  /*!
    @brief Interpret Tags from Different Camera make, and save it
        in the respective XMP container.
    @param size Size of the data block used to store Tag Information.
   */
  void CameraTagsDecoder(size_t size);
  /*!
    @brief Interpret Audio Description Tag, and save it
        in the respective XMP container.
   */
  void audioDescDecoder();
  /*!
    @brief Helps to calculate Frame Rate from timeToSample chunk,
        and save it in the respective XMP container.
   */
  void timeToSampleDecoder();
  /*!
    @brief Recognizes which stream is currently under processing,
        and save its information in currentStream_ .
   */
  void setMediaStream();
  /*!
    @brief Used to discard a tag along with its data. The Tag will
        be skipped and not decoded.
    @param size Size of the data block that is to skipped.
   */
  void discard(size_t size);

  //! Variable which stores Time Scale unit, used to calculate time.
  uint64_t timeScale_ = 0;
  //! Variable which stores current stream being processed.
  int currentStream_ = 0;
  //! Variable to check the end of metadata traversing.
  bool continueTraversing_ = false;
  //! Variable to store height and width of a video frame.
  uint64_t height_ = 0;
  uint64_t width_ = 0;
  //! Prevent stack exhaustion due to excessively deep recursion.
  const size_t max_recursion_depth_;

};  // QuickTimeVideo End

// *****************************************************************************
// template, inline and free functions

// These could be static private functions on Image subclasses but then
// ImageFactory needs to be made a friend.
/*!
  @brief Create a new QuicktimeVideo instance and return an auto-pointer to it.
      Caller owns the returned object and the auto-pointer ensures that
      it will be deleted.
 */
EXIV2API Image::UniquePtr newQTimeInstance(BasicIo::UniquePtr io, bool create);

//! Check if the file iIo is a Quick Time Video.
EXIV2API bool isQTimeType(BasicIo& iIo, bool advance);

}  // namespace Exiv2

#endif  // QUICKTIMEVIDEO_HPP_
