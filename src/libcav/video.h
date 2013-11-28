/*
 * video.h
 * Copyright (C) 2013  Ilan Pegoraro and Luís Neves
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CAV_VIDEO_H
#define CAV_VIDEO_H

#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "frame.h"

/**
 * Class that has all the information about the Video and manages accessing/displaying the Frames.
 */

class Video
{
public:
	/**
	 * Constructs a video reading from the first webcam.
	 */
	Video();
	/** Construct a Video object from a device.
	 * @param device - device number
	 */
	Video(int device);
	/** Construct a Video object from a file.	
	 * @param path - file path of the  
	 */
	Video(const std::string& path);
	/**
	 * Construct a Video object for writing.	
	 * @param fpath  - File path of the file
	 * @param rows - Number of rows
	 * @param cols - Number of columns.
	 * @param fps - Frames per second.
	 * @param format - VideoFormat.
	 */
	Video(const std::string& fpath, uint rows, uint cols, uint fps, VideoFormat format);
	/**
	 * Video Destructor.
	 * Cleans the internal state of the class, closes the stream.
	 */
	virtual ~Video();

	/**
	 * Gets the rows that each frame represents
	 * @return uint - Number of rows
	 */
	uint rows();
	/**
	 * Gets the columns that each frame represents
	 * @return uint - Number of columns
	 */
	uint cols();
	/**
	 * Gets the frames per second
	 * @return uint - Number of frames per second
	 */
	uint fps();
	/**
	 * Gets the video's format.
	 * @return VideoFormat - Current VideoFormat.
	 */
	VideoFormat format();

	/**
	 * Returns the videos next frame. Note that the frame is dynamically allocated, so the resposability of deleting it is delegated to the caller.
	 * @throws VideoEndedException
	 * @return Frame* - Dynamically allocated frame.
	 */
	Frame* getFrame();
	/**
	 * Writes the frame to the mass storage device.
	 * pre-condition: the frame to be written must match the videos format.
	 * @param f - A reference to a frame
	 * @return Frame - the next frame
	 */
	void putFrame(Frame& f);

	/**
	 * Sets the posistion of the file to the begining.
	 */
	void reset();
	/**
	 * Displays the video in a window.
	 * @param playing - if true the video runs with its FPS, otherwise a key must be pressed to step through the frames. (default: true)
	 */
	void display(bool playing = true);

	/**
	 * Converts a video to the new format. To avoid problems the video is restart to the begining.
	 * @param path - path to the video.
	 * @param dest - out video format.
	 */
	void convert(const std::string& path, VideoFormat dest);

	/**
	 * Gets the videos number of frames.
	 * @return total number of frames
	 */
	uint getTotalFrames();

protected:
	inline uint getFrameSize()
	{
		switch(m_type)
		{
			case RGB:
			case YUV_444: return m_rows * m_cols * 3;
			break;
			case YUV_422: return m_rows * m_cols + (m_rows * m_cols / 2) * 2; 
			break;
			case YUV_420: return m_rows * m_cols + (m_rows / 2 * m_cols / 2) * 2;
			break;
		}
		return 0;
	}
	/**
	 * File stream to read of write the video (YUV444, YUV422 and YUV422 formats).
	 */
	std::fstream m_stream;
	/**
	 * Number of rows of the videos
	 */
	uint m_rows;
	/**
	 * Number of columns of the videos
	 */
	uint m_cols;
	/**
	 * Frame rate (Frames per second)
	 */
	uint m_fps;
	/**
	 * Whether we're reading from camera (or avi files) or YUV.
	 */
	bool m_fromCam;
	/**
	 * Access to the RGB video (camera or .avi)
	 */
	cv::VideoCapture m_video;
	/**
	 * Video's format.
	 */
	VideoFormat m_type;
	/** Header's size. */
	uint m_headerSize;
};

#endif