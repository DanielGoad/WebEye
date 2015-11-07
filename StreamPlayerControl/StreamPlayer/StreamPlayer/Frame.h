#ifndef FFMPEG_FACADE_FRAME_H
#define FFMPEG_FACADE_FRAME_H

#include <cstdint>
#include <boost/noncopyable.hpp>

#pragma warning( push )
#pragma warning( disable : 4100 )

#include <boost/thread/mutex.hpp>

#pragma warning( pop )

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace FFmpeg
{

#pragma warning( push )
#pragma warning( disable : 4244 )

    extern "C"
    {
#include <libavcodec/avcodec.h>
    }

#pragma warning( pop )

    namespace Facade
    {
        /// <summary>
        /// A Frame class implements a set of frame-related utilities. 
        /// </summary>
        class Frame : private boost::noncopyable
        {
        public:
            /// <summary>
            /// Initializes a new instance of the Frame class.
            /// </summary>
            /// <param name="width">The width of the frame, in pixels.</param>
            /// <param name="height">The height of the frame, in pixels.</param>
            /// <param name="interframeDelayInMilliseconds">An interframe delay, in milliseconds.</param>
            /// <param name="avPicture">The AVPicture object to initialize the frame with.</param>
            Frame(uint32_t width, uint32_t height,
                int32_t interframeDelayInMilliseconds, AVPicture &avPicture);

            /// <summary>
            /// Gets the width of the frame, in pixels.
            /// </summary>
            uint32_t Width() const { return width_; }

            /// <summary>
            /// Gets the height of the frame, in pixels.
            /// </summary>
            uint32_t Height() const { return height_; }

            /// <summary>
            /// Updates the frame.
            /// </summary>
            /// <param name="avPicture">The AVPicture object to update the frame with.</param>
            /// <param name="interframeDelayInMilliseconds">an interframe delay, in milliseconds.</param>
            void Update(AVPicture &avPicture, int32_t interframeDelayInMilliseconds);

            /// <summary>
            /// Draws the frame.
            /// </summary>
            /// <param name="window">A container window that frame should be drawn on.</param>
            void Draw(HWND window);

            /// <summary>
            /// Converts the frame to a bitmap.
            /// </summary>
            /// <param name="bmpPtr">Address of a pointer to a byte that will receive the DIB.</param>
            void ToBmp(uint8_t **bmpPtr);

            /// <summary>
            /// Releases all resources used by the frame.
            /// </summary>
            ~Frame()
            {
                delete[] pixelsPtr_;
            }

        private:
            /// <summary>
            /// The bits in the array are packed together, but each scan line must be
            /// padded with zeros to end on a LONG data-type boundary.
            /// </summary>
            uint32_t GetPadding(int32_t lineSize) const
            {
                return lineSize % sizeof(uint32_t) > 0 ?
                    sizeof(uint32_t) - (lineSize % sizeof(uint32_t)) : 0;
            }

            int32_t width_, height_;
            uint8_t *pixelsPtr_;
            int32_t interframeDelayInMilliseconds_;
            boost::mutex mutex_;
        };
    }
}

#endif // FFMPEG_FACADE_FRAME_H