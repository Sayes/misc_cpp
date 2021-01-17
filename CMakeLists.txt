CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(evhtp)

OPTION (SEETACAMS_DEBUG "" ON)

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

if (PLATFORM STREQUAL "rknn")

# ----------------------------------------------------------------------------
#  ffmpeg library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/mnt/b/workshop/opensource/FFmpeg-n4.0)
INCLUDE_DIRECTORIES(/home/syz/mnt/b/workshop/opensource/FFmpeg-n4.0/build_20201225_android)
LINK_DIRECTORIES(/home/syz/mnt/b/workshop/opensource/FFmpeg-n4.0/build_20201225_android/android/armv7-a/lib)
LINK_LIBRARIES(avutil avformat avcodec swresample swscale)

# ----------------------------------------------------------------------------
#  x264 library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/ffmpeg_android/jni/x264)
LINK_DIRECTORIES(/home/syz/workshop/opensource/ffmpeg_android/jni/build/lib)
LINK_LIBRARIES(x264)

# ----------------------------------------------------------------------------
#  zlib library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/zlib)
LINK_DIRECTORIES(/home/syz/workshop/opensource/zlib/build_android_20201208)
LINK_LIBRARIES(z)

# ----------------------------------------------------------------------------
#  opencv library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/opencv/modules/core/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/opencv/modules/videoio/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/opencv/modules/highgui/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/opencv/modules/imgcodecs/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/opencv/modules/imgproc/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/opencv/build_20201221_android)
LINK_DIRECTORIES(/home/syz/workshop/opensource/opencv/build_20201221_android/lib/armeabi-v7a)
LINK_LIBRARIES(opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_videoio)

# ----------------------------------------------------------------------------
#  libevent library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/libevent/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/libevent/build_20201229_android/include)
LINK_DIRECTORIES(/home/syz/workshop/opensource/libevent/build_20201229_android/lib)
LINK_LIBRARIES(event event_core event_extra)

# ----------------------------------------------------------------------------
#  jsoncpp library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/jsoncpp/include)
LINK_DIRECTORIES(/home/syz/workshop/opensource/jsoncpp/build_20201229_android/src/lib_json)
LINK_LIBRARIES(jsoncpp)

# ----------------------------------------------------------------------------
#  openssl library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/openssl/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/openssl/build_20210115_android/include)
#LINK_DIRECTORIES(/home/syz/workshop/opensource/openssl/build_20210115_android)
#LINK_LIBRARIES(crypto ssl)

# ----------------------------------------------------------------------------
# oniguruma library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/oniguruma/src)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/oniguruma/build_20210116_android)
LINK_DIRECTORIES(/home/syz/workshop/opensource/oniguruma/build_20210116_android)
LINK_LIBRARIES(onig)

# ----------------------------------------------------------------------------
# libevhtp library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/libevhtp/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/libevhtp/build_20210115_android/include)
LINK_DIRECTORIES(/home/syz/workshop/opensource/libevhtp/build_20210115_android)
LINK_LIBRARIES(evhtp)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(./src)
LINK_DIRECTORIES(/home/syz/Android/android-ndk-r20b/platforms/android-21/arch-arm/usr/lib)
LINK_LIBRARIES(c)

endif()



if (PLATFORM STREQUAL "x86_64")

# ----------------------------------------------------------------------------
#  ffmpeg library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/usr/local/ffmpeg/include)
LINK_DIRECTORIES(/usr/local/ffmpeg/lib)
LINK_LIBRARIES(avutil avformat avcodec swresample swscale)

# ----------------------------------------------------------------------------
#  x264 library
# ----------------------------------------------------------------------------
#INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/x264)
#LINK_DIRECTORIES(/home/syz/workshop/opensource/x264)
LINK_LIBRARIES(x264)

# ----------------------------------------------------------------------------
#  zlib library
# ----------------------------------------------------------------------------
LINK_LIBRARIES(z)

# ----------------------------------------------------------------------------
#  opencv library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/usr/local/opencv/include)
LINK_DIRECTORIES(/usr/local/opencv/lib)
LINK_LIBRARIES(opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_videoio)

# ----------------------------------------------------------------------------
#  libevent library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/usr/local/libevent/include)
LINK_DIRECTORIES(/usr/local/libevent/lib)
LINK_LIBRARIES(event event_openssl event_core event_extra)

# ----------------------------------------------------------------------------
#  jsoncpp library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/usr/local/jsoncpp/include)
LINK_DIRECTORIES(/usr/local/jsoncpp/lib)
LINK_LIBRARIES(jsoncpp)

# ----------------------------------------------------------------------------
#  openssl library
# ----------------------------------------------------------------------------
#INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/openssl/include)
#INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/openssl/build_20210115_android/include)
#LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu)
#LINK_LIBRARIES(crypto ssl)

# ----------------------------------------------------------------------------
# oniguruma library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/oniguruma/src)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/oniguruma/build_20210117_x86_64)
LINK_DIRECTORIES(/home/syz/workshop/opensource/oniguruma/build_20210117_x86_64)
LINK_LIBRARIES(onig)

# ----------------------------------------------------------------------------
# libevhtp library
# ----------------------------------------------------------------------------
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/libevhtp/include)
INCLUDE_DIRECTORIES(/home/syz/workshop/opensource/libevhtp/build/include)
LINK_DIRECTORIES(/home/syz/workshop/opensource/libevhtp/build)
LINK_LIBRARIES(evhtp)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(./src)
LINK_LIBRARIES(pthread)


endif()



ADD_EXECUTABLE(evhtp_op
        evhtp_op.c
        )
