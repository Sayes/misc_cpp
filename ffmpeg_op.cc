/*
 *
 * g++ ffmpeg_op.cc -std=c++11 -I/usr/local/ffmpeg/include
 * -L/usr/local/ffmpeg/lib -lavdevice -lavutil -lavformat -lavcodec -lswresample
 * -lswscale -o release/ffmpeg_op
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

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("ffmpeg_op /dev/video0\n");
    return 0;
  }
  AVFormatContext* input_ctx = nullptr;
  AVPacket* packet = nullptr;
  AVFrame* orig_frm = nullptr;
  AVFrame* out_frm = nullptr;
  uint8_t* out_buffer = nullptr;
  do {
    if (avformat_network_init() < 0) {
      break;
    }
    avdevice_register_all();
    input_ctx = avformat_alloc_context();
    if (!input_ctx) {
      break;
    }
    if (avformat_open_input(&input_ctx, argv[1], nullptr, nullptr) < 0) {
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

    AVCodecContext* decodec_ctx = input_video_stream->codec;
    AVCodec* codec = avcodec_find_decoder(decodec_ctx->codec_id);
    if (!codec) {
      break;
    }
    if (avcodec_open2(decodec_ctx, codec, nullptr) < 0) {
      break;
    }
    packet = av_packet_alloc();
    if (av_read_frame(input_ctx, packet) >= 0) {
      int got_frm;
      orig_frm = av_frame_alloc();
      if (!orig_frm) {
        break;
      }
      int len = avcodec_decode_video2(decodec_ctx, orig_frm, &got_frm, packet);
      if (len < 0) {
        break;
      }
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
      int img_size = av_image_get_buffer_size(
          decodec_ctx->pix_fmt, decodec_ctx->width, decodec_ctx->height, 1);
      out_buffer = (uint8_t*)av_malloc(img_size);
      av_image_fill_arrays(out_frm->data, out_frm->linesize, out_buffer,
                           AV_PIX_FMT_YUV420P, decodec_ctx->width,
                           decodec_ctx->height, 1);

      if (got_frm) {
        sws_scale(conv_YUV420_ctx, orig_frm->data, orig_frm->linesize, 0,
                  decodec_ctx->height, out_frm->data, out_frm->linesize);
        FILE* fp = fopen("./output.yuv", "w");
        if (!fp) {
          break;
        }
        fwrite(out_buffer, img_size, 1, fp);
        fclose(fp);
      }
    }
  } while (false);
  if (!out_buffer) {
    av_free(out_buffer);
    out_buffer = nullptr;
  }
  if (!out_frm) {
    av_frame_free(&out_frm);
    out_frm = nullptr;
  }
  if (!packet) {
    av_packet_free_side_data(packet);
    av_packet_free(&packet);
    packet = nullptr;
  }
  if (!orig_frm) {
    av_frame_free(&orig_frm);
  }
  if (!input_ctx) {
    avformat_close_input(&input_ctx);
    avformat_free_context(input_ctx);
  }
  avformat_network_deinit();
  return 0;
}
