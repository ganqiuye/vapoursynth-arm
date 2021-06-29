/*================================================================
*
*   File:       main.cpp
*   Author:     Gan Qiuye(ganqiuye@163.com)
*   Date:       2021-04-22  17:29
*   Version:        1.0
*   Description:
*
================================================================*/
#include "VapourSynth.h"
#include <stdio.h>
#include <malloc.h>
#include <iostream>
#include "VSHelper.h"
#include "ffms2Wrapper.h"
#include "nnedi3Wrapper.h"
#include "QTGMC.h"

const char* srcVideoFile = "/sdcard/Movies/tt.ts";
const char* outVideoFile = "/sdcard/image/out.yuv";

static int saveYUV(const VSAPI* vsapi, VSNodeRef *node)
{
    FILE *outFile = NULL;
    char errMsg[1024];
    int  error = 0;
    const VSVideoInfo *vi = vsapi->getVideoInfo(node);
    if (!isConstantFormat(vi) || !vi->numFrames) {
        fprintf(stderr, "Cannot output clips with varying dimensions or unknown length\n");
        return -1;
    }
    printf("====w:%d, h:%d, fps:%lld, numFrames:%d\n", vi->width, vi->height, vi->fpsNum, vi->numFrames);
    outFile = fopen(outVideoFile, "wb");
    if (!outFile) {
        fprintf(stderr, "Failed to open output for writing\n");
        return -1;
    }
    for (int n = 0; n < vi->numFrames; n++) {
        const VSFrameRef *frame = vsapi->getFrame(n, node, errMsg, sizeof(errMsg));

        if (!frame) { // Check if an error happened when getting the frame
            error = 1;
            break;
        }

        // Loop over every row of every plane write to the file
        for (int p = 0; p < vi->format->numPlanes; p++) {
            int stride = vsapi->getStride(frame, p);
            const uint8_t *readPtr = vsapi->getReadPtr(frame, p);
            int rowSize = vsapi->getFrameWidth(frame, p) * vi->format->bytesPerSample;
            int height = vsapi->getFrameHeight(frame, p);

            for (int y = 0; y < height; y++) {
                // You should probably handle any fwrite errors here as well
                fwrite(readPtr, rowSize, 1, outFile);
                readPtr += stride;
            }
        }

        vsapi->freeFrame(frame);
    }

    fclose(outFile);
    return 0;
}

int main(int argc, char* argv[])
{
	const VSAPI* vsapi = getVapourSynthAPI(VAPOURSYNTH_API_VERSION);
	VSCore *core = vsapi->createCore(1);
    FFms2Wrapper* ffms2 = new FFms2Wrapper(vsapi, core);
    VSNodeRef *node = ffms2->setSource(srcVideoFile);
    MiniQTGMC* qtgmc = new MiniQTGMC(vsapi, core, node);
    node = qtgmc->process();

    saveYUV(vsapi, node);

OUT:
    delete ffms2;
    delete qtgmc;
    vsapi->freeNode(node);
    vsapi->freeCore(core);
	return 0;
}