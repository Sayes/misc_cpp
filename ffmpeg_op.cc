/*
 * Copyright (c) 2017-2018 SYZ
 *
 * https://github.com/FFmpeg/FFmpeg.git
 *
 * g++ h264decoder_rk.cc ffmpeg_op.cc -std=c++11 -I. -I/usr/local/ffmpeg/include
 * -I/usr/local/opencv/include -L/usr/local/opencv/lib -lopencv_core
 * -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio
 * -L/usr/local/ffmpeg/lib -lavdevice -lavutil -lavformat -lavcodec -lswscale
 * -I/home/syz/workshop/opensource/mpp/inc
 * -I/home/syz/workshop/opensource/mpp/osal/inc
 * -I/home/syz/workshop/opensource/mpp/utils
 * -I/home/syz/workshop/opensource/mpp/mpp/base/inc
 * -L/home/syz/workshop/opensource/mpp/build/linux/x86_64/mpp -lrockchip_mpp -o
 * release/ffmpeg_op
 *
 * check output yuv image
 * ffplay -f rawvideo -video_size 640x480 output.yuv
 */

#include <cstdio>
#include <cstring>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>
#include <libswscale/swscale.h>
}

#define SUPPORT_OPENCV
#if defined(SUPPORT_OPENCV)
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif

#include "constdef.h"
#include "h264decoder_rk.h"

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("ffmpeg_op /dev/video0\n");
    return 0;
  }
  AVFormatContext* input_ctx = nullptr;
  AVDictionary* options = nullptr;
  AVPacket* packet = nullptr;
  AVFrame* orig_frm = nullptr;
  AVFrame* out_frm = nullptr;
  uint8_t* out_buffer = nullptr;

  do {
    bool USE_MPP = false;
    if (avformat_network_init() < 0) {
      break;
    }

    input_ctx = avformat_alloc_context();
    if (!input_ctx) {
      break;
    }

    avdevice_register_all();
    av_dict_set(&options, "buffer_size", "2048000", 0);
    av_dict_set(&options, "rtsp_transport", "tcp", 0);
    av_dict_set(&options, "stimeout", "5000000", 0);
    av_dict_set(&options, "max_delay", "500000", 0);
    if (avformat_open_input(&input_ctx, argv[1], nullptr, &options) < 0) {
      avformat_free_context(input_ctx);
      break;
    }
    if (avformat_find_stream_info(input_ctx, nullptr) < 0) {
      break;
    }
    AVStream* input_video_stream = nullptr;
    for (int i = 0; i < input_ctx->nb_streams; ++i) {
      if (input_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        input_video_stream = input_ctx->streams[i];
        break;
      }
    }
    if (!input_video_stream) {
      break;
    }

    AVCodec* codec = nullptr;
    AVCodecContext* decodec_ctx = nullptr;

    seetacams::H264DecoderRk h264decoder_rk;
    if (USE_MPP) {
      // rknn decoder
      if (h264decoder_rk.init(input_video_stream->codec,
                              seetacams::E_CHANNEL_CODEC::H264) != 0) {
        break;
      }
      h264decoder_rk.set_fmt(seetacams::E_CHANNEL_FMT::JPEG);
    } else {
      // cpu decoder
      avcodec_register_all();
      codec = avcodec_find_decoder(input_video_stream->codec->codec_id);
      if (!codec) {
        break;
      }
      decodec_ctx = avcodec_alloc_context3(codec);
      avcodec_parameters_to_context(decodec_ctx, input_video_stream->codecpar);
      if (avcodec_open2(decodec_ctx, codec, nullptr) < 0) {
        break;
      }
    }

    packet = av_packet_alloc();
    av_init_packet(packet);

    orig_frm = av_frame_alloc();
    if (!orig_frm) {
      break;
    }

#if defined(SUPPORT_OPENCV)
    cv::Mat mat;
#endif
    int got_frm = 0;

    // begin loop
    int loop_cnt = 0;
    while (1) {
      int read_status = -1;
      do {
        read_status = av_read_frame(input_ctx, packet);
      } while (read_status == AVERROR(EAGAIN));

      if (read_status >= 0) {
        if (USE_MPP) {
          // rk decoder
          int w = 0;
          if (h264decoder_rk.decode(packet)) {
#if defined(SUPPORT_OPENCV)
            mat = h264decoder_rk.getMat();
            w = mat.cols;
#endif
            if (w > 0) {
              got_frm = 1;
              break;
            }
          }
        } else {
          // cpu decoder
          int len =
              avcodec_decode_video2(decodec_ctx, orig_frm, &got_frm, packet);
          if (len >= 0) {
            break;
          }
        }
        av_packet_unref(packet);
      }
      if (loop_cnt++ > 20) {
        printf("loop 20 times\n");
        break;
      }
    }  // end loop
    if (!got_frm) {
      break;
    }

    if (USE_MPP) {
#if defined(SUPPORT_OPENCV)
      printf("width %d height %d\n", mat.cols, mat.rows);
      cv::imwrite("./output.jpg", mat);
#endif
    } else {
      SwsContext* conv_YUV420_ctx = sws_getContext(
          decodec_ctx->width, decodec_ctx->height, decodec_ctx->pix_fmt,
          decodec_ctx->width, decodec_ctx->height, AV_PIX_FMT_YUV420P,
          SWS_BICUBIC, nullptr, nullptr, nullptr);
      if (!conv_YUV420_ctx) {
        break;
      }

      out_frm = av_frame_alloc();
      if (!out_frm) {
        break;
      }
      int out_buffer_size = av_image_get_buffer_size(
          decodec_ctx->pix_fmt, decodec_ctx->width, decodec_ctx->height, 1);

      out_buffer = (uint8_t*)av_malloc(out_buffer_size);
      av_image_fill_arrays(out_frm->data, out_frm->linesize, out_buffer,
                           AV_PIX_FMT_YUV420P, decodec_ctx->width,
                           decodec_ctx->height, 1);

      sws_scale(conv_YUV420_ctx, orig_frm->data, orig_frm->linesize, 0,
                decodec_ctx->height, out_frm->data, out_frm->linesize);

      printf("width %d height %d\n", decodec_ctx->width, decodec_ctx->height);
      FILE* fp = fopen("./output.yuv", "w");
      if (!fp) {
        break;
      }
      fwrite(out_buffer, out_buffer_size, 1, fp);
      fclose(fp);

#if defined(SUPPORT_OPENCV)
      cv::Mat yuvImg;
      yuvImg.create(decodec_ctx->height * 3 / 2, decodec_ctx->width, CV_8UC1);
      memcpy(yuvImg.data, out_buffer,
             decodec_ctx->width * decodec_ctx->height * 3 / 2);
      if (yuvImg.empty()) {
        break;
      }
      cv::cvtColor(yuvImg, mat, CV_YUV420p2RGB);
      cv::imwrite("./output.jpg", mat);
#endif
    }

  } while (false);

  if (options) {
    av_dict_free(&options);
  }
  if (out_buffer) {
    av_free(out_buffer);
  }
  if (out_frm) {
    av_frame_free(&out_frm);
  }
  av_packet_free(&packet);
  if (orig_frm) {
    av_frame_free(&orig_frm);
  }
  if (input_ctx) {
    avformat_close_input(&input_ctx);
  }
  avformat_network_deinit();
  return 0;
}
