/*
 * Copyright (c) 2017-2018 SYZ
 *
 * https://github.com/FFmpeg/FFmpeg.git
 *
 * g++ ffmpeg_read_stream.cc -std=c++11 -I. -I/usr/local/ffmpeg/include
 * -L/usr/local/ffmpeg/lib -lavdevice -lavutil -lavformat -lavcodec -lswscale
 * -o ffmpeg_read_stream
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/error.h>
#include <libavutil/imgutils.h>
#include <libavutil/timestamp.h>
#include <libswscale/swscale.h>
}

#include "constdef.h"

int main(int argc, char** argv) {
  if (avformat_network_init() < 0) {
    printf("avformat_network_init() failed\n");
    return 0;
  }

AGAIN:
  AVFormatContext* input_ctx = nullptr;
  AVPacket* packet = nullptr;
  AVDictionary* options = nullptr;

  do {
    input_ctx = avformat_alloc_context();
    if (!input_ctx) {
      printf("avformat_alloc_context() failed\n");
      sleep(1);
      break;
    }

    avdevice_register_all();

    av_dict_set(&options, "buffer_size", "2048000", 0);
    av_dict_set(&options, "rtsp_transport", "tcp", 0);
    av_dict_set(&options, "stimeout", "5000000", 0);
    av_dict_set(&options, "max_delay", "500000", 0);

    int open_status =
        avformat_open_input(&input_ctx, argv[1], nullptr, &options);
    if (open_status < 0) {
      avformat_free_context(input_ctx);
      input_ctx = nullptr;

      char errbuf[1024] = {};
      av_strerror(open_status, errbuf, 1024);

      time_t now = time(nullptr);
      std::tm* curr_tm = localtime(&now);
      char time[80] = {0};
      strftime(time, 80, "%Y-%m-%d %H:%M:%S", curr_tm);

      printf("avformat_open_input() failed, error: %s, time: %s\n", errbuf,
             time);
      sleep(1);
      break;
    }
    if (avformat_find_stream_info(input_ctx, nullptr) < 0) {
      printf("avformat_find_stream_info() failed\n");
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
      printf("input_video_stream == null\n");
      break;
    }

    packet = av_packet_alloc();
    av_init_packet(packet);

    uint32_t loop_cnt = 0;
    while (1) {
      int read_status = -1;
      do {
        read_status = av_read_frame(input_ctx, packet);
      } while (read_status == AVERROR(EAGAIN));

      if (read_status >= 0) {
        // decode here
      } else {
        char errbuf[1024] = {};
        av_strerror(read_status, errbuf, 1024);

        time_t now = time(nullptr);
        std::tm* curr_tm = localtime(&now);
        char time[80] = {0};
        strftime(time, 80, "%Y-%m-%d %H:%M:%S", curr_tm);

        printf(
            "===================== av_read_frame() failed, error code :%d, "
            "error string %s, %s, frame count %d\n",
            read_status, errbuf, time, loop_cnt);
        break;
      }

      loop_cnt++;
      if (loop_cnt > 4294967290) {
        loop_cnt = 0;
      }

      av_packet_unref(packet);
    }

  } while (false);
  av_packet_unref(packet);

  if (options) {
    av_dict_free(&options);
  }
  av_packet_free(&packet);
  if (input_ctx) {
    avformat_close_input(&input_ctx);
  }

  goto AGAIN;

  avformat_network_deinit();
  return 0;
}
